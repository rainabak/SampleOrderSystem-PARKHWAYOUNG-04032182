#pragma once
#include <string>
#include <vector>
#include "../models/OrderSummary.h"
#include "../models/SampleStatus.h"
#include "../models/ProductionLine.h"

class MonitoringView
{
public:
    void showMenu()                                                         const;
    int  getMenuChoice()                                                    const;
    void showMessage(const std::string& msg)                                const;
    void showDashboard(const OrderSummary&              summary,
                       const std::vector<SampleStatus>& statuses)           const;
    void showOrderSummary(const OrderSummary& summary)                      const;
    void showSampleList(const std::vector<SampleStatus>& statuses)          const;
    void showProductionStatus(const std::vector<ProductionLine>& lines)     const;
};
