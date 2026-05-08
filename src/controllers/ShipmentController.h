#pragma once
#include "IController.h"
#include "../views/ShipmentView.h"
#include "../services/ShipmentService.h"

class ShipmentController : public IController
{
public:
    ShipmentController(ShipmentView& view, ShipmentService& service);
    void run() override;

private:
    ShipmentView&    m_view;
    ShipmentService& m_service;

    void handleChoice(int choice);
    void handleShip();
    void handleHistory();
};
