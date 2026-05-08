#pragma once
#include <string>
#include <vector>
#include "../repositories/IOrderRepository.h"
#include "../repositories/IProductionLineRepository.h"
#include "../models/Order.h"

class OrderService
{
public:
    OrderService(IOrderRepository& orderRepo, IProductionLineRepository& lineRepo);

    Order              createOrder(int sampleId, const std::string& productName,
                                   const std::string& customerName, int quantity);
    bool               startProduction(int orderId, const std::string& lineName);
    bool               confirmOrder(int orderId);
    bool               cancelOrder(int orderId);
    std::vector<Order> getByStatus(const std::string& status) const;

private:
    IOrderRepository&          m_orderRepo;
    IProductionLineRepository& m_lineRepo;

    bool isValidTransition(const std::string& from, const std::string& to) const;
};
