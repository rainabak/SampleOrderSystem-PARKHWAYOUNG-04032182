#include "MonitoringView.h"
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

static const std::string SEP   = "============================================\n";
static const std::string SEP_S = "--------------------------------------------\n";

static std::string toStatusLabel(StockStatus s)
{
    switch (s)
    {
    case StockStatus::SURPLUS:  return "[여유]";
    case StockStatus::SHORTAGE: return "[부족]";
    case StockStatus::DEPLETED: return "[고갈]";
    default:                    return "[ - ]";
    }
}

// ── 공통 출력 헬퍼 ────────────────────────────────────────────────────────────

static void printSectionHeader(const std::string& title)
{
    std::cout << "\n" << SEP_S << "  " << title << "\n" << SEP_S;
}

static void printOrderRows(const OrderSummary& s)
{
    std::cout << "  RESERVED  : " << std::setw(4) << s.countReserved  << "건\n"
              << "  PRODUCING : " << std::setw(4) << s.countProducing << "건\n"
              << "  CONFIRMED : " << std::setw(4) << s.countConfirmed << "건\n"
              << "  RELEASED  : " << std::setw(4) << s.countRelease   << "건\n"
              << SEP_S
              << "  합계      : " << std::setw(4) << s.total          << "건  (REJECTED 제외)\n";
}

static void printSampleTable(const std::vector<SampleStatus>& statuses)
{
    if (statuses.empty()) { std::cout << "  등록된 시료가 없습니다.\n"; return; }

    std::cout << std::right
              << std::setw(5)  << "ID"
              << std::setw(6)  << "재고"
              << std::setw(6)  << "수요"
              << "  상태   이름\n"
              << "  " << std::string(44, '-') << "\n";

    for (const auto& s : statuses)
    {
        std::cout << std::right
                  << std::setw(5)  << s.id
                  << std::setw(6)  << s.stock
                  << std::setw(6)  << s.demand
                  << "  " << std::left << std::setw(8) << toStatusLabel(s.stockStatus)
                  << s.name << "\n";
    }
}

// ── 메뉴 / 입력 ───────────────────────────────────────────────────────────────

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

// ── Dashboard ─────────────────────────────────────────────────────────────────

void MonitoringView::showDashboard(const OrderSummary&               summary,
                                   const std::vector<SampleStatus>&  statuses,
                                   const std::vector<ProductionLine>& lines) const
{
    std::cout << SEP
              << "       [SampleOrderSystem Dashboard]\n"
              << SEP;

    // 주문 현황
    printSectionHeader("주문 현황");
    printOrderRows(summary);

    // 시료 재고 현황
    printSectionHeader("시료 재고 현황");
    printSampleTable(statuses);

    // 생산 현황 요약
    printSectionHeader("생산 현황");
    int running = 0, paused = 0;
    for (const auto& l : lines)
    {
        if (l.status == "RUNNING") ++running;
        else if (l.status == "PAUSED") ++paused;
    }
    std::cout << "  활성 라인 — RUNNING: " << running
              << "건  PAUSED: " << paused << "건\n";

    std::cout << "\n" << SEP;
}

// ── 개별 조회 화면 ────────────────────────────────────────────────────────────

void MonitoringView::showOrderSummary(const OrderSummary& summary) const
{
    std::cout << SEP << "         [ 주문 상태별 집계 ]\n" << SEP;
    printOrderRows(summary);
    std::cout << "\n" << SEP;
}

void MonitoringView::showSampleList(const std::vector<SampleStatus>& statuses) const
{
    std::cout << SEP << "         [ 시료별 재고 현황 ]\n" << SEP;
    printSampleTable(statuses);

    // 재고 상태 요약
    int surplus = 0, shortage = 0, depleted = 0;
    for (const auto& s : statuses)
    {
        if      (s.stockStatus == StockStatus::SURPLUS)  ++surplus;
        else if (s.stockStatus == StockStatus::SHORTAGE) ++shortage;
        else if (s.stockStatus == StockStatus::DEPLETED) ++depleted;
    }
    std::cout << SEP_S
              << "  여유: " << surplus  << "개  "
              << "부족: "   << shortage << "개  "
              << "고갈: "   << depleted << "개\n"
              << "\n" << SEP;
}

void MonitoringView::showProductionStatus(const std::vector<ProductionLine>& lines) const
{
    std::cout << SEP << "       [ 생산 현황 (RUNNING / PAUSED) ]\n" << SEP;

    if (lines.empty())
    {
        std::cout << "  활성 생산 라인이 없습니다.\n";
    }
    else
    {
        std::cout << std::right
                  << std::setw(6)  << "ID"
                  << std::setw(8)  << "주문ID"
                  << std::setw(10) << "진행률"
                  << "  상태      라인명\n"
                  << "  " << std::string(42, '-') << "\n";

        for (const auto& l : lines)
        {
            std::cout << std::right
                      << std::setw(6)  << l.id
                      << std::setw(8)  << l.orderId
                      << std::setw(9)  << l.progress << "%"
                      << "  " << std::left << std::setw(10) << l.status
                      << l.lineName << "\n";
        }
    }
    std::cout << "\n" << SEP;
}
