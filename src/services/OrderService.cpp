#include "OrderService.h"
#include <string>

static const std::string STATUS_RESERVED = "RESERVED";

OrderService::OrderService(IOrderRepository&          orderRepo,
                           IProductionLineRepository& lineRepo,
                           ISampleRepository&         sampleRepo)
    : m_orderRepo(orderRepo)
    , m_lineRepo(lineRepo)
    , m_sampleRepo(sampleRepo)
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

std::string OrderService::getLastError() const
{
    return m_lastError;
}

// ── 다음 단계 구현 예정 ───────────────────────────────────────────────────────

bool OrderService::startProduction(int, const std::string&) { return false; }
bool OrderService::confirmOrder(int)                        { return false; }
bool OrderService::cancelOrder(int)                         { return false; }

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
