#include "functional_data_series.hpp"
#include <math.h>
#include <algorithm>

namespace ds {


functional_data_series::functional_data_series(function_type_t func_type, uint64_t id, const data_series_properties& props, std::shared_ptr<data_series> source)
    : data_series(id, props)
    , m_type(func_type)
    , m_source(source)
{
    switch (func_type)
    {
        case sin_function: 
            m_function = std::bind((double(*)(double))&std::sin, std::placeholders::_1);
            break;
        case cos_function:
            m_function = std::bind((double(*)(double))&std::cos, std::placeholders::_1);
            break;
        case tan_function: 
            m_function = std::bind((double(*)(double))&std::tan, std::placeholders::_1);
            break;
        case asin_function:
            m_function = std::bind((double(*)(double))&std::asin, std::placeholders::_1);
            break;        
        case acos_function:
            m_function = std::bind((double(*)(double))&std::acos, std::placeholders::_1);
            break;        
        case atan_function:
            m_function = std::bind((double(*)(double))&std::atan, std::placeholders::_1);
            break;        
        case square_function: 
            m_function = [](double x) { return x * x; };
            break;
        case square_root_function: 
            m_function = std::bind((double(*)(double))&std::sqrt, std::placeholders::_1);
            break;
        case abs_function: 
            m_function = std::bind((double(*)(double))&std::fabs, std::placeholders::_1);
            break;
        case negate_function: 
            m_function = [](double x) { return -x; };
            break;
        case exp_function: 
            m_function = std::bind((double(*)(double))&std::exp, std::placeholders::_1);
            break;
        case log_function: 
            m_function = std::bind((double(*)(double))&std::log, std::placeholders::_1);
            break;
        default:
            m_function = [](double x) { return x; };
            break;
    }
}

size_t functional_data_series::hash() const
{
    return m_source ? m_source->hash() : std::hash<int>()(static_cast<int>(m_type + 1));
}

data_series::data_type_t functional_data_series::type() const
{
    return data_series::function_data;
}

double functional_data_series::operator()(double x)
{
    if (m_source)
        return m_function((*m_source)(x));
    else
        return m_function(x);
}

nlohmann::json functional_data_series::to_json(data_encoding::encoding_type_t type) const {
    auto j = data_series::to_json(type);
    j[JSON_FUNC_DATAS_TYPE] = function_type();
    j[JSON_FUNC_DATAS_SOURCE] = source_id();
    return j;
}

bool functional_data_series::operator==(const functional_data_series& rhs) const
{
    return data_series::operator==(rhs) && m_type == rhs.m_type && m_source == rhs.m_source;
}

}