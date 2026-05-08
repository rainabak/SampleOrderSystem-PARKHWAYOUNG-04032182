#include "OrderView.h"
#include <iomanip>
#include <iostream>
#include <limits>

void OrderView::showMenu() const
{
    std::cout << "============================\n"
              << "        주문 관리           \n"
              << "============================\n"
              << "  1. 주문 생성\n"
              << "  2. 주문 목록 조회\n"
              << "  3. 주문 승인\n"
              << "  4. 주문 거절\n"
              << "  0. 돌아가기\n"
              << "============================\n"
              << "선택: ";
}

int OrderView::getMenuChoice() const
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

void OrderView::showMessage(const std::string& msg) const
{
    std::cout << "\n" << msg << "\n";
}

int OrderView::promptSampleId() const
{
    int id;
    while (true)
    {
        std::cout << "시료 ID 입력: ";
        if (std::cin >> id && id > 0)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return id;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "유효한 시료 ID를 입력하세요.\n";
    }
}

std::string OrderView::promptCustomerName() const
{
    std::string name;
    while (true)
    {
        std::cout << "고객사명 입력: ";
        std::getline(std::cin, name);
        if (!name.empty()) return name;
        std::cout << "고객사명은 비워둘 수 없습니다.\n";
    }
}

int OrderView::promptQuantity() const
{
    int qty;
    while (true)
    {
        std::cout << "수량 입력: ";
        if (std::cin >> qty && qty > 0)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return qty;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "1 이상의 수량을 입력하세요.\n";
    }
}

int OrderView::promptOrderId() const
{
    int id;
    while (true)
    {
        std::cout << "주문 ID 입력: ";
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

std::string OrderView::promptLineName() const
{
    std::string name;
    while (true)
    {
        std::cout << "라인명 입력 (예: LINE-A01): ";
        std::getline(std::cin, name);
        if (!name.empty()) return name;
        std::cout << "라인명은 비워둘 수 없습니다.\n";
    }
}

void OrderView::showOrders(const std::vector<Order>& orders) const
{
    std::cout << "\n"
              << std::left
              << std::setw(6)  << "ID"
              << std::setw(8)  << "시료ID"
              << std::setw(20) << "제품명"
              << std::setw(16) << "고객사"
              << std::setw(8)  << "수량"
              << "상태" << "\n"
              << std::string(70, '-') << "\n";
    for (const auto& o : orders)
    {
        std::cout << std::setw(6)  << o.id
                  << std::setw(8)  << o.sampleId
                  << std::setw(20) << o.productName
                  << std::setw(16) << o.customerName
                  << std::setw(8)  << o.quantity
                  << o.status << "\n";
    }
    std::cout << "\n";
}

void OrderView::showOrder(const Order& o) const
{
    std::cout << "\n현재 정보"
              << "\n  ID      : " << o.id
              << "\n  시료 ID : " << o.sampleId
              << "\n  제품명  : " << o.productName
              << "\n  고객사  : " << o.customerName
              << "\n  수량    : " << o.quantity
              << "\n  상태    : " << o.status << "\n\n";
}
