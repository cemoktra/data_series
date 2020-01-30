#ifndef _OPERATIONAL_DATA_SERIES_HPP_
#define _OPERATIONAL_DATA_SERIES_HPP_

#include "data_series.hpp"
#include <optional>


namespace ds 
{

class operational_data_series : public data_series {
public:
    enum operation_type_t {
        add_operation,
        sub_operation,
        mul_operation,
        div_operation,
        max_operation,
        min_operation,
        pow_operation,
    };

    operational_data_series(operation_type_t func_type, uint64_t id, std::vector<std::shared_ptr<data_series>> sources);
    operational_data_series(const operational_data_series&) = delete;
    ~operational_data_series() = default;

    inline operation_type_t operation_type() const { return m_type; }
    inline std::vector<uint64_t> source_ids() const { std::vector<uint64_t> ids; for (auto source : m_sources) ids.push_back(source->id()); return ids; }
    
    nlohmann::json to_json(data_encoding::encoding_type_t type) const override;

    double operator()(double x) const override;

    bool operator==(const operational_data_series& rhs) const;

private:
    std::vector<std::shared_ptr<data_series>> m_sources;
    std::function<double(double)> m_function;
    operation_type_t m_type;    
};

}

#endif