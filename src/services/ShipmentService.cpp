#include "ShipmentService.h"

ShipmentService::ShipmentService(IOrderRepository&    orderRepo,
                                 IShipmentRepository& shipmentRepo,
                                 InventoryService&    inventoryService)
    : m_orderRepo(orderRepo)
    , m_shipmentRepo(shipmentRepo)
    , m_inventoryService(inventoryService)
{}

bool ShipmentService::ship(int) { return false; }

std::vector<Shipment> ShipmentService::getHistory() const
{
    return {};
}
