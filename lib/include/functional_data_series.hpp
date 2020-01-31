#ifndef _FUNCTIONAL_DATA_SERIES_HPP_
#define _FUNCTIONAL_DATA_SERIES_HPP_

#include "data_series.hpp"
#include <optional>
#include <functional>


namespace ds 
{

class functional_data_series : public data_series {
    friend class data_collection;
    
public:
    enum function_type_t {
        sin_function,
        cos_function,
        tan_function,
        asin_function,
        acos_function,
        atan_function,
        square_function,
        square_root_function,
        abs_function,
        negate_function,
        exp_function,
        log_function,
        ident_function,
    };

    functional_data_series() = delete;
    functional_data_series(const functional_data_series&) = delete;
    ~functional_data_series() = default;

    inline function_type_t function_type() const { return m_type; }
    inline uint64_t source_id() const { return m_source ? m_source->id() : 0; }

    size_t hash() const override;
    virtual data_series::data_type_t type() const override;
    double operator()(double x) override;
    nlohmann::json to_json(data_encoding::encoding_type_t type) const override;

    bool operator==(const functional_data_series& rhs) const;

protected:
    functional_data_series(function_type_t func_type, uint64_t id, const data_series_properties& props, std::shared_ptr<data_series> source = nullptr);

    std::function<double(double)> m_function;
    std::shared_ptr<data_series> m_source;
    function_type_t m_type;
};

}

#endif