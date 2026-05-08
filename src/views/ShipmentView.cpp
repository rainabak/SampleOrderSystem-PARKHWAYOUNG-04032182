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

void ShipmentView::showShipments(const std::vector<Shipment>& shipments) const
{
    std::cout << "\n"
              << std::left
              << std::setw(6)  << "ID"
              << std::setw(10) << "주문ID"
              << std::setw(10) << "시료ID"
              << std::setw(8)  << "수량"
              << std::setw(16) << "고객사"
              << "출고일자" << "\n"
              << std::string(62, '-') << "\n";
    for (const auto& s : shipments)
    {
        std::cout << std::setw(6)  << s.id
                  << std::setw(10) << s.orderId
                  << std::setw(10) << s.sampleId
                  << std::setw(8)  << s.quantity
                  << std::setw(16) << s.customerName
                  << s.shippedAt << "\n";
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
