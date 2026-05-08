#pragma once
#include <vector>
#include "IShipmentRepository.h"
#include "../persistence/JsonFileStorage.h"

class ShipmentRepository : public IShipmentRepository
{
public:
    explicit ShipmentRepository(JsonFileStorage& storage);

    void                  add(const Shipment& shipment) override;
    std::vector<Shipment> findAll()               const override;
    Shipment*             findById(int id)               override;

private:
    JsonFileStorage&      m_storage;
    std::vector<Shipment> m_shipments;
    int                   m_nextId{ 1 };

    void load();
    void persist() const;
};
