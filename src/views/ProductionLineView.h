#pragma once
#include <string>
#include <vector>
#include "../models/ProductionLine.h"
#include "../services/ProductionService.h"

class ProductionLineView
{
public:
    void showMenu()                                             const;
    int  getMenuChoice()                                        const;
    void showMessage(const std::string& msg)                    const;

    int  promptLineId()                                         const;
    int  promptProgress()                                       const;
    void showLines(const std::vector<ProductionLine>& lines)    const;
    void showLine(const ProductionLine& line)                   const;

    void showProductionStarted(const ProductionStartResult& r)  const;
    void showProductionCompleted(int orderId, int stockAdded)   const;
};
