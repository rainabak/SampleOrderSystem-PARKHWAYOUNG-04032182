#pragma once
#include <vector>
#include "../repositories/IOrderRepository.h"
#include "../repositories/IShipmentRepository.h"
#include "../models/Shipment.h"
#include "InventoryService.h"

class ShipmentService
{
public:
    ShipmentService(IOrderRepository&    orderRepo,
                    IShipmentRepository& shipmentRepo,
                    InventoryService&    inventoryService);

    bool                  ship(int orderId);
    std::vector<Shipment> getHistory() const;

private:
    IOrderRepository&    m_orderRepo;
    IShipmentRepository& m_shipmentRepo;
    InventoryService&    m_inventoryService;
};
