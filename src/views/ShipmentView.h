#pragma once
#include <string>
#include <vector>
#include "../models/Order.h"
#include "../models/Shipment.h"

class ShipmentView
{
public:
    void showMenu()                                              const;
    int  getMenuChoice()                                         const;
    void showMessage(const std::string& msg)                     const;

    int  promptOrderId()                                         const;
    void showOrders(const std::vector<Order>& orders)            const;
    void showShipments(const std::vector<Shipment>& shipments)   const;
    void showShipment(const Shipment& shipment)                  const;
};
