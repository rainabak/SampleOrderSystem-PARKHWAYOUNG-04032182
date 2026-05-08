#include "InventoryService.h"

InventoryService::InventoryService(ISampleRepository& sampleRepo)
    : m_sampleRepo(sampleRepo)
{}

bool InventoryService::increase(int sampleId, int qty)
{
    Sample* s = m_sampleRepo.findById(sampleId);
    if (!s) return false;
    Sample updated = *s;
    updated.stock += qty;
    return m_sampleRepo.update(updated);
}

bool InventoryService::increaseWithYield(int, int) { return false; }

bool InventoryService::decrease(int sampleId, int qty)
{
    Sample* s = m_sampleRepo.findById(sampleId);
    if (!s) return false;
    if (s->stock < qty) return false;
    Sample updated = *s;
    updated.stock -= qty;
    return m_sampleRepo.update(updated);
}

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
