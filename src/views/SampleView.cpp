#include "SampleView.h"
#include <iomanip>
#include <iostream>
#include <limits>

void SampleView::showMenu() const
{
    std::cout << "============================\n"
              << "        시료 관리           \n"
              << "============================\n"
              << "  1. 시료 등록\n"
              << "  2. 시료 목록 조회\n"
              << "  3. 시료 검색\n"
              << "  4. 시료 수정\n"
              << "  5. 시료 삭제\n"
              << "  0. 돌아가기\n"
              << "============================\n"
              << "선택: ";
}

int SampleView::getMenuChoice() const
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

void SampleView::showMessage(const std::string& msg) const
{
    std::cout << "\n" << msg << "\n";
}

std::string SampleView::promptName() const
{
    std::string name;
    while (true)
    {
        std::cout << "이름 입력: ";
        std::getline(std::cin, name);
        if (!name.empty()) return name;
        std::cout << "이름은 비워둘 수 없습니다.\n";
    }
}

std::string SampleView::promptDescription() const
{
    std::string desc;
    std::cout << "설명 입력 (생략 가능): ";
    std::getline(std::cin, desc);
    return desc;
}

int SampleView::promptAvgProductionTime() const
{
    int time;
    while (true)
    {
        std::cout << "평균 생산 시간 입력 (분, 1 이상): ";
        if (std::cin >> time && time >= 1)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return time;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "1분 이상의 값을 입력하세요.\n";
    }
}

double SampleView::promptYield() const
{
    double yield;
    while (true)
    {
        std::cout << "수율(%) 입력 (0 초과 ~ 100): ";
        if (std::cin >> yield && yield > 0.0 && yield <= 100.0)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return yield;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "0 초과 100 이하의 수율을 입력하세요.\n";
    }
}

int SampleView::promptStock() const
{
    int stock;
    while (true)
    {
        std::cout << "초기 재고 입력 (0 이상): ";
        if (std::cin >> stock && stock >= 0)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return stock;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "0 이상의 값을 입력하세요.\n";
    }
}

int SampleView::promptId() const
{
    int id;
    while (true)
    {
        std::cout << "ID 입력: ";
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

std::string SampleView::promptSearchKeyword() const
{
    std::string keyword;
    std::cout << "검색어 입력 (이름 포함 검색): ";
    std::getline(std::cin, keyword);
    return keyword;
}

void SampleView::showSamples(const std::vector<Sample>& samples) const
{
    std::cout << "\n"
              << std::left
              << std::setw(6)  << "ID"
              << std::setw(22) << "이름"
              << std::setw(8)  << "재고"
              << std::setw(14) << "평균생산시간"
              << "수율(%)\n"
              << std::string(58, '-') << "\n";
    for (const auto& s : samples)
    {
        std::cout << std::setw(6)  << s.id
                  << std::setw(22) << s.name
                  << std::setw(8)  << s.stock
                  << std::setw(14) << (std::to_string(s.avgProductionTime) + "분")
                  << std::fixed << std::setprecision(1) << s.yield << "\n";
    }
    std::cout << std::defaultfloat << "\n";
}

void SampleView::showSample(const Sample& s) const
{
    std::cout << "\n현재 정보"
              << "\n  ID              : " << s.id
              << "\n  이름            : " << s.name
              << "\n  설명            : " << s.description
              << "\n  평균 생산 시간  : " << s.avgProductionTime << "분"
              << "\n  수율            : " << std::fixed << std::setprecision(1) << s.yield << "%"
              << "\n  재고            : " << s.stock << "\n\n"
              << std::defaultfloat;
}
