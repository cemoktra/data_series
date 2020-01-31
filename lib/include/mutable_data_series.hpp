#ifndef _MUTABLE_DATA_SERIES_HPP_
#define _MUTABLE_DATA_SERIES_HPP_

#include "static_data_series.hpp"

namespace ds {

class mutable_data_series : public static_data_series
{
    friend class data_collection;

public:
    mutable_data_series() = delete;
    mutable_data_series(const mutable_data_series&) = delete;
    ~mutable_data_series() = default;

    data_series::data_type_t type() const override;

    void push_back(double value);
    void push_back(std::vector<double> values);

protected:
    mutable_data_series(uint64_t id, double frequency, double start = 0.0);
    mutable_data_series(uint64_t id, nlohmann::json json);
};

}

#endif