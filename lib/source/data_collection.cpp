#include "data_collection.hpp"
#include "static_data_series.hpp"
#include <iomanip>
#include <fstream>

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

std::shared_ptr<data_collection> data_collection::from_json(nlohmann::json json)
{
    m_next_id = json[JSON_COLLECTION_ID];
    auto collection = create_collection();
        
    auto json_datas = json[JSON_COLLECTION_DATAS];
    for (auto json_data : json_datas) {
        data_series::data_type_t type = json_data[JSON_DATAS_TYPE];
        m_next_id = json_data[JSON_DATAS_ID];
        switch (type) {
            case data_series::static_data:
                collection->add_raw_pointer(new static_data_series(m_next_id++, json_data));
                break;
            case data_series::function_data:
                collection->functional_data_from_json(json_data);
                break;
            case data_series::operational_data:
                collection->operational_data_from_json(json_data);
                break;
            default:
                throw std::exception();
        }
    }

    return collection;
}

std::shared_ptr<data_collection> data_collection::from_json_file(const std::string& file)
{
    std::ifstream f(file);
    nlohmann::json j;
    f >> j;
    return from_json(j);
}

void data_collection::functional_data_from_json(nlohmann::json json)
{
    ds::functional_data_series::function_type_t function_type = json[JSON_FUNC_DATAS_TYPE];
    auto source = data_series_by_id(json[JSON_FUNC_DATAS_SOURCE]);
    create_functional_data_series(function_type, ds::data_series_properties(json[JSON_DATAS_PROPERTIES]), source);
}

void data_collection::operational_data_from_json(nlohmann::json json)
{
    std::vector<uint64_t> source_ids = json[JSON_OP_DATAS_SOURCES];
    ds::operational_data_series::operation_type_t op_type = json[JSON_OP_DATAS_TYPE];
    std::vector<std::shared_ptr<data_series>> sources;
    for (auto source_id : source_ids)
        sources.push_back(data_series_by_id(source_id));
    create_operational_data_series(op_type, sources);
}

std::shared_ptr<data_series> data_collection::create_static_series(const data_series_properties& props, double initial_value)
{
    return add_raw_pointer(new static_data_series(m_next_id++, props, initial_value));
}

std::shared_ptr<data_series> data_collection::create_static_series(const data_series_properties& props, std::function<double(double)> func)
{
    return add_raw_pointer(new static_data_series(m_next_id++, props, func));
}

std::shared_ptr<data_series> data_collection::create_static_series(std::initializer_list<double> list, double frequency, double start)
{
    return add_raw_pointer(new static_data_series(m_next_id++, list, frequency, start));
}

std::shared_ptr<data_series> data_collection::create_static_series(const std::vector<double>& vector, double frequency, double start)
{
    return add_raw_pointer(new static_data_series(m_next_id++, vector, frequency, start));
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
    return add_raw_pointer(raw_ptr);
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
    return add_raw_pointer(raw_ptr);
}

std::shared_ptr<data_series> data_collection::data_series_by_id(uint64_t id)
{
    auto it = std::find_if(m_datas.begin(), m_datas.end(), [id](std::shared_ptr<data_series> data) { return data->id() == id; });
    return it == m_datas.end() ? nullptr : *it;
}

std::shared_ptr<data_series> data_collection::add_raw_pointer(data_series *raw_ptr)
{
    if (!raw_ptr)
        return nullptr;
    auto ptr = std::shared_ptr<data_series>(raw_ptr);
    m_datas.push_back(ptr);
    return ptr;
}

nlohmann::json data_collection::to_json(data_encoding::encoding_type_t type) const
{
    nlohmann::json json_datas;
    for (auto data : m_datas)
        json_datas.push_back(data->to_json(type));
    return nlohmann::json{{JSON_COLLECTION_ID, id()}, {JSON_COLLECTION_DATAS, json_datas}};
}

void data_collection::to_json_file(const std::string& file, data_encoding::encoding_type_t type) const
{
    std::ofstream o (file);
    o << std::setw(2) << to_json(type) << std::endl;
}

}