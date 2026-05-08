#include "ProductionLineRepository.h"
#include "../persistence/JsonUtil.h"
#include <algorithm>
#include <sstream>

// ── ProductionLine 직렬화/역직렬화 (이 파일 전용) ────────────────────────────

static std::string toJson(const ProductionLine& pl)
{
    std::ostringstream oss;
    oss << "{"
        << "\"id\":"         << pl.id
        << ",\"orderId\":"   << pl.orderId
        << ",\"lineName\":\"" << JsonUtil::escapeString(pl.lineName) << "\""
        << ",\"status\":\""  << JsonUtil::escapeString(pl.status)   << "\""
        << ",\"progress\":"  << pl.progress
        << "}";
    return oss.str();
}

static std::string buildJson(const std::vector<ProductionLine>& lines, int nextId)
{
    std::ostringstream oss;
    oss << "{\"nextId\":" << nextId << ",\"items\":[";
    for (size_t i = 0; i < lines.size(); i++)
    {
        if (i > 0) oss << ",";
        oss << toJson(lines[i]);
    }
    oss << "]}";
    return oss.str();
}

static ProductionLine fromJson(const std::string& obj)
{
    ProductionLine pl;
    pl.id       = JsonUtil::readInt(obj,    "id");
    pl.orderId  = JsonUtil::readInt(obj,    "orderId");
    pl.lineName = JsonUtil::readString(obj, "lineName");
    pl.status   = JsonUtil::readString(obj, "status");
    pl.progress = JsonUtil::readInt(obj,    "progress");
    return pl;
}

static void loadFromJson(const std::string& raw,
                         std::vector<ProductionLine>& out,
                         int& nextId)
{
    out.clear();
    nextId = 1;
    if (raw.empty()) return;

    nextId = JsonUtil::readInt(raw, "nextId");

    for (const auto& obj : JsonUtil::splitObjects(raw))
    {
        ProductionLine pl = fromJson(obj);
        if (pl.id > 0)
            out.push_back(std::move(pl));
    }

    for (const auto& pl : out)
    {
        if (pl.id >= nextId)
            nextId = pl.id + 1;
    }
}

// ── ProductionLineRepository ──────────────────────────────────────────────────

ProductionLineRepository::ProductionLineRepository(JsonFileStorage& storage)
    : m_storage(storage)
{
    load();
}

void ProductionLineRepository::add(const ProductionLine& line)
{
    ProductionLine newLine;
    newLine.id       = m_nextId++;
    newLine.orderId  = line.orderId;
    newLine.lineName = line.lineName;
    newLine.status   = line.status;
    newLine.progress = line.progress;
    m_lines.push_back(newLine);
    persist();
}

std::vector<ProductionLine> ProductionLineRepository::findAll() const
{
    if (!m_storage.exists())
        return {};

    std::vector<ProductionLine> result;
    int nextId;
    loadFromJson(m_storage.load(), result, nextId);
    return result;
}

ProductionLine* ProductionLineRepository::findById(int id)
{
    auto it = std::find_if(m_lines.begin(), m_lines.end(),
                           [id](const ProductionLine& pl) { return pl.id == id; });
    return it != m_lines.end() ? &(*it) : nullptr;
}

ProductionLine* ProductionLineRepository::findByOrderId(int orderId)
{
    auto it = std::find_if(m_lines.begin(), m_lines.end(),
                           [orderId](const ProductionLine& pl) { return pl.orderId == orderId; });
    return it != m_lines.end() ? &(*it) : nullptr;
}

bool ProductionLineRepository::update(const ProductionLine& line)
{
    auto it = std::find_if(m_lines.begin(), m_lines.end(),
                           [&line](const ProductionLine& pl) { return pl.id == line.id; });
    if (it == m_lines.end()) return false;

    *it = line;
    persist();
    return true;
}

bool ProductionLineRepository::remove(int id)
{
    auto it = std::find_if(m_lines.begin(), m_lines.end(),
                           [id](const ProductionLine& pl) { return pl.id == id; });
    if (it == m_lines.end()) return false;

    m_lines.erase(it);
    persist();
    return true;
}

// ── private ───────────────────────────────────────────────────────────────────

void ProductionLineRepository::load()
{
    loadFromJson(m_storage.load(), m_lines, m_nextId);
}

void ProductionLineRepository::persist() const
{
    m_storage.save(buildJson(m_lines, m_nextId));
}
