#pragma once
#include <vector>
#include "../repositories/IProductionLineRepository.h"
#include "../repositories/IOrderRepository.h"
#include "../models/ProductionLine.h"
#include "InventoryService.h"
#include "OrderService.h"

class ProductionService
{
public:
    ProductionService(IProductionLineRepository& lineRepo,
                      IOrderRepository&          orderRepo,
                      InventoryService&          inventoryService,
                      OrderService&              orderService);

    bool                        updateProgress(int lineId, int progress);
    bool                        completeLine(int lineId);
    bool                        pauseLine(int lineId);
    bool                        resumeLine(int lineId);
    std::vector<ProductionLine> getActiveLines() const;

private:
    IProductionLineRepository& m_lineRepo;
    IOrderRepository&          m_orderRepo;
    InventoryService&          m_inventoryService;
    OrderService&              m_orderService;
};
