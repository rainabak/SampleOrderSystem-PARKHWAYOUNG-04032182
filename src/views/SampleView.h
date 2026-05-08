#pragma once
#include <string>
#include <vector>
#include "../models/Sample.h"

class SampleView
{
public:
    void        showMenu()                                const;
    int         getMenuChoice()                           const;
    void        showMessage(const std::string& msg)       const;

    std::string promptName()                              const;
    std::string promptDescription()                       const;
    int         promptAvgProductionTime()                 const;
    double      promptYield()                             const;
    int         promptStock()                             const;
    int         promptId()                                const;
    std::string promptSearchKeyword()                     const;

    void        showSamples(const std::vector<Sample>&)   const;
    void        showSample(const Sample&)                 const;
};
