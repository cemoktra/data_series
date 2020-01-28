#ifndef _FUNCTIONAL_DATA_SERIES_HPP_
#define _FUNCTIONAL_DATA_SERIES_HPP_

#include "data_series.hpp"
#include <optional>


namespace ds 
{

class functional_data_series : public data_series {
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
        pow_function,
        // exp_function,
        // log_function,
    };

    functional_data_series(function_type_t func_type, uint64_t id, const data_series_properties& props, std::shared_ptr<data_series> source = nullptr);
    functional_data_series(const functional_data_series&) = delete;
    ~functional_data_series() = default;

    inline function_type_t function_type() const { return m_type; }
    inline uint64_t source_id() const { return m_source ? m_source->id() : 0; }

    virtual double operator()(double x) const override;

private:
std::shared_ptr<data_series> m_source;
    function_type_t m_type;    
};

class sin_data_series : public functional_data_series {
public:
    sin_data_series(uint64_t id, const data_series_properties& props, std::shared_ptr<data_series> source = nullptr);
    sin_data_series(const sin_data_series&) = delete;
    ~sin_data_series() = default;

    double operator()(double x) const override;
};

class cos_data_series : public functional_data_series {
public:
    cos_data_series(uint64_t id, const data_series_properties& props, std::shared_ptr<data_series> source = nullptr);
    cos_data_series(const cos_data_series&) = delete;
    ~cos_data_series() = default;

    double operator()(double x) const override;
};

class tan_data_series : public functional_data_series {
public:
    tan_data_series(uint64_t id, const data_series_properties& props, std::shared_ptr<data_series> source = nullptr);
    tan_data_series(const tan_data_series&) = delete;
    ~tan_data_series() = default;

    double operator()(double x) const override;
};

class asin_data_series : public functional_data_series {
public:
    asin_data_series(uint64_t id, const data_series_properties& props, std::shared_ptr<data_series> source = nullptr);
    asin_data_series(const asin_data_series&) = delete;
    ~asin_data_series() = default;

    double operator()(double x) const override;
};

class acos_data_series : public functional_data_series {
public:
    acos_data_series(uint64_t id, const data_series_properties& props, std::shared_ptr<data_series> source = nullptr);
    acos_data_series(const acos_data_series&) = delete;
    ~acos_data_series() = default;

    double operator()(double x) const override;
};

class atan_data_series : public functional_data_series {
public:
    atan_data_series(uint64_t id, const data_series_properties& props, std::shared_ptr<data_series> source = nullptr);
    atan_data_series(const atan_data_series&) = delete;
    ~atan_data_series() = default;

    double operator()(double x) const override;
};

class square_data_series : public functional_data_series {
public:
    square_data_series(uint64_t id, const data_series_properties& props, std::shared_ptr<data_series> source = nullptr);
    square_data_series(const square_data_series&) = delete;
    ~square_data_series() = default;

    double operator()(double x) const override;
};

class square_root_data_series : public functional_data_series {
public:
    square_root_data_series(uint64_t id, const data_series_properties& props, std::shared_ptr<data_series> source = nullptr);
    square_root_data_series(const square_root_data_series&) = delete;
    ~square_root_data_series() = default;

    double operator()(double x) const override;
};

class abs_data_series : public functional_data_series {
public:
    abs_data_series(uint64_t id, const data_series_properties& props, std::shared_ptr<data_series> source = nullptr);
    abs_data_series(const abs_data_series&) = delete;
    ~abs_data_series() = default;

    double operator()(double x) const override;
};

class negate_data_series : public functional_data_series {
public:
    negate_data_series(uint64_t id, const data_series_properties& props, std::shared_ptr<data_series> source = nullptr);
    negate_data_series(const negate_data_series&) = delete;
    ~negate_data_series() = default;

    double operator()(double x) const override;
};

class pow_data_series : public functional_data_series {
public:
    pow_data_series(uint64_t id, const data_series_properties& props, std::shared_ptr<data_series> source = nullptr);
    pow_data_series(const pow_data_series&) = delete;
    ~pow_data_series() = default;

    double operator()(double x) const override;
};


}

#endif