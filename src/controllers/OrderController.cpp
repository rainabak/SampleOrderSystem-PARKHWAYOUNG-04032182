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
    case 1: handleCreate();          break;
    case 2: handleList();            break;
    case 3: handleStartProduction(); break;
    case 4: handleCancel();          break;
    default:
        m_view.showMessage("잘못된 입력입니다.");
        break;
    }
    ConsoleUtil::printSeparator();
}

void OrderController::handleCreate()          {}
void OrderController::handleList()            {}
void OrderController::handleStartProduction() {}
void OrderController::handleCancel()          {}
