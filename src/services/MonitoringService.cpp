#include "MonitoringService.h"

static const std::string STATUS_RESERVED  = "RESERVED";
static const std::string STATUS_PRODUCING = "PRODUCING";
static const std::string STATUS_CONFIRMED = "CONFIRMED";
static const std::string STATUS_RELEASED  = "RELEASED";

static const std::string LINE_RUNNING = "RUNNING";
static const std::string LINE_PAUSED  = "PAUSED";

static StockStatus calcStockStatus(int stock, int demand)
{
    if (stock == 0)     return StockStatus::DEPLETED;
    if (stock > demand) return StockStatus::SURPLUS;
    return StockStatus::SHORTAGE;
}

MonitoringService::MonitoringService(IOrderRepository&          orderRepo,
                                     ISampleRepository&         sampleRepo,
                                     IProductionLineRepository& lineRepo)
    : m_orderRepo(orderRepo)
    , m_sampleRepo(sampleRepo)
    , m_lineRepo(lineRepo)
{}

OrderSummary MonitoringService::getOrderSummary() const
{
    OrderSummary summary;
    for (const auto& order : m_orderRepo.findAll())
    {
        if      (order.status == STATUS_RESERVED)  { ++summary.countReserved;  ++summary.total; }
        else if (order.status == STATUS_PRODUCING) { ++summary.countProducing; ++summary.total; }
        else if (order.status == STATUS_CONFIRMED) { ++summary.countConfirmed; ++summary.total; }
        else if (order.status == STATUS_RELEASED)  { ++summary.countRelease;   ++summary.total; }
    }
    return summary;
}

std::vector<SampleStatus> MonitoringService::getSampleStatuses() const
{
    const auto orders  = m_orderRepo.findAll();
    const auto samples = m_sampleRepo.findAll();

    std::vector<SampleStatus> result;
    result.reserve(samples.size());

    for (const auto& sample : samples)
    {
        int demand = 0;
        for (const auto& order : orders)
        {
            if (order.sampleId == sample.id &&
                (order.status == STATUS_RESERVED || order.status == STATUS_PRODUCING))
            {
                demand += order.quantity;
            }
        }
        result.push_back({ sample.id, sample.name, sample.description,
                           sample.stock, demand,
                           calcStockStatus(sample.stock, demand) });
    }
    return result;
}

std::vector<ProductionLine> MonitoringService::getActiveLines() const
{
    std::vector<ProductionLine> result;
    for (const auto& line : m_lineRepo.findAll())
    {
        if (line.status == LINE_RUNNING || line.status == LINE_PAUSED)
            result.push_back(line);
    }
    return result;
}
