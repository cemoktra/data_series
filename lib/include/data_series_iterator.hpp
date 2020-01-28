#ifndef _DATA_SERIES_ITERATOR_HPP_
#define _DATA_SERIES_ITERATOR_HPP_

#include <functional>
#include <iterator>
#include <math.h>

namespace ds {

class data_series;

class data_series_iterator
{
public:
    typedef double value_type;
    typedef std::ptrdiff_t difference_type;
    typedef value_type* pointer;
    typedef value_type& reference;
    typedef std::input_iterator_tag iterator_category;

    explicit data_series_iterator(uint64_t sample, data_series* data);

    value_type operator*() const { return m_value; }

    bool operator==(const data_series_iterator& other) const { return m_data == other.m_data && m_sample == other.m_sample; }
    bool operator!=(const data_series_iterator& other) const { return !(*this == other); }

    value_type operator++(int);
    data_series_iterator& operator++();

private:
    value_type update_value(uint64_t sample);

    data_series* m_data;
    value_type m_value;
    uint64_t m_sample;
};

}

#endif