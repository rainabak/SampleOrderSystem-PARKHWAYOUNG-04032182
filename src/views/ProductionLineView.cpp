#include "ProductionLineView.h"
#include <iomanip>
#include <iostream>
#include <limits>

void ProductionLineView::showMenu() const
{
    std::cout << "============================\n"
              << "      생산 라인 관리        \n"
              << "============================\n"
              << "  1. 라인 목록 조회\n"
              << "  2. 진행률 갱신\n"
              << "  3. 생산 완료 처리\n"
              << "  4. 일시 중지\n"
              << "  5. 재개\n"
              << "  0. 돌아가기\n"
              << "============================\n"
              << "선택: ";
}

int ProductionLineView::getMenuChoice() const
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

void ProductionLineView::showMessage(const std::string& msg) const
{
    std::cout << "\n" << msg << "\n";
}

int ProductionLineView::promptLineId() const
{
    int id;
    while (true)
    {
        std::cout << "라인 ID 입력: ";
        if (std::cin >> id && id > 0)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return id;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "유효한 ID를 입력하세요.\n";
    }
}

int ProductionLineView::promptProgress() const
{
    int progress;
    while (true)
    {
        std::cout << "진행률 입력 (1-99): ";
        if (std::cin >> progress && progress >= 1 && progress <= 99)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return progress;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "1~99 사이의 값을 입력하세요.\n";
    }
}

void ProductionLineView::showLines(const std::vector<ProductionLine>& lines) const
{
    std::cout << "\n"
              << std::left
              << std::setw(6)  << "ID"
              << std::setw(10) << "주문ID"
              << std::setw(16) << "라인명"
              << std::setw(12) << "상태"
              << "진행률" << "\n"
              << std::string(56, '-') << "\n";
    for (const auto& l : lines)
    {
        std::cout << std::setw(6)  << l.id
                  << std::setw(10) << l.orderId
                  << std::setw(16) << l.lineName
                  << std::setw(12) << l.status
                  << l.progress << "%\n";
    }
    std::cout << "\n";
}

void ProductionLineView::showLine(const ProductionLine& l) const
{
    std::cout << "\n  ID      : " << l.id
              << "\n  주문 ID : " << l.orderId
              << "\n  라인명  : " << l.lineName
              << "\n  상태    : " << l.status
              << "\n  진행률  : " << l.progress << "%\n\n";
}
