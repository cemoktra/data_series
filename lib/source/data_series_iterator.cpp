#include "data_series_iterator.hpp"
#include "data_series.hpp"

namespace ds {

data_series_iterator::data_series_iterator(uint64_t sample, data_series* data)
    : m_data(data)
    , m_sample(sample)
{
    m_value = update_value(sample);
}

data_series_iterator::value_type data_series_iterator::operator++(int)
{
    auto ret = update_value(++m_sample);
    ++*this;
    return ret;
}

data_series_iterator& data_series_iterator::operator++()
{
    m_value = update_value(++m_sample);
    return *this;
}


data_series_iterator::value_type data_series_iterator::update_value(uint64_t sample)
{
    return (*m_data)(m_data->properties().sampleToTime(sample));
}

}