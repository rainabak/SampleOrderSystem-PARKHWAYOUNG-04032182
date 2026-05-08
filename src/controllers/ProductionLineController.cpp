#include "ProductionLineController.h"
#include "../utils/ConsoleUtil.h"

ProductionLineController::ProductionLineController(ProductionLineView& view,
                                                   ProductionService&  service)
    : m_view(view), m_service(service)
{}

void ProductionLineController::run()
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

void ProductionLineController::handleChoice(int choice)
{
    ConsoleUtil::printSeparator();
    switch (choice)
    {
    case 1: handleList();           break;
    case 2: handleUpdateProgress(); break;
    case 3: handleComplete();       break;
    case 4: handlePause();          break;
    case 5: handleResume();         break;
    default:
        m_view.showMessage("잘못된 입력입니다.");
        break;
    }
    ConsoleUtil::printSeparator();
}

void ProductionLineController::handleList()           {}
void ProductionLineController::handleUpdateProgress() {}
void ProductionLineController::handleComplete()       {}
void ProductionLineController::handlePause()          {}
void ProductionLineController::handleResume()         {}
