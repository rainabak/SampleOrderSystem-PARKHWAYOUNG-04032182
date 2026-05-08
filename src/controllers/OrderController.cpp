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
    case 1: handleCreate();  break;
    case 2: handleList();    break;
    case 3: handleApprove(); break;
    case 4: handleReject();  break;
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

void OrderController::handleApprove()
{
    const auto reserved = m_service.getByStatus("RESERVED");
    if (reserved.empty())
    {
        m_view.showMessage("승인 대기 중인 주문(RESERVED)이 없습니다.");
        return;
    }
    m_view.showOrders(reserved);

    const int id = m_view.promptOrderId();
    if (!m_service.approveOrder(id))
    {
        m_view.showMessage("[오류] " + m_service.getLastError());
        return;
    }

    const Order* updated = m_service.findById(id);
    if (updated && updated->status == "CONFIRMED")
        m_view.showMessage("승인 완료 — 재고 충분 (상태: CONFIRMED)");
    else if (updated && updated->status == "PRODUCING")
        m_view.showMessage("승인 완료 — 재고 부족, 생산 큐 등록 (상태: PRODUCING)");
    else
        m_view.showMessage("승인 처리되었습니다.");
}

void OrderController::handleReject()
{
    const auto reserved = m_service.getByStatus("RESERVED");
    if (reserved.empty())
    {
        m_view.showMessage("거절 대상 주문(RESERVED)이 없습니다.");
        return;
    }
    m_view.showOrders(reserved);

    const int id = m_view.promptOrderId();
    if (m_service.cancelOrder(id))
        m_view.showMessage("주문이 거절되었습니다. (상태: REJECTED)");
    else
        m_view.showMessage("[오류] " + m_service.getLastError());
}
