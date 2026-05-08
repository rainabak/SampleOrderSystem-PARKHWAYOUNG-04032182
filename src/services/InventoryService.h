#pragma once
#include <vector>
#include "../repositories/ISampleRepository.h"
#include "../models/Sample.h"

class InventoryService
{
public:
    explicit InventoryService(ISampleRepository& sampleRepo);

    bool                increase(int sampleId, int qty);
    bool                increaseWithYield(int sampleId, int rawQuantity);
    bool                decrease(int sampleId, int qty);
    bool                canFulfill(int sampleId, int qty) const;
    std::vector<Sample> getLowStockList(int threshold)    const;

private:
    ISampleRepository& m_sampleRepo;
};
