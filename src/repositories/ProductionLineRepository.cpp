#include "ProductionLineRepository.h"

ProductionLineRepository::ProductionLineRepository(JsonFileStorage& storage)
    : m_storage(storage)
{
    load();
}

void ProductionLineRepository::add(const ProductionLine&) {}

std::vector<ProductionLine> ProductionLineRepository::findAll() const
{
    return m_lines;
}

ProductionLine* ProductionLineRepository::findById(int)      { return nullptr; }
ProductionLine* ProductionLineRepository::findByOrderId(int) { return nullptr; }

bool ProductionLineRepository::update(const ProductionLine&) { return false; }
bool ProductionLineRepository::remove(int)                   { return false; }

void ProductionLineRepository::load()        {}
void ProductionLineRepository::persist() const {}
