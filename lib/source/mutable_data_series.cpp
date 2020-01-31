#include "mutable_data_series.hpp"

namespace ds {

mutable_data_series::mutable_data_series(uint64_t id, double frequency, double start)
    : static_data_series(id, {}, frequency, start)
{
}

mutable_data_series::mutable_data_series(uint64_t id, nlohmann::json json)
    : static_data_series(id, json)
{
}

void mutable_data_series::push_back(double value)
{
    m_data.push_back(value);
    m_hash = m_hash ^ (std::hash<double>()(value) << 1);
    m_properties.add_samples(1);
}

void mutable_data_series::push_back(std::vector<double> values)
{
    m_data.insert(m_data.end(), values.begin(), values.end());
    for (auto value : values)
        m_hash = m_hash ^ (std::hash<double>()(value) << 1);
    m_properties.add_samples(values.size());
}

data_series::data_type_t mutable_data_series::type() const
{
    return data_series::mutable_data;
}

}