#pragma once
#include <vector>
#include "../models/OrderSummary.h"
#include "../models/SampleStatus.h"
#include "../models/ProductionLine.h"
#include "../repositories/IOrderRepository.h"
#include "../repositories/ISampleRepository.h"
#include "../repositories/IProductionLineRepository.h"

class MonitoringService
{
public:
    MonitoringService(IOrderRepository&          orderRepo,
                      ISampleRepository&         sampleRepo,
                      IProductionLineRepository& lineRepo);

    OrderSummary                getOrderSummary()   const;
    std::vector<SampleStatus>   getSampleStatuses() const;
    std::vector<ProductionLine> getActiveLines()    const;

private:
    IOrderRepository&          m_orderRepo;
    ISampleRepository&         m_sampleRepo;
    IProductionLineRepository& m_lineRepo;
};
