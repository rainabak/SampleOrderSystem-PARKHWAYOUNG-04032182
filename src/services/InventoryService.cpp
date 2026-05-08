#include "InventoryService.h"

InventoryService::InventoryService(ISampleRepository& sampleRepo)
    : m_sampleRepo(sampleRepo)
{}

bool InventoryService::increase(int, int)          { return false; }
bool InventoryService::increaseWithYield(int, int) { return false; }
bool InventoryService::decrease(int, int)          { return false; }

bool InventoryService::canFulfill(int sampleId, int qty) const
{
    for (const auto& s : m_sampleRepo.findAll())
    {
        if (s.id == sampleId)
            return s.stock >= qty;
    }
    return false;
}

std::vector<Sample> InventoryService::getLowStockList(int) const
{
    return {};
}
