#pragma once
#include <vector>
#include "../repositories/IProductionLineRepository.h"
#include "../repositories/IOrderRepository.h"
#include "../repositories/ISampleRepository.h"
#include "../models/ProductionLine.h"
#include "InventoryService.h"
#include "OrderService.h"

struct ProductionStartResult
{
    int         lineId;
    std::string lineName;
    int         orderId;
    int         actualQty;
    int         totalTimeMins;
};

class ProductionService
{
public:
    ProductionService(IProductionLineRepository& lineRepo,
                      IOrderRepository&          orderRepo,
                      ISampleRepository&         sampleRepo,
                      InventoryService&          inventoryService,
                      OrderService&              orderService);

    bool                        startNextInQueue(ProductionStartResult& result);
    bool                        completeLine(int lineId);
    bool                        updateProgress(int lineId, int progress);
    bool                        pauseLine(int lineId);
    bool                        resumeLine(int lineId);

    std::vector<ProductionLine> getActiveLines()  const;
    std::vector<ProductionLine> getQueuedLines()  const;

    std::string                 getLastError()    const;

private:
    IProductionLineRepository& m_lineRepo;
    IOrderRepository&          m_orderRepo;
    ISampleRepository&         m_sampleRepo;
    InventoryService&          m_inventoryService;
    OrderService&              m_orderService;
    std::string                m_lastError;
};
