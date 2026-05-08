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
    case 1: handleViewStatus(); break;
    case 2: handleViewQueue();  break;
    case 3: handleStartQueue(); break;
    case 4: handleComplete();   break;
    default:
        m_view.showMessage("잘못된 입력입니다.");
        break;
    }
    ConsoleUtil::printSeparator();
}

void ProductionLineController::handleViewStatus()
{
    const auto lines = m_service.getActiveLines();
    if (lines.empty())
    {
        m_view.showMessage("진행 중인 생산 라인이 없습니다.");
        return;
    }
    m_view.showLines(lines);
}

void ProductionLineController::handleViewQueue()
{
    const auto queued = m_service.getQueuedLines();
    if (queued.empty())
    {
        m_view.showMessage("생산 대기(Queue) 항목이 없습니다.");
        return;
    }
    m_view.showLines(queued);
}

void ProductionLineController::handleStartQueue()
{
    const auto queued = m_service.getQueuedLines();
    if (queued.empty())
    {
        m_view.showMessage("생산 대기(Queue) 항목이 없습니다.");
        return;
    }
    m_view.showLines(queued);

    ProductionStartResult result{};
    if (!m_service.startNextInQueue(result))
    {
        m_view.showMessage("[오류] " + m_service.getLastError());
        return;
    }
    m_view.showProductionStarted(result);
}

void ProductionLineController::handleComplete()
{
    const auto running = m_service.getActiveLines();
    if (running.empty())
    {
        m_view.showMessage("완료 처리할 RUNNING 라인이 없습니다.");
        return;
    }
    m_view.showLines(running);

    const int lineId = m_view.promptLineId();
    if (!m_service.completeLine(lineId))
    {
        m_view.showMessage("[오류] " + m_service.getLastError());
        return;
    }
    m_view.showMessage("생산 완료 처리되었습니다. 재고가 증가하고 주문이 CONFIRMED 상태로 변경되었습니다.");
}
