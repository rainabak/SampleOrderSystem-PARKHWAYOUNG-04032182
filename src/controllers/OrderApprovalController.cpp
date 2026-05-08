#include "OrderApprovalController.h"
#include "../utils/ConsoleUtil.h"

OrderApprovalController::OrderApprovalController(OrderView& view, OrderService& service)
    : m_view(view), m_service(service)
{}

void OrderApprovalController::run()
{
    while (true)
    {
        ConsoleUtil::clearScreen();
        m_view.showApprovalMenu();
        const int choice = m_view.getMenuChoice();
        if (choice == 0) break;
        handleChoice(choice);
        ConsoleUtil::pause();
    }
}

void OrderApprovalController::handleChoice(int choice)
{
    ConsoleUtil::printSeparator();
    switch (choice)
    {
    case 1: handleApprove(); break;
    case 2: handleReject();  break;
    default:
        m_view.showMessage("잘못된 입력입니다.");
        break;
    }
    ConsoleUtil::printSeparator();
}

void OrderApprovalController::handleApprove()
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

void OrderApprovalController::handleReject()
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
