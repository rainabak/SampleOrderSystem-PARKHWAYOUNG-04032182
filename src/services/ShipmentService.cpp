#include "ShipmentService.h"
#include <ctime>

static const std::string STATUS_CONFIRMED = "CONFIRMED";
static const std::string STATUS_RELEASED  = "RELEASED";

static std::string currentDate()
{
    time_t t = std::time(nullptr);
    struct tm tm_buf;
    localtime_s(&tm_buf, &t);
    char buf[11];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d", &tm_buf);
    return buf;
}

ShipmentService::ShipmentService(IOrderRepository&    orderRepo,
                                 IShipmentRepository& shipmentRepo,
                                 InventoryService&    inventoryService)
    : m_orderRepo(orderRepo)
    , m_shipmentRepo(shipmentRepo)
    , m_inventoryService(inventoryService)
{}

// ── 출고 처리 ─────────────────────────────────────────────────────────────────

bool ShipmentService::ship(int orderId)
{
    Order* order = m_orderRepo.findById(orderId);
    if (!order)
    {
        m_lastError = "해당 ID의 주문을 찾을 수 없습니다.";
        return false;
    }
    if (order->status != STATUS_CONFIRMED)
    {
        m_lastError = "CONFIRMED 상태의 주문만 출고할 수 있습니다. 현재 상태: " + order->status;
        return false;
    }
    if (!m_inventoryService.canFulfill(order->sampleId, order->quantity))
    {
        m_lastError = "재고가 부족하여 출고할 수 없습니다.";
        return false;
    }

    // 재고 차감
    if (!m_inventoryService.decrease(order->sampleId, order->quantity))
    {
        m_lastError = "재고 차감에 실패했습니다.";
        return false;
    }

    // 주문 상태 RELEASED
    Order updated  = *order;
    updated.status = STATUS_RELEASED;
    m_orderRepo.update(updated);

    // 출고 레코드 생성
    Shipment s;
    s.orderId      = orderId;
    s.sampleId     = order->sampleId;
    s.quantity     = order->quantity;
    s.customerName = order->customerName;
    s.shippedAt    = currentDate();
    m_shipmentRepo.add(s);

    m_lastError.clear();
    return true;
}

// ── 조회 ──────────────────────────────────────────────────────────────────────

std::vector<Shipment> ShipmentService::getHistory() const
{
    return m_shipmentRepo.findAll();
}

std::vector<Order> ShipmentService::getConfirmedOrders() const
{
    std::vector<Order> result;
    for (const auto& o : m_orderRepo.findAll())
    {
        if (o.status == STATUS_CONFIRMED)
            result.push_back(o);
    }
    return result;
}

std::string ShipmentService::getLastError() const
{
    return m_lastError;
}
