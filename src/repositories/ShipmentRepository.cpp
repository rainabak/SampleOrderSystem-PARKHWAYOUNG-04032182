#include "ShipmentRepository.h"
#include "../persistence/JsonUtil.h"
#include <algorithm>
#include <sstream>

// ── Shipment 직렬화/역직렬화 (이 파일 전용) ──────────────────────────────────

static std::string toJson(const Shipment& s)
{
    std::ostringstream oss;
    oss << "{"
        << "\"id\":"           << s.id
        << ",\"orderId\":"     << s.orderId
        << ",\"sampleId\":"    << s.sampleId
        << ",\"quantity\":"    << s.quantity
        << ",\"customerName\":\"" << JsonUtil::escapeString(s.customerName) << "\""
        << ",\"shippedAt\":\""    << JsonUtil::escapeString(s.shippedAt)    << "\""
        << "}";
    return oss.str();
}

static std::string buildJson(const std::vector<Shipment>& shipments, int nextId)
{
    std::ostringstream oss;
    oss << "{\"nextId\":" << nextId << ",\"items\":[";
    for (size_t i = 0; i < shipments.size(); i++)
    {
        if (i > 0) oss << ",";
        oss << toJson(shipments[i]);
    }
    oss << "]}";
    return oss.str();
}

static Shipment fromJson(const std::string& obj)
{
    Shipment s;
    s.id           = JsonUtil::readInt(obj,    "id");
    s.orderId      = JsonUtil::readInt(obj,    "orderId");
    s.sampleId     = JsonUtil::readInt(obj,    "sampleId");
    s.quantity     = JsonUtil::readInt(obj,    "quantity");
    s.customerName = JsonUtil::readString(obj, "customerName");
    s.shippedAt    = JsonUtil::readString(obj, "shippedAt");
    return s;
}

static void loadFromJson(const std::string&    raw,
                         std::vector<Shipment>& out,
                         int&                   nextId)
{
    out.clear();
    nextId = 1;
    if (raw.empty()) return;

    nextId = JsonUtil::readInt(raw, "nextId");

    for (const auto& obj : JsonUtil::splitObjects(raw))
    {
        Shipment s = fromJson(obj);
        if (s.id > 0)
            out.push_back(std::move(s));
    }

    for (const auto& s : out)
    {
        if (s.id >= nextId)
            nextId = s.id + 1;
    }
}

// ── ShipmentRepository ────────────────────────────────────────────────────────

ShipmentRepository::ShipmentRepository(JsonFileStorage& storage)
    : m_storage(storage)
{
    load();
}

void ShipmentRepository::add(const Shipment& shipment)
{
    Shipment newShipment   = shipment;
    newShipment.id         = m_nextId++;
    m_shipments.push_back(newShipment);
    persist();
}

std::vector<Shipment> ShipmentRepository::findAll() const
{
    if (!m_storage.exists())
        return {};

    std::vector<Shipment> result;
    int nextId;
    loadFromJson(m_storage.load(), result, nextId);
    return result;
}

Shipment* ShipmentRepository::findById(int id)
{
    auto it = std::find_if(m_shipments.begin(), m_shipments.end(),
                           [id](const Shipment& s){ return s.id == id; });
    return it != m_shipments.end() ? &(*it) : nullptr;
}

void ShipmentRepository::load()
{
    loadFromJson(m_storage.load(), m_shipments, m_nextId);
}

void ShipmentRepository::persist() const
{
    m_storage.save(buildJson(m_shipments, m_nextId));
}
