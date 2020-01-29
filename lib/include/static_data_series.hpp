#ifndef _STATIC_DATA_SERIES_HPP_
#define _STATIC_DATA_SERIES_HPP_

#include "data_series.hpp"

#include <memory>
#include <vector>
#include <functional>
#include <iostream>


namespace ds {
    class static_data_series : public data_series, public std::enable_shared_from_this<static_data_series>
    {
        friend class data_collection;

    public:
        static_data_series() = delete;
        static_data_series(const static_data_series&) = delete;
        ~static_data_series() = default;

        double operator()(double x) const override;

        virtual nlohmann::json to_json(data_encoding::encoding_type_t type) const;

        bool operator==(const static_data_series& rhs) const;

    protected:
        static_data_series(uint64_t id, const data_series_properties& props, double initial_value = std::numeric_limits<double>::quiet_NaN());        
        static_data_series(uint64_t id, const data_series_properties& props, std::function<double(double)> func);
        static_data_series(uint64_t id, std::initializer_list<double> list, double frequency, double start = 0.0);
        static_data_series(uint64_t id, const std::vector<double>& vector, double frequency, double start = 0.0);
        static_data_series(uint64_t id, nlohmann::json json);
        std::vector<double> m_data;
    };
}

#endif