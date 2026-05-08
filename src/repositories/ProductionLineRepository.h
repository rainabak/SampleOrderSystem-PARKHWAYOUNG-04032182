#pragma once
#include <vector>
#include "IProductionLineRepository.h"
#include "../persistence/JsonFileStorage.h"

class ProductionLineRepository : public IProductionLineRepository
{
public:
    explicit ProductionLineRepository(JsonFileStorage& storage);

    void                        add(const ProductionLine& line)    override;
    std::vector<ProductionLine> findAll()                    const override;
    ProductionLine*             findById(int id)                   override;
    ProductionLine*             findByOrderId(int orderId)         override;
    bool                        update(const ProductionLine& line) override;
    bool                        remove(int id)                     override;

private:
    JsonFileStorage&            m_storage;
    std::vector<ProductionLine> m_lines;
    int                         m_nextId{ 1 };

    void load();
    void persist() const;
};
