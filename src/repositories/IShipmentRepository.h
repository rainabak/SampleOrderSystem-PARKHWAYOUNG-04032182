#pragma once
#include <vector>
#include "../models/Shipment.h"

class IShipmentRepository
{
public:
    virtual ~IShipmentRepository() = default;

    virtual void                  add(const Shipment& shipment) = 0;
    virtual std::vector<Shipment> findAll()               const = 0;
    virtual Shipment*             findById(int id)               = 0;
};
