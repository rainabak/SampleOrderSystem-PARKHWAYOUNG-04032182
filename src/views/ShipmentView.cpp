#include "ShipmentView.h"
#include <iomanip>
#include <iostream>
#include <limits>

void ShipmentView::showMenu() const
{
    std::cout << "============================\n"
              << "        출고 관리           \n"
              << "============================\n"
              << "  1. 출고 처리\n"
              << "  2. 출고 이력 조회\n"
              << "  0. 돌아가기\n"
              << "============================\n"
              << "선택: ";
}

int ShipmentView::getMenuChoice() const
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

void ShipmentView::showMessage(const std::string& msg) const
{
    std::cout << "\n" << msg << "\n";
}

int ShipmentView::promptOrderId() const
{
    int id;
    while (true)
    {
        std::cout << "출고할 주문 ID 입력: ";
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

void ShipmentView::showOrders(const std::vector<Order>& orders) const
{
    std::cout << "\n출고 가능 주문 (CONFIRMED)\n"
              << std::right
              << std::setw(6)  << "ID"
              << std::setw(8)  << "시료ID"
              << std::setw(8)  << "수량"
              << "  고객사              제품명\n"
              << std::string(60, '-') << "\n";
    for (const auto& o : orders)
    {
        std::cout << std::right
                  << std::setw(6)  << o.id
                  << std::setw(8)  << o.sampleId
                  << std::setw(8)  << o.quantity
                  << "  " << std::left << std::setw(20) << o.customerName
                  << o.productName << "\n";
    }
    std::cout << "\n";
}

void ShipmentView::showShipments(const std::vector<Shipment>& shipments) const
{
    std::cout << "\n출고 이력\n"
              << std::right
              << std::setw(6)  << "ID"
              << std::setw(8)  << "주문ID"
              << std::setw(8)  << "시료ID"
              << std::setw(6)  << "수량"
              << "  출고일자    고객사\n"
              << std::string(60, '-') << "\n";
    for (const auto& s : shipments)
    {
        std::cout << std::right
                  << std::setw(6)  << s.id
                  << std::setw(8)  << s.orderId
                  << std::setw(8)  << s.sampleId
                  << std::setw(6)  << s.quantity
                  << "  " << std::left << std::setw(12) << s.shippedAt
                  << s.customerName << "\n";
    }
    std::cout << "\n";
}

void ShipmentView::showShipment(const Shipment& s) const
{
    std::cout << "\n  ID      : " << s.id
              << "\n  주문 ID : " << s.orderId
              << "\n  시료 ID : " << s.sampleId
              << "\n  수량    : " << s.quantity
              << "\n  고객사  : " << s.customerName
              << "\n  출고일  : " << s.shippedAt << "\n\n";
}
