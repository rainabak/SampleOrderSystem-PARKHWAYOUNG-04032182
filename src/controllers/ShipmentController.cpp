#include "ShipmentController.h"
#include "../utils/ConsoleUtil.h"

ShipmentController::ShipmentController(ShipmentView& view, ShipmentService& service)
    : m_view(view), m_service(service)
{}

void ShipmentController::run()
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

void ShipmentController::handleChoice(int choice)
{
    ConsoleUtil::printSeparator();
    switch (choice)
    {
    case 1: handleShip();    break;
    case 2: handleHistory(); break;
    default:
        m_view.showMessage("잘못된 입력입니다.");
        break;
    }
    ConsoleUtil::printSeparator();
}

void ShipmentController::handleShip()
{
    const auto confirmed = m_service.getConfirmedOrders();
    if (confirmed.empty())
    {
        m_view.showMessage("출고 가능한 주문(CONFIRMED)이 없습니다.");
        return;
    }
    m_view.showOrders(confirmed);

    const int id = m_view.promptOrderId();
    if (m_service.ship(id))
        m_view.showMessage("출고 처리 완료되었습니다. (상태: RELEASED)");
    else
        m_view.showMessage("[오류] " + m_service.getLastError());
}

void ShipmentController::handleHistory()
{
    const auto history = m_service.getHistory();
    if (history.empty())
    {
        m_view.showMessage("출고 이력이 없습니다.");
        return;
    }
    m_view.showShipments(history);
}
