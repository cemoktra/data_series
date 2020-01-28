#ifndef _DATA_SERIES_REGRESSION_HPP_
#define _DATA_SERIES_REGRESSION_HPP_

#include <stdint.h>
#include <memory>
#include <vector>


namespace ds {

class data_series;

class data_series_regression {
public:
    data_series_regression() = delete;
    data_series_regression(uint8_t polynom_degree, bool bias = true);
    data_series_regression(const data_series_regression&) = delete;
    ~data_series_regression() = default;

    void analyze(std::shared_ptr<data_series> data);

    inline uint8_t polynom_degree() const { return m_polynom_degree; }

    inline std::vector<double> polynom() const { return m_polynom; }

private:
    std::vector<double> m_polynom;

    uint8_t m_polynom_degree;
    bool m_bias;    
};

}

#endif