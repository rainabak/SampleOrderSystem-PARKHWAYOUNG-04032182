#pragma once
#include <string>
#include <vector>
#include "../models/Shipment.h"

class ShipmentView
{
public:
    void showMenu()                                              const;
    int  getMenuChoice()                                         const;
    void showMessage(const std::string& msg)                     const;

    int  promptOrderId()                                         const;
    void showShipments(const std::vector<Shipment>& shipments)   const;
    void showShipment(const Shipment& shipment)                  const;
};
