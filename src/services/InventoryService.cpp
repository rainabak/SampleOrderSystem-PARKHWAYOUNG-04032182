#include "InventoryService.h"

InventoryService::InventoryService(ISampleRepository& sampleRepo)
    : m_sampleRepo(sampleRepo)
{}

bool InventoryService::increase(int, int)             { return false; }
bool InventoryService::increaseWithYield(int, int)    { return false; }
bool InventoryService::decrease(int, int)             { return false; }
bool InventoryService::canFulfill(int, int) const     { return false; }

std::vector<Sample> InventoryService::getLowStockList(int) const
{
    return {};
}
