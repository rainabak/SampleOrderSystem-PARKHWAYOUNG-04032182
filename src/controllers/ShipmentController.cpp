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

void ShipmentController::handleShip()    {}
void ShipmentController::handleHistory() {}
