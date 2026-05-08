#pragma once
#include "IController.h"
#include "../views/OrderView.h"
#include "../services/OrderService.h"

class OrderController : public IController
{
public:
    OrderController(OrderView& view, OrderService& service);
    void run() override;

private:
    OrderView&    m_view;
    OrderService& m_service;

    void handleChoice(int choice);
    void handleCreate();
    void handleList();
    void handleApprove();
    void handleReject();
};
