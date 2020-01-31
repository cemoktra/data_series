#include "mutable_data_series.hpp"

namespace ds {

mutable_data_series::mutable_data_series(uint64_t id, double frequency, double start)
    : static_data_series(id, {}, frequency, start)
{
}

void mutable_data_series::push_back(double value)
{
    m_data.push_back(value);
    m_properties.add_samples(1);
}

void mutable_data_series::push_back(std::vector<double> values)
{
    m_data.insert(m_data.end(), values.begin(), values.end());
    m_properties.add_samples(values.size());
}

}