#include "static_data_series.hpp"

namespace ds {


static_data_series::static_data_series(uint64_t id, const data_series_properties& props, double initial_value)
    : data_series(id, props, data_type_t::static_data)
    , m_data(props.samples())
{
    std::fill(m_data.begin(), m_data.end(), initial_value);
}

static_data_series::static_data_series(uint64_t id, const data_series_properties& props, std::function<double(double)> func)
    : data_series(id, props, data_type_t::static_data)
    , m_data(props.samples())
{
    for (auto sample = 0; sample < properties().samples(); sample++)
        m_data[sample] = func(properties().sampleToTime(sample));
}

static_data_series::static_data_series(uint64_t id, std::initializer_list<double> list, double frequency, double start)
    : data_series(id, data_series_properties(list.size(), frequency, start), data_type_t::static_data)
    , m_data(list)
{
}

static_data_series::static_data_series(uint64_t id, const std::vector<double>& vector, double frequency, double start)
    : data_series(id, data_series_properties(vector.size(), frequency, start), data_type_t::static_data)
    , m_data(vector)
{
}

double static_data_series::operator()(double x) const
{
    auto sample = properties().timeToSample(x);    
    if (sample < 0 || sample >= properties().samples())
        return std::numeric_limits<double>::quiet_NaN();
    return m_data[sample];
}

}