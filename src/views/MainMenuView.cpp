#include "MainMenuView.h"
#include <iostream>
#include <limits>

void MainMenuView::showMenu() const
{
    std::cout << "============================\n"
              << "    SampleOrderSystem       \n"
              << "============================\n"
              << "  1. 시료 관리\n"
              << "  2. 주문 접수\n"
              << "  3. 주문 승인 / 거절\n"
              << "  4. 생산 라인\n"
              << "  5. 모니터링\n"
              << "  6. 출고 처리\n"
              << "  0. 종료\n"
              << "============================\n"
              << "선택: ";
}

int MainMenuView::getMenuChoice() const
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
