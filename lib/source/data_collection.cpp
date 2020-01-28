#include "data_collection.hpp"
#include "static_data_series.hpp"

namespace ds {

std::atomic_uint64_t data_collection::m_next_id = 1;

data_collection::data_collection(uint64_t id)
    : m_id(id)
{
}

data_collection::~data_collection()
{
}

std::shared_ptr<data_collection> data_collection::create_collection()
{
    auto raw_ptr = new data_collection(m_next_id++);
    return std::shared_ptr<data_collection>(raw_ptr);
}

std::shared_ptr<data_series> data_collection::create_static_series(const data_series_properties& props, double initial_value)
{
    auto raw_ptr = new static_data_series(m_next_id++, props, initial_value);
    return std::shared_ptr<static_data_series>(raw_ptr);
}

std::shared_ptr<data_series> data_collection::create_static_series(const data_series_properties& props, std::function<double(double)> func)
{
    auto raw_ptr = new static_data_series(m_next_id++, props, func);
    return std::shared_ptr<static_data_series>(raw_ptr);
}

std::shared_ptr<data_series> data_collection::create_static_series(std::initializer_list<double> list, double frequency, double start)
{
    auto raw_ptr = new static_data_series(m_next_id++, list, frequency, start);
    return std::shared_ptr<static_data_series>(raw_ptr);
}

std::shared_ptr<data_series> data_collection::create_static_series(const std::vector<double>& vector, double frequency, double start)
{
    auto raw_ptr = new static_data_series(m_next_id++, vector, frequency, start);
    return std::shared_ptr<static_data_series>(raw_ptr);
}

std::shared_ptr<data_series> data_collection::create_polynom(const data_series_properties& props, const std::vector<double>& polynom)
{
    std::vector<std::shared_ptr<data_series>> polynom_series;

    for (auto i = polynom.size(); i > 0; i--) {
        auto degree = polynom.size() - i;

        auto const_fun = create_static_series(props, [i, polynom](double x) { return polynom[i - 1]; });
        if (degree) {
            auto degree_const_fun = create_static_series(props, [i, degree](double x) { return degree; });
            auto pow_fun = create_functional_data_series(functional_data_series::pow_function, props, degree_const_fun);
            auto mul_fun = create_operational_data_series(operational_data_series::mul_operation, {const_fun, pow_fun});
            polynom_series.push_back(mul_fun);
        } else
            polynom_series.push_back(const_fun);
    }
    return create_operational_data_series(operational_data_series::add_operation, polynom_series);
}


std::shared_ptr<data_series> data_collection::create_functional_data_series(functional_data_series::function_type_t type, const data_series_properties& props, std::shared_ptr<data_series> source)
{
    data_series *raw_ptr = nullptr;

    switch (type) {
        case functional_data_series::sin_function:
            raw_ptr = new sin_data_series(m_next_id++, props, source); break;
        case functional_data_series::cos_function:
            raw_ptr = new cos_data_series(m_next_id++, props, source); break;
        case functional_data_series::tan_function:
            raw_ptr = new tan_data_series(m_next_id++, props, source); break;
        case functional_data_series::asin_function:
            raw_ptr = new asin_data_series(m_next_id++, props, source); break;
        case functional_data_series::acos_function:
            raw_ptr = new acos_data_series(m_next_id++, props, source); break;
        case functional_data_series::atan_function:
            raw_ptr = new atan_data_series(m_next_id++, props, source); break;
        case functional_data_series::square_function:
            raw_ptr = new square_data_series(m_next_id++, props, source); break;
        case functional_data_series::square_root_function:
            raw_ptr = new square_root_data_series(m_next_id++, props, source); break;
        case functional_data_series::abs_function:
            raw_ptr = new abs_data_series(m_next_id++, props, source); break;
        case functional_data_series::negate_function:
            raw_ptr = new negate_data_series(m_next_id++, props, source); break;
        case functional_data_series::pow_function:
            raw_ptr = new pow_data_series(m_next_id++, props, source); break;            
        default:
            break;
    }

    return std::shared_ptr<data_series>(raw_ptr);
}

std::shared_ptr<data_series> data_collection::create_operational_data_series(operational_data_series::operation_type_t type, std::vector<std::shared_ptr<data_series>> sources)
{
    data_series *raw_ptr = nullptr;

    switch (type) {
        case operational_data_series::add_operation:
            raw_ptr = new add_operation_series(m_next_id++, sources); break;
        case operational_data_series::sub_operation:
            raw_ptr = new sub_operation_series(m_next_id++, sources); break;
        case operational_data_series::mul_operation:
            raw_ptr = new mul_operation_series(m_next_id++, sources); break;
        case operational_data_series::div_operation:
            raw_ptr = new div_operation_series(m_next_id++, sources); break;
        case operational_data_series::max_operation:
            raw_ptr = new max_operation_series(m_next_id++, sources); break;
        case operational_data_series::min_operation:
            raw_ptr = new min_operation_series(m_next_id++, sources); break;
        default:
            break;
    }

    return std::shared_ptr<data_series>(raw_ptr);
}

}