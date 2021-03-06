#include "static_data_series.hpp"
#include "data_encoding.hpp"

namespace ds {


static_data_series::static_data_series(uint64_t id, const data_series_properties& props, double initial_value)
    : data_series(id, props)
    , m_data(props.samples())
    , m_hash(0)
{
    std::fill(m_data.begin(), m_data.end(), initial_value);
    calculate_hash();
}

static_data_series::static_data_series(uint64_t id, const data_series_properties& props, std::function<double(double)> func)
    : data_series(id, props)
    , m_data(props.samples())
    , m_hash(0)
{
    for (auto sample = 0; sample < properties().samples(); sample++)
        m_data[sample] = func(properties().sampleToTime(sample));
    calculate_hash();
}

static_data_series::static_data_series(uint64_t id, std::initializer_list<double> list, double frequency, double start)
    : data_series(id, data_series_properties(list.size(), frequency, start))
    , m_data(list)
    , m_hash(0)
{
    calculate_hash();
}

static_data_series::static_data_series(uint64_t id, const std::vector<double>& vector, double frequency, double start)
    : data_series(id, data_series_properties(vector.size(), frequency, start))
    , m_data(vector)
    , m_hash(0)
{
    calculate_hash();
}

static_data_series::static_data_series(uint64_t id, nlohmann::json json)
    : data_series(id, data_series_properties(json[JSON_DATAS_PROPERTIES]))
    , m_hash(0)
{
    if (json.contains(JSON_STATIC_DATAS_CONST)) {
        double const_value = json[JSON_STATIC_DATAS_CONST];
        m_data.resize(properties().samples());
        std::fill(m_data.begin(), m_data.end(), const_value);
    } else {
        data_encoding::encoding_type_t type = json[JSON_STATIC_DATAS_ENCODING];
        m_data = data_encoding::decode(type, json[JSON_STATIC_DATAS_ENCODED]);
    }
    calculate_hash();
}

size_t static_data_series::hash() const
{
    return m_hash;
}

data_series::data_type_t static_data_series::type() const
{
    return data_series::static_data;
}

double static_data_series::operator()(double x)
{
    auto sample = properties().timeToSample(x);    
    if (sample < 0 || sample >= properties().samples())
        return std::numeric_limits<double>::quiet_NaN();
    return m_data[sample];
}

nlohmann::json static_data_series::to_json(data_encoding::encoding_type_t type) const {
    auto j = data_series::to_json(type);
    
    auto count = std::count (m_data.begin(), m_data.end(), *m_data.begin());
    if (count == m_data.size()) {
        j[JSON_STATIC_DATAS_CONST] = *m_data.begin();
    } else {
        j[JSON_STATIC_DATAS_ENCODING] = type;
        j[JSON_STATIC_DATAS_ENCODED] = data_encoding::encode(type, m_data);
    }
    return j;
}

bool static_data_series::operator==(const static_data_series& rhs) const
{
    return data_series::operator==(rhs) && std::equal(m_data.begin(), m_data.end(), rhs.m_data.begin());
}

void static_data_series::calculate_hash()
{
    for (auto value : *this)
        m_hash = m_hash ^ (std::hash<double>()(value) << 1);
}

}