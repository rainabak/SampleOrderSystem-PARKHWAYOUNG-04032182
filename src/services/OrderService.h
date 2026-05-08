#pragma once
#include <string>
#include <vector>
#include "../repositories/IOrderRepository.h"
#include "../repositories/IProductionLineRepository.h"
#include "../repositories/ISampleRepository.h"
#include "../models/Order.h"

class OrderService
{
public:
    OrderService(IOrderRepository&          orderRepo,
                 IProductionLineRepository& lineRepo,
                 ISampleRepository&         sampleRepo);

    bool               createOrder(int                sampleId,
                                   const std::string& customerName,
                                   int                quantity);
    bool               startProduction(int orderId, const std::string& lineName);
    bool               confirmOrder(int orderId);
    bool               cancelOrder(int orderId);

    std::vector<Order> findAll()                               const;
    std::vector<Order> getByStatus(const std::string& status)  const;

    std::string        getLastError()                          const;

private:
    IOrderRepository&          m_orderRepo;
    IProductionLineRepository& m_lineRepo;
    ISampleRepository&         m_sampleRepo;
    std::string                m_lastError;

    bool validateCustomerName(const std::string& name);
    bool validateQuantity(int quantity);
    bool isValidTransition(const std::string& from, const std::string& to) const;
};
