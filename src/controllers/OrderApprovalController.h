#pragma once
#include "IController.h"
#include "../views/OrderView.h"
#include "../services/OrderService.h"

class OrderApprovalController : public IController
{
public:
    OrderApprovalController(OrderView& view, OrderService& service);
    void run() override;

private:
    OrderView&    m_view;
    OrderService& m_service;

    void handleChoice(int choice);
    void handleApprove();
    void handleReject();
};
