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
    };

    operational_data_series(operation_type_t func_type, uint64_t id, std::vector<std::shared_ptr<data_series>> sources);
    operational_data_series(const operational_data_series&) = delete;
    ~operational_data_series() = default;

    inline operation_type_t operation_type() const { return m_type; }
    inline std::vector<uint64_t> source_ids() const { std::vector<uint64_t> ids; for (auto source : m_sources) ids.push_back(source->id()); return ids; }
    
    nlohmann::json to_json(data_encoding::encoding_type_t type) const override;

    bool operator==(const operational_data_series& rhs) const;

protected:
    std::vector<std::shared_ptr<data_series>> m_sources;

private:
    operation_type_t m_type;    
};

class add_operation_series : public operational_data_series {
public:
    add_operation_series(uint64_t id, std::vector<std::shared_ptr<data_series>> sources);
    add_operation_series(const add_operation_series&) = delete;
    ~add_operation_series() = default;

    double operator()(double x) const override;
};

class sub_operation_series : public operational_data_series {
public:
    sub_operation_series(uint64_t id, std::vector<std::shared_ptr<data_series>> sources);
    sub_operation_series(const sub_operation_series&) = delete;
    ~sub_operation_series() = default;

    double operator()(double x) const override;
};


class mul_operation_series : public operational_data_series {
public:
    mul_operation_series(uint64_t id, std::vector<std::shared_ptr<data_series>> sources);
    mul_operation_series(const mul_operation_series&) = delete;
    ~mul_operation_series() = default;

    double operator()(double x) const override;
};


class div_operation_series : public operational_data_series {
public:
    div_operation_series(uint64_t id, std::vector<std::shared_ptr<data_series>> sources);
    div_operation_series(const div_operation_series&) = delete;
    ~div_operation_series() = default;

    double operator()(double x) const override;
};


class min_operation_series : public operational_data_series {
public:
    min_operation_series(uint64_t id, std::vector<std::shared_ptr<data_series>> sources);
    min_operation_series(const min_operation_series&) = delete;
    ~min_operation_series() = default;

    double operator()(double x) const override;
};


class max_operation_series : public operational_data_series {
public:
    max_operation_series(uint64_t id, std::vector<std::shared_ptr<data_series>> sources);
    max_operation_series(const max_operation_series&) = delete;
    ~max_operation_series() = default;

    double operator()(double x) const override;
};


}

#endif