#pragma once
#include <string>

struct Shipment
{
    int         id          { 0 };
    int         orderId     { 0 };
    int         sampleId    { 0 };
    int         quantity    { 0 };
    std::string customerName;
    std::string shippedAt;
};
