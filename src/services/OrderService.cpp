#include "OrderService.h"
#include "../models/ProductionLine.h"
#include <string>

static const std::string STATUS_RESERVED  = "RESERVED";
static const std::string STATUS_PRODUCING = "PRODUCING";
static const std::string STATUS_CONFIRMED = "CONFIRMED";
static const std::string STATUS_REJECTED  = "REJECTED";

OrderService::OrderService(IOrderRepository&          orderRepo,
                           IProductionLineRepository& lineRepo,
                           ISampleRepository&         sampleRepo,
                           InventoryService&          inventoryService)
    : m_orderRepo(orderRepo)
    , m_lineRepo(lineRepo)
    , m_sampleRepo(sampleRepo)
    , m_inventoryService(inventoryService)
{}

// ── 주문 접수 ─────────────────────────────────────────────────────────────────

bool OrderService::createOrder(int                sampleId,
                                const std::string& customerName,
                                int                quantity)
{
    if (!validateCustomerName(customerName)) return false;
    if (!validateQuantity(quantity))         return false;

    const Sample* sample = m_sampleRepo.findById(sampleId);
    if (!sample)
    {
        m_lastError = "존재하지 않는 시료 ID입니다: " + std::to_string(sampleId);
        return false;
    }

    Order o;
    o.sampleId     = sampleId;
    o.productName  = sample->name;
    o.customerName = customerName;
    o.quantity     = quantity;
    o.status       = STATUS_RESERVED;
    m_orderRepo.add(o);

    m_lastError.clear();
    return true;
}

// ── 주문 승인 ─────────────────────────────────────────────────────────────────

bool OrderService::approveOrder(int orderId)
{
    Order* order = m_orderRepo.findById(orderId);
    if (!order)
    {
        m_lastError = "해당 ID의 주문을 찾을 수 없습니다.";
        return false;
    }
    if (order->status != STATUS_RESERVED)
    {
        m_lastError = "RESERVED 상태의 주문만 승인할 수 있습니다. 현재 상태: " + order->status;
        return false;
    }

    Order updated = *order;

    if (m_inventoryService.canFulfill(order->sampleId, order->quantity))
    {
        // 재고 충분 → CONFIRMED
        updated.status = STATUS_CONFIRMED;
        m_orderRepo.update(updated);
    }
    else
    {
        // 재고 부족 → PRODUCING + 생산 큐 등록
        updated.status = STATUS_PRODUCING;
        m_orderRepo.update(updated);

        ProductionLine line;
        line.orderId  = orderId;
        line.lineName = "LINE-" + std::to_string(orderId);
        line.status   = "IDLE";
        line.progress = 0;
        m_lineRepo.add(line);
    }

    m_lastError.clear();
    return true;
}

// ── 주문 거절 ─────────────────────────────────────────────────────────────────

bool OrderService::cancelOrder(int orderId)
{
    Order* order = m_orderRepo.findById(orderId);
    if (!order)
    {
        m_lastError = "해당 ID의 주문을 찾을 수 없습니다.";
        return false;
    }
    if (order->status != STATUS_RESERVED)
    {
        m_lastError = "RESERVED 상태의 주문만 거절할 수 있습니다. 현재 상태: " + order->status;
        return false;
    }

    Order updated = *order;
    updated.status = STATUS_REJECTED;
    m_orderRepo.update(updated);

    m_lastError.clear();
    return true;
}

// ── 조회 ──────────────────────────────────────────────────────────────────────

std::vector<Order> OrderService::findAll() const
{
    return m_orderRepo.findAll();
}

std::vector<Order> OrderService::getByStatus(const std::string& status) const
{
    std::vector<Order> result;
    for (const auto& o : m_orderRepo.findAll())
    {
        if (o.status == status)
            result.push_back(o);
    }
    return result;
}

Order* OrderService::findById(int id)
{
    return m_orderRepo.findById(id);
}

std::string OrderService::getLastError() const
{
    return m_lastError;
}

// ── 다음 단계 구현 예정 ───────────────────────────────────────────────────────

bool OrderService::startProduction(int, const std::string&) { return false; }
bool OrderService::confirmOrder(int)                        { return false; }

// ── private ───────────────────────────────────────────────────────────────────

bool OrderService::validateCustomerName(const std::string& name)
{
    if (name.empty())
    {
        m_lastError = "고객사명은 비워둘 수 없습니다.";
        return false;
    }
    if (name.size() > 50)
    {
        m_lastError = "고객사명은 50자를 초과할 수 없습니다.";
        return false;
    }
    return true;
}

bool OrderService::validateQuantity(int quantity)
{
    if (quantity <= 0)
    {
        m_lastError = "주문 수량은 1개 이상이어야 합니다.";
        return false;
    }
    return true;
}

bool OrderService::isValidTransition(const std::string&, const std::string&) const
{
    return false;
}
