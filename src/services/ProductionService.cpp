#include "ProductionService.h"
#include <algorithm>
#include <cmath>

static const std::string LINE_IDLE    = "IDLE";
static const std::string LINE_RUNNING = "RUNNING";
static const std::string LINE_DONE    = "DONE";

// 실 생산량 = ceil(부족분 / (수율_비율 * 0.9))
static int calcActualQty(int shortfall, double yieldPct)
{
    if (shortfall <= 0) return 0;
    const double effectiveYield = (yieldPct / 100.0) * 0.9;
    if (effectiveYield <= 0.0) return shortfall;
    return static_cast<int>(std::ceil(static_cast<double>(shortfall) / effectiveYield));
}

ProductionService::ProductionService(IProductionLineRepository& lineRepo,
                                     IOrderRepository&          orderRepo,
                                     ISampleRepository&         sampleRepo,
                                     InventoryService&          inventoryService,
                                     OrderService&              orderService)
    : m_lineRepo(lineRepo)
    , m_orderRepo(orderRepo)
    , m_sampleRepo(sampleRepo)
    , m_inventoryService(inventoryService)
    , m_orderService(orderService)
{}

// ── 생산 시작 (FIFO Queue) ────────────────────────────────────────────────────

bool ProductionService::startNextInQueue(ProductionStartResult& result)
{
    const auto queued = getQueuedLines();
    if (queued.empty())
    {
        m_lastError = "생산 대기 중인 항목이 없습니다.";
        return false;
    }

    const ProductionLine& next  = queued.front();
    Order*                order = m_orderRepo.findById(next.orderId);
    if (!order)
    {
        m_lastError = "연결된 주문을 찾을 수 없습니다. (orderId=" + std::to_string(next.orderId) + ")";
        return false;
    }

    Sample* samplePtr = m_sampleRepo.findById(order->sampleId);
    if (!samplePtr)
    {
        m_lastError = "연결된 시료를 찾을 수 없습니다. (sampleId=" + std::to_string(order->sampleId) + ")";
        return false;
    }
    const Sample sample = *samplePtr;

    const int shortfall  = std::max(0, order->quantity - sample.stock);
    const int actualQty  = calcActualQty(shortfall, sample.yield);
    const int totalTime  = sample.avgProductionTime * actualQty;

    ProductionLine updated = next;
    updated.status   = LINE_RUNNING;
    updated.progress = 0;
    m_lineRepo.update(updated);

    result = { next.id, next.lineName, next.orderId, actualQty, totalTime };
    m_lastError.clear();
    return true;
}

// ── 생산 완료 ─────────────────────────────────────────────────────────────────

bool ProductionService::completeLine(int lineId)
{
    ProductionLine* line = m_lineRepo.findById(lineId);
    if (!line)
    {
        m_lastError = "생산 라인을 찾을 수 없습니다.";
        return false;
    }
    if (line->status != LINE_RUNNING)
    {
        m_lastError = "RUNNING 상태의 라인만 완료 처리할 수 있습니다. 현재 상태: " + line->status;
        return false;
    }

    Order* order = m_orderRepo.findById(line->orderId);
    if (!order)
    {
        m_lastError = "연결된 주문을 찾을 수 없습니다.";
        return false;
    }

    Sample* samplePtr = m_sampleRepo.findById(order->sampleId);
    if (!samplePtr)
    {
        m_lastError = "연결된 시료를 찾을 수 없습니다.";
        return false;
    }
    const Sample sample = *samplePtr;

    const int shortfall = std::max(0, order->quantity - sample.stock);
    const int actualQty = calcActualQty(shortfall, sample.yield);

    // 1. 생산 라인 완료 처리
    ProductionLine updatedLine = *line;
    updatedLine.status   = LINE_DONE;
    updatedLine.progress = 100;
    m_lineRepo.update(updatedLine);

    // 2. 재고 증가
    m_inventoryService.increase(sample.id, actualQty);

    // 3. 주문 CONFIRMED
    m_orderService.confirmOrder(order->id);

    m_lastError.clear();
    return true;
}

// ── 조회 ──────────────────────────────────────────────────────────────────────

std::vector<ProductionLine> ProductionService::getActiveLines() const
{
    std::vector<ProductionLine> result;
    for (const auto& pl : m_lineRepo.findAll())
    {
        if (pl.status == LINE_RUNNING || pl.status == "PAUSED")
            result.push_back(pl);
    }
    return result;
}

std::vector<ProductionLine> ProductionService::getQueuedLines() const
{
    std::vector<ProductionLine> result;
    for (const auto& pl : m_lineRepo.findAll())
    {
        if (pl.status == LINE_IDLE)
            result.push_back(pl);
    }
    std::sort(result.begin(), result.end(),
              [](const ProductionLine& a, const ProductionLine& b){ return a.id < b.id; });
    return result;
}

std::string ProductionService::getLastError() const { return m_lastError; }

// ── 다음 단계 구현 예정 ───────────────────────────────────────────────────────

bool ProductionService::updateProgress(int, int) { return false; }
bool ProductionService::pauseLine(int)           { return false; }
bool ProductionService::resumeLine(int)          { return false; }
