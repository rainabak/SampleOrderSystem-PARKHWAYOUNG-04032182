#include "OrderService.h"

OrderService::OrderService(IOrderRepository& orderRepo,
                           IProductionLineRepository& lineRepo)
    : m_orderRepo(orderRepo)
    , m_lineRepo(lineRepo)
{}

Order OrderService::createOrder(int, const std::string&,
                                const std::string&, int)
{
    return {};
}

bool OrderService::startProduction(int, const std::string&) { return false; }
bool OrderService::confirmOrder(int)                        { return false; }
bool OrderService::cancelOrder(int)                         { return false; }

std::vector<Order> OrderService::getByStatus(const std::string&) const
{
    return {};
}

bool OrderService::isValidTransition(const std::string&, const std::string&) const
{
    return false;
}
