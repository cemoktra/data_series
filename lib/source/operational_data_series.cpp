#include "operational_data_series.hpp"
#include <algorithm>

namespace ds {


operational_data_series::operational_data_series(operation_type_t op_type, uint64_t id, std::vector<std::shared_ptr<data_series>> sources)
    : data_series(id, sources[0]->properties(), data_type_t::operational_data)
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


add_operation_series::add_operation_series(uint64_t id, std::vector<std::shared_ptr<data_series>> sources)
    : operational_data_series(add_operation, id, sources)
{
}

double add_operation_series::operator()(double x) const
{
    double res = (**m_sources.begin())(x);

    for (auto it = std::next(m_sources.begin()); it != m_sources.end(); ++it)
        res += (**it)(x);
    return res;
}

sub_operation_series::sub_operation_series(uint64_t id, std::vector<std::shared_ptr<data_series>> sources)
    : operational_data_series(sub_operation, id, sources)
{
}

double sub_operation_series::operator()(double x) const
{
    double res = (**m_sources.begin())(x);

    for (auto it = std::next(m_sources.begin()); it != m_sources.end(); ++it)
        res -= (**it)(x);
    return res;
}

mul_operation_series::mul_operation_series(uint64_t id, std::vector<std::shared_ptr<data_series>> sources)
    : operational_data_series(mul_operation, id, sources)
{
}

double mul_operation_series::operator()(double x) const
{
    double res = (**m_sources.begin())(x);

    for (auto it = std::next(m_sources.begin()); it != m_sources.end(); ++it)
        res *= (**it)(x);
    return res;
}

div_operation_series::div_operation_series(uint64_t id, std::vector<std::shared_ptr<data_series>> sources)
    : operational_data_series(div_operation, id, sources)
{
}

double div_operation_series::operator()(double x) const
{
    double res = (**m_sources.begin())(x);

    for (auto it = std::next(m_sources.begin()); it != m_sources.end(); ++it)
        res /= (**it)(x);
    return res;
}

max_operation_series::max_operation_series(uint64_t id, std::vector<std::shared_ptr<data_series>> sources)
    : operational_data_series(max_operation, id, sources)
{
}

double max_operation_series::operator()(double x) const
{
    double res = (**m_sources.begin())(x);

    for (auto it = std::next(m_sources.begin()); it != m_sources.end(); ++it)
        res = std::max(res, (**it)(x));
    return res;
}

min_operation_series::min_operation_series(uint64_t id, std::vector<std::shared_ptr<data_series>> sources)
    : operational_data_series(min_operation, id, sources)
{
}

double min_operation_series::operator()(double x) const
{
    double res = (**m_sources.begin())(x);

    for (auto it = std::next(m_sources.begin()); it != m_sources.end(); ++it)
        res = std::min(res, (**it)(x));
    return res;
}


}