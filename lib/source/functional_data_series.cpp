#include "functional_data_series.hpp"
#include <math.h>
#include <algorithm>

namespace ds {

static constexpr double maximum_frequency = 1.0 / std::numeric_limits<double>::epsilon();



functional_data_series::functional_data_series(function_type_t func_type, uint64_t id, const data_series_properties& props, std::shared_ptr<data_series> source)
    : data_series(id, props, data_type_t::function_data)
    , m_type(func_type)
    , m_source(source)
{
}

double functional_data_series::operator()(double x) const
{
    return m_source ? (*m_source)(x) : x;    
}

// sinus class
sin_data_series::sin_data_series(uint64_t id, const data_series_properties& props, std::shared_ptr<data_series> source)
    : functional_data_series(sin_function, id, props, source)
{
}

double sin_data_series::operator()(double x) const
{
    return sin(functional_data_series::operator()(x));
}

// cosinus class
cos_data_series::cos_data_series(uint64_t id, const data_series_properties& props, std::shared_ptr<data_series> source)
    : functional_data_series(cos_function, id, props, source)
{
}

double cos_data_series::operator()(double x) const
{
    return cos(functional_data_series::operator()(x));
}

// tangens class
tan_data_series::tan_data_series(uint64_t id, const data_series_properties& props, std::shared_ptr<data_series> source)
    : functional_data_series(cos_function, id, props, source)
{
}

double tan_data_series::operator()(double x) const
{
    return tan(functional_data_series::operator()(x));
}

// arc sinus class
asin_data_series::asin_data_series(uint64_t id, const data_series_properties& props, std::shared_ptr<data_series> source)
    : functional_data_series(asin_function, id, props, source)
{
}

double asin_data_series::operator()(double x) const
{
    return asin(functional_data_series::operator()(x));
}

// arc cosinus class
acos_data_series::acos_data_series(uint64_t id, const data_series_properties& props, std::shared_ptr<data_series> source)
    : functional_data_series(acos_function, id, props, source)
{
}

double acos_data_series::operator()(double x) const
{
    return acos(functional_data_series::operator()(x));
}

// arc tangens class
atan_data_series::atan_data_series(uint64_t id, const data_series_properties& props, std::shared_ptr<data_series> source)
    : functional_data_series(atan_function, id, props, source)
{
}

double atan_data_series::operator()(double x) const
{
    return atan(functional_data_series::operator()(x));
}

// square class
square_data_series::square_data_series(uint64_t id, const data_series_properties& props, std::shared_ptr<data_series> source)
    : functional_data_series(square_function, id, props, source)
{
}

double square_data_series::operator()(double x) const
{
    auto source_x = functional_data_series::operator()(x);
    return source_x * source_x;
}

// square root class
square_root_data_series::square_root_data_series(uint64_t id, const data_series_properties& props, std::shared_ptr<data_series> source)
    : functional_data_series(square_root_function, id, props, source)
{
}

double square_root_data_series::operator()(double x) const
{
    return sqrt(functional_data_series::operator()(x));
}

// abs class
abs_data_series::abs_data_series(uint64_t id, const data_series_properties& props, std::shared_ptr<data_series> source)
    : functional_data_series(abs_function, id, props, source)
{
}

double abs_data_series::operator()(double x) const
{
    return fabs(functional_data_series::operator()(x));
}

// negate class
negate_data_series::negate_data_series(uint64_t id, const data_series_properties& props, std::shared_ptr<data_series> source)
    : functional_data_series(negate_function, id, props, source)
{
}

double negate_data_series::operator()(double x) const
{
    return -(functional_data_series::operator()(x));
}

// pow class
pow_data_series::pow_data_series(uint64_t id, const data_series_properties& props, std::shared_ptr<data_series> source)
    : functional_data_series(negate_function, id, props, source)
{
}

double pow_data_series::operator()(double x) const
{
    return pow(x, functional_data_series::operator()(x));
}


}