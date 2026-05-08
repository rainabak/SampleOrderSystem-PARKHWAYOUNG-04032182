#pragma once
#include "IController.h"
#include "../views/MainMenuView.h"

class MainController : public IController
{
public:
    MainController(MainMenuView&  view,
                   IController&   sample,
                   IController&   orderReceive,
                   IController&   orderApproval,
                   IController&   productionLine,
                   IController&   monitoring,
                   IController&   shipment);
    void run() override;

private:
    MainMenuView& m_view;
    IController&  m_sample;
    IController&  m_orderReceive;
    IController&  m_orderApproval;
    IController&  m_productionLine;
    IController&  m_monitoring;
    IController&  m_shipment;

    void handleChoice(int choice);
};
