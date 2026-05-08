#pragma once
#include "IController.h"
#include "../views/ProductionLineView.h"
#include "../services/ProductionService.h"

class ProductionLineController : public IController
{
public:
    ProductionLineController(ProductionLineView& view, ProductionService& service);
    void run() override;

private:
    ProductionLineView& m_view;
    ProductionService&  m_service;

    void handleChoice(int choice);
    void handleViewStatus();
    void handleViewQueue();
    void handleStartQueue();
    void handleComplete();
};
