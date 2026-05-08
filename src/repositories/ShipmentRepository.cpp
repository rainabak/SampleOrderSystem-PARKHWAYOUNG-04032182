#include "ShipmentRepository.h"

ShipmentRepository::ShipmentRepository(JsonFileStorage& storage)
    : m_storage(storage)
{
    load();
}

void ShipmentRepository::add(const Shipment&) {}

std::vector<Shipment> ShipmentRepository::findAll() const
{
    return m_shipments;
}

Shipment* ShipmentRepository::findById(int) { return nullptr; }

void ShipmentRepository::load()        {}
void ShipmentRepository::persist() const {}
