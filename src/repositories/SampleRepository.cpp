#include "SampleRepository.h"
#include "../persistence/JsonUtil.h"
#include <algorithm>
#include <sstream>

// ── Sample 직렬화/역직렬화 (이 파일 전용) ────────────────────────────────────

static std::string toJson(const Sample& s)
{
    std::ostringstream oss;
    oss << "{"
        << "\"id\":"                << s.id                                  << ","
        << "\"name\":\""            << JsonUtil::escapeString(s.name)        << "\","
        << "\"description\":\""     << JsonUtil::escapeString(s.description) << "\","
        << "\"stock\":"             << s.stock                               << ","
        << "\"avgProductionTime\":" << s.avgProductionTime                   << ","
        << "\"yield\":"             << s.yield
        << "}";
    return oss.str();
}

static std::string buildJson(const std::vector<Sample>& samples, int nextId)
{
    std::ostringstream oss;
    oss << "{\"nextId\":" << nextId << ",\"items\":[";
    for (size_t i = 0; i < samples.size(); i++)
    {
        if (i > 0) oss << ",";
        oss << toJson(samples[i]);
    }
    oss << "]}";
    return oss.str();
}

static Sample fromJson(const std::string& obj)
{
    Sample s;
    s.id                = JsonUtil::readInt(obj,    "id");
    s.name              = JsonUtil::readString(obj, "name");
    s.description       = JsonUtil::readString(obj, "description");
    s.stock             = JsonUtil::readInt(obj,    "stock");
    s.avgProductionTime = JsonUtil::readInt(obj,    "avgProductionTime");
    s.yield             = JsonUtil::readDouble(obj, "yield");
    return s;
}

static void loadFromJson(const std::string& raw,
                         std::vector<Sample>& out,
                         int& nextId)
{
    out.clear();
    nextId = 1;
    if (raw.empty()) return;

    nextId = JsonUtil::readInt(raw, "nextId");

    for (const auto& obj : JsonUtil::splitObjects(raw))
    {
        Sample s = fromJson(obj);
        if (s.id > 0)
            out.push_back(std::move(s));
    }

    for (const auto& s : out)
    {
        if (s.id >= nextId)
            nextId = s.id + 1;
    }
}

// ── SampleRepository ─────────────────────────────────────────────────────────

SampleRepository::SampleRepository(JsonFileStorage& storage)
    : m_storage(storage)
{
    loadFromJson(m_storage.load(), m_samples, m_nextId);
}

void SampleRepository::add(const Sample& sample)
{
    Sample newSample;
    newSample.id                = m_nextId++;
    newSample.name              = sample.name;
    newSample.description       = sample.description;
    newSample.stock             = sample.stock;
    newSample.avgProductionTime = sample.avgProductionTime;
    newSample.yield             = sample.yield;
    m_samples.push_back(newSample);
    persist();
}

std::vector<Sample> SampleRepository::findAll() const
{
    if (!m_storage.exists())
        return {};

    std::vector<Sample> result;
    int nextId;
    loadFromJson(m_storage.load(), result, nextId);
    return result;
}

Sample* SampleRepository::findById(int id)
{
    auto it = std::find_if(m_samples.begin(), m_samples.end(),
                           [id](const Sample& s) { return s.id == id; });
    return it != m_samples.end() ? &(*it) : nullptr;
}

bool SampleRepository::update(const Sample& sample)
{
    auto it = std::find_if(m_samples.begin(), m_samples.end(),
                           [&sample](const Sample& s) { return s.id == sample.id; });
    if (it == m_samples.end()) return false;

    *it = sample;
    persist();
    return true;
}

bool SampleRepository::remove(int id)
{
    auto it = std::find_if(m_samples.begin(), m_samples.end(),
                           [id](const Sample& s) { return s.id == id; });
    if (it == m_samples.end()) return false;

    m_samples.erase(it);
    persist();
    return true;
}

void SampleRepository::persist() const
{
    m_storage.save(buildJson(m_samples, m_nextId));
}
