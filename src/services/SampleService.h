#pragma once
#include <string>
#include <vector>
#include "../repositories/ISampleRepository.h"
#include "../models/Sample.h"

class SampleService
{
public:
    explicit SampleService(ISampleRepository& sampleRepo);

    bool                createSample(const std::string& name,
                                     const std::string& description,
                                     int                avgProductionTime,
                                     double             yield,
                                     int                stock);
    bool                updateSample(const Sample& updated);
    bool                removeSample(int id);

    std::vector<Sample> findAll()                                const;
    Sample*             findById(int id);
    std::vector<Sample> searchByName(const std::string& keyword) const;

    std::string         getLastError() const;

private:
    ISampleRepository& m_sampleRepo;
    std::string        m_lastError;

    bool validateName(const std::string& name);
    bool validateYield(double yield);
    bool validateAvgProductionTime(int time);
    bool validateStock(int stock);
    bool isDuplicateName(const std::string& name, int excludeId = 0);
};
