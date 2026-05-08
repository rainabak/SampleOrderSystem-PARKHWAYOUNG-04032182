#include "OrderController.h"
#include "../utils/ConsoleUtil.h"

OrderController::OrderController(OrderView& view, OrderService& service)
    : m_view(view), m_service(service)
{}

void OrderController::run()
{
    while (true)
    {
        ConsoleUtil::clearScreen();
        m_view.showMenu();
        const int choice = m_view.getMenuChoice();
        if (choice == 0) break;
        handleChoice(choice);
        ConsoleUtil::pause();
    }
}

void OrderController::handleChoice(int choice)
{
    ConsoleUtil::printSeparator();
    switch (choice)
    {
    case 1: handleCreate(); break;
    case 2: handleList();   break;
    default:
        m_view.showMessage("잘못된 입력입니다.");
        break;
    }
    ConsoleUtil::printSeparator();
}

void OrderController::handleCreate()
{
    const int         sampleId     = m_view.promptSampleId();
    const std::string customerName = m_view.promptCustomerName();
    const int         quantity     = m_view.promptQuantity();

    if (m_service.createOrder(sampleId, customerName, quantity))
        m_view.showMessage("주문이 등록되었습니다. (상태: RESERVED)");
    else
        m_view.showMessage("[오류] " + m_service.getLastError());
}

void OrderController::handleList()
{
    const auto orders = m_service.findAll();
    if (orders.empty())
    {
        m_view.showMessage("등록된 주문이 없습니다.");
        return;
    }
    m_view.showOrders(orders);
}
