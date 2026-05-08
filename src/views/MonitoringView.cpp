#include "MonitoringView.h"
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

static const std::string SEP   = "============================================\n";
static const std::string SEP_S = "--------------------------------------------\n";

static std::string toStatusLabel(StockStatus status)
{
    switch (status)
    {
    case StockStatus::SURPLUS:  return "여유";
    case StockStatus::SHORTAGE: return "부족";
    case StockStatus::DEPLETED: return "고갈";
    default:                    return "-";
    }
}

static void printHeader()
{
    std::cout << SEP << "         [Data Monitoring Tool]\n" << SEP;
}

static void printOrderRows(const OrderSummary& summary)
{
    std::cout << "\n주문 상태 요약\n"
              << "- RESERVED  : " << summary.countReserved  << "건\n"
              << "- PRODUCING : " << summary.countProducing << "건\n"
              << "- CONFIRMED : " << summary.countConfirmed << "건\n"
              << "- RELEASED  : " << summary.countRelease   << "건\n";
}

static void printSampleItems(const std::vector<SampleStatus>& statuses)
{
    if (statuses.empty()) { std::cout << "  등록된 시료가 없습니다.\n"; return; }
    for (const auto& s : statuses)
    {
        std::cout << "- " << s.name
                  << " / 재고 "  << s.stock
                  << " / 상태 "  << toStatusLabel(s.stockStatus) << "\n";
    }
}

static void printFooter()
{
    std::cout << "\n" << SEP << "  0. 뒤로가기\n";
}

void MonitoringView::showMenu() const
{
    std::cout << SEP
              << "              [ 모니터링 ]\n" << SEP
              << "  1. 전체 Dashboard\n"
              << "  2. 주문 상태별 집계\n"
              << "  3. 시료 재고 현황\n"
              << "  4. 생산 현황\n"
              << "  0. 돌아가기\n" << SEP
              << "선택: ";
}

int MonitoringView::getMenuChoice() const
{
    int choice;
    if (!(std::cin >> choice))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

void MonitoringView::showMessage(const std::string& msg) const
{
    std::cout << "\n" << msg << "\n";
}

void MonitoringView::showOrderSummary(const OrderSummary& summary) const
{
    printHeader();
    printOrderRows(summary);
    std::cout << SEP_S << "  합계      : " << summary.total << "건\n";
    printFooter();
}

void MonitoringView::showSampleList(const std::vector<SampleStatus>& statuses) const
{
    printHeader();
    std::cout << "\n시료별 재고 현황\n";
    printSampleItems(statuses);
    printFooter();
}

void MonitoringView::showDashboard(const OrderSummary&              summary,
                                   const std::vector<SampleStatus>& statuses) const
{
    printHeader();
    printOrderRows(summary);
    std::cout << "\n" << SEP_S << "시료별 재고 현황\n";
    printSampleItems(statuses);
    printFooter();
}

void MonitoringView::showProductionStatus(const std::vector<ProductionLine>& lines) const
{
    printHeader();
    std::cout << "\n생산 현황 (RUNNING / PAUSED)\n"
              << std::left
              << std::setw(6)  << "ID"
              << std::setw(10) << "주문ID"
              << std::setw(16) << "라인명"
              << std::setw(12) << "상태"
              << "진행률\n"
              << std::string(50, '-') << "\n";
    if (lines.empty())
    {
        std::cout << "  활성 생산 라인이 없습니다.\n";
    }
    else
    {
        for (const auto& l : lines)
        {
            std::cout << std::setw(6)  << l.id
                      << std::setw(10) << l.orderId
                      << std::setw(16) << l.lineName
                      << std::setw(12) << l.status
                      << l.progress << "%\n";
        }
    }
    printFooter();
}
