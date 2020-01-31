#include "operational_data_series.hpp"
#include <algorithm>

namespace ds {


operational_data_series::operational_data_series(operation_type_t op_type, uint64_t id, std::vector<std::shared_ptr<data_series>> sources)
    : data_series(id, sources[0]->properties())
    , m_type(op_type)
    , m_sources(sources)
{
    auto current_props = sources[0]->properties();
    
    for (auto source : sources) {
        if (current_props != source->properties()) {
            // TODO: custom  exception (all operands need same properties)
            throw std::exception();
        }
    }

    switch (op_type)
    {
        case add_operation: 
            m_function = [this](double x) {
                double res = (**m_sources.begin())(x);
                for (auto it = std::next(m_sources.begin()); it != m_sources.end(); ++it)
                    res += (**it)(x);
                return res;
            };
            break;
        case sub_operation:
            m_function = [this](double x) {
                double res = (**m_sources.begin())(x);
                for (auto it = std::next(m_sources.begin()); it != m_sources.end(); ++it)
                    res -= (**it)(x);
                return res;
            };
            break;
        case mul_operation: 
            m_function = [this](double x) {
                double res = (**m_sources.begin())(x);
                for (auto it = std::next(m_sources.begin()); it != m_sources.end(); ++it)
                    res *= (**it)(x);
                return res;
            };
            break;
        case div_operation:
            m_function = [this](double x) {
                double res = (**m_sources.begin())(x);
                for (auto it = std::next(m_sources.begin()); it != m_sources.end(); ++it)
                    res /= (**it)(x);
                return res;
            };
            break;        
        case max_operation:
            m_function = [this](double x) {
                double res = (**m_sources.begin())(x);
                for (auto it = std::next(m_sources.begin()); it != m_sources.end(); ++it)
                    res = std::max(res, (**it)(x));
                return res;
            };
            break;        
        case min_operation:
            m_function = [this](double x) {
                double res = (**m_sources.begin())(x);
                for (auto it = std::next(m_sources.begin()); it != m_sources.end(); ++it)
                    res = std::min(res, (**it)(x));
                return res;
            };
            break;        
        case pow_operation: 
            m_function = [this](double x) {
                if (m_sources.size() != 2)
                    // TODO: custom exception
                    throw std::exception();
                return pow((*m_sources[0])(x), (*m_sources[1])(x));
            };
            break;
        default:
            // TODO: custom  exception (unknoqn op type)
            throw std::exception();
    }    
}

size_t operational_data_series::hash() const
{
    size_t hash_value = std::hash<int>()(static_cast<int>(m_type + 1));
    for (auto source : m_sources)
        hash_value = hash_value ^ (source->hash() << 1);
    return hash_value;
}

data_series::data_type_t operational_data_series::type() const
{
    return data_series::operational_data;
}

double operational_data_series::operator()(double x)
{
    return m_function(x);
}


nlohmann::json operational_data_series::to_json(data_encoding::encoding_type_t type) const  {
    auto j = data_series::to_json(type);
    j[JSON_OP_DATAS_TYPE] = operation_type();
    nlohmann::json json_sourceids;
    for (auto source_id : source_ids())
        json_sourceids.push_back(source_id);
    j[JSON_OP_DATAS_SOURCES] = json_sourceids;
    return j;
}

bool operational_data_series::operator==(const operational_data_series& rhs) const
{
    return data_series::operator==(rhs) && m_type == rhs.m_type && std::equal(m_sources.begin(), m_sources.end(), rhs.m_sources.begin());
}

}