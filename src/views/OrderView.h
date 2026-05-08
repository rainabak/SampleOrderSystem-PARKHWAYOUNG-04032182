#pragma once
#include <string>
#include <vector>
#include "../models/Order.h"

class OrderView
{
public:
    void        showMenu()                                const;
    int         getMenuChoice()                           const;
    void        showMessage(const std::string& msg)       const;

    int         promptSampleId()                          const;
    std::string promptCustomerName()                      const;
    int         promptQuantity()                          const;
    int         promptOrderId()                           const;
    std::string promptLineName()                          const;
    void        showOrders(const std::vector<Order>&)     const;
    void        showOrder(const Order&)                   const;
};
