#include "SampleController.h"
#include "../utils/ConsoleUtil.h"

SampleController::SampleController(SampleView& view, SampleService& service)
    : m_view(view), m_service(service)
{}

void SampleController::run()
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

void SampleController::handleChoice(int choice)
{
    ConsoleUtil::printSeparator();
    switch (choice)
    {
    case 1: handleCreate(); break;
    case 2: handleList();   break;
    case 3: handleSearch(); break;
    case 4: handleUpdate(); break;
    case 5: handleDelete(); break;
    default:
        m_view.showMessage("잘못된 입력입니다.");
        break;
    }
    ConsoleUtil::printSeparator();
}

void SampleController::handleCreate()
{
    const std::string name        = m_view.promptName();
    const std::string description = m_view.promptDescription();
    const int         avgProdTime = m_view.promptAvgProductionTime();
    const double      yield       = m_view.promptYield();
    const int         stock       = m_view.promptStock();

    if (m_service.createSample(name, description, avgProdTime, yield, stock))
        m_view.showMessage("시료가 등록되었습니다.");
    else
        m_view.showMessage("[오류] " + m_service.getLastError());
}

void SampleController::handleList()
{
    const auto samples = m_service.findAll();
    if (samples.empty())
    {
        m_view.showMessage("등록된 시료가 없습니다.");
        return;
    }
    m_view.showSamples(samples);
}

void SampleController::handleSearch()
{
    const std::string keyword = m_view.promptSearchKeyword();
    if (keyword.empty())
    {
        m_view.showMessage("검색어를 입력하세요.");
        return;
    }

    const auto result = m_service.searchByName(keyword);
    if (result.empty())
        m_view.showMessage("검색 결과가 없습니다: " + keyword);
    else
        m_view.showSamples(result);
}

void SampleController::handleUpdate()
{
    const auto samples = m_service.findAll();
    if (samples.empty())
    {
        m_view.showMessage("수정할 시료가 없습니다.");
        return;
    }
    m_view.showSamples(samples);

    const int id      = m_view.promptId();
    Sample*   found   = m_service.findById(id);
    if (!found)
    {
        m_view.showMessage("해당 ID의 시료를 찾을 수 없습니다.");
        return;
    }

    m_view.showSample(*found);

    Sample updated;
    updated.id                = found->id;
    updated.name              = m_view.promptName();
    updated.description       = m_view.promptDescription();
    updated.avgProductionTime = m_view.promptAvgProductionTime();
    updated.yield             = m_view.promptYield();
    updated.stock             = m_view.promptStock();

    if (m_service.updateSample(updated))
        m_view.showMessage("시료가 수정되었습니다.");
    else
        m_view.showMessage("[오류] " + m_service.getLastError());
}

void SampleController::handleDelete()
{
    const auto samples = m_service.findAll();
    if (samples.empty())
    {
        m_view.showMessage("삭제할 시료가 없습니다.");
        return;
    }
    m_view.showSamples(samples);

    const int id = m_view.promptId();
    if (m_service.removeSample(id))
        m_view.showMessage("시료가 삭제되었습니다.");
    else
        m_view.showMessage("[오류] " + m_service.getLastError());
}
