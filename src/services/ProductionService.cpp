#include "ProductionService.h"

ProductionService::ProductionService(IProductionLineRepository& lineRepo,
                                     IOrderRepository&          orderRepo,
                                     InventoryService&          inventoryService,
                                     OrderService&              orderService)
    : m_lineRepo(lineRepo)
    , m_orderRepo(orderRepo)
    , m_inventoryService(inventoryService)
    , m_orderService(orderService)
{}

bool ProductionService::updateProgress(int, int) { return false; }
bool ProductionService::completeLine(int)        { return false; }
bool ProductionService::pauseLine(int)           { return false; }
bool ProductionService::resumeLine(int)          { return false; }

std::vector<ProductionLine> ProductionService::getActiveLines() const
{
    return {};
}
