#include "SampleService.h"
#include <algorithm>
#include <cctype>

SampleService::SampleService(ISampleRepository& sampleRepo)
    : m_sampleRepo(sampleRepo)
{}

// ── CRUD ─────────────────────────────────────────────────────────────────────

bool SampleService::createSample(const std::string& name,
                                  const std::string& description,
                                  int                avgProductionTime,
                                  double             yield,
                                  int                stock)
{
    if (!validateName(name))                       return false;
    if (isDuplicateName(name))                     return false;
    if (!validateAvgProductionTime(avgProductionTime)) return false;
    if (!validateYield(yield))                     return false;
    if (!validateStock(stock))                     return false;

    Sample s;
    s.id                = 0;
    s.name              = name;
    s.description       = description;
    s.avgProductionTime = avgProductionTime;
    s.yield             = yield;
    s.stock             = stock;
    m_sampleRepo.add(s);
    m_lastError.clear();
    return true;
}

bool SampleService::updateSample(const Sample& updated)
{
    if (!m_sampleRepo.findById(updated.id))
    {
        m_lastError = "해당 ID의 시료를 찾을 수 없습니다.";
        return false;
    }
    if (!validateName(updated.name))                          return false;
    if (isDuplicateName(updated.name, updated.id))            return false;
    if (!validateAvgProductionTime(updated.avgProductionTime)) return false;
    if (!validateYield(updated.yield))                        return false;
    if (!validateStock(updated.stock))                        return false;

    const bool ok = m_sampleRepo.update(updated);
    if (!ok) m_lastError = "수정에 실패했습니다.";
    else     m_lastError.clear();
    return ok;
}

bool SampleService::removeSample(int id)
{
    if (!m_sampleRepo.findById(id))
    {
        m_lastError = "해당 ID의 시료를 찾을 수 없습니다.";
        return false;
    }
    const bool ok = m_sampleRepo.remove(id);
    if (!ok) m_lastError = "삭제에 실패했습니다.";
    else     m_lastError.clear();
    return ok;
}

// ── 조회 ─────────────────────────────────────────────────────────────────────

std::vector<Sample> SampleService::findAll() const
{
    return m_sampleRepo.findAll();
}

Sample* SampleService::findById(int id)
{
    return m_sampleRepo.findById(id);
}

std::vector<Sample> SampleService::searchByName(const std::string& keyword) const
{
    auto toLower = [](std::string s) {
        std::transform(s.begin(), s.end(), s.begin(),
                       [](unsigned char c){ return static_cast<char>(std::tolower(c)); });
        return s;
    };

    const std::string lowerKey = toLower(keyword);
    std::vector<Sample> result;

    for (const auto& s : m_sampleRepo.findAll())
    {
        if (toLower(s.name).find(lowerKey) != std::string::npos)
            result.push_back(s);
    }
    return result;
}

std::string SampleService::getLastError() const
{
    return m_lastError;
}

// ── 검증 (private) ────────────────────────────────────────────────────────────

bool SampleService::validateName(const std::string& name)
{
    if (name.empty())
    {
        m_lastError = "시료 이름은 비워둘 수 없습니다.";
        return false;
    }
    if (name.size() > 50)
    {
        m_lastError = "시료 이름은 50자를 초과할 수 없습니다.";
        return false;
    }
    return true;
}

bool SampleService::validateYield(double yield)
{
    if (yield <= 0.0 || yield > 100.0)
    {
        m_lastError = "수율은 0 초과 100 이하여야 합니다.";
        return false;
    }
    return true;
}

bool SampleService::validateAvgProductionTime(int time)
{
    if (time < 1)
    {
        m_lastError = "평균 생산 시간은 1분 이상이어야 합니다.";
        return false;
    }
    return true;
}

bool SampleService::validateStock(int stock)
{
    if (stock < 0)
    {
        m_lastError = "재고는 0 이상이어야 합니다.";
        return false;
    }
    return true;
}

bool SampleService::isDuplicateName(const std::string& name, int excludeId)
{
    for (const auto& s : m_sampleRepo.findAll())
    {
        if (s.id == excludeId) continue;
        if (s.name == name)
        {
            m_lastError = "이미 존재하는 시료 이름입니다: " + name;
            return true;
        }
    }
    return false;
}
