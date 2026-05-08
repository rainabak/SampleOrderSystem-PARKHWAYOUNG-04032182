#pragma once
#include <vector>
#include "../models/ProductionLine.h"

class IProductionLineRepository
{
public:
    virtual ~IProductionLineRepository() = default;

    virtual void                        add(const ProductionLine& line)    = 0;
    virtual std::vector<ProductionLine> findAll()                    const = 0;
    virtual ProductionLine*             findById(int id)                   = 0;
    virtual ProductionLine*             findByOrderId(int orderId)         = 0;
    virtual bool                        update(const ProductionLine& line) = 0;
    virtual bool                        remove(int id)                     = 0;
};
