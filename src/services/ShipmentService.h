#pragma once
#include <string>
#include <vector>
#include "../repositories/IOrderRepository.h"
#include "../repositories/IShipmentRepository.h"
#include "../models/Order.h"
#include "../models/Shipment.h"
#include "InventoryService.h"

class ShipmentService
{
public:
    ShipmentService(IOrderRepository&    orderRepo,
                    IShipmentRepository& shipmentRepo,
                    InventoryService&    inventoryService);

    bool                  ship(int orderId);
    std::vector<Shipment> getHistory()         const;
    std::vector<Order>    getConfirmedOrders() const;
    std::string           getLastError()       const;

private:
    IOrderRepository&    m_orderRepo;
    IShipmentRepository& m_shipmentRepo;
    InventoryService&    m_inventoryService;
    std::string          m_lastError;
};
