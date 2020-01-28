#include "data_series_regression.hpp"
#include "data_series.hpp"
#include <exception>
#include <xtensor/xarray.hpp>
#include <xtensor-blas/xlinalg.hpp>


namespace ds {

data_series_regression::data_series_regression(uint8_t polynom_degree, bool bias)
    : m_polynom_degree(polynom_degree)
    , m_bias(bias)
{
}

void data_series_regression::analyze(std::shared_ptr<data_series> data)
{
    auto data_props = data->properties();
    uint8_t parameters = m_polynom_degree + (m_bias ? 1 : 0);
    uint64_t samples = data_props.samples();

    if (!parameters) {
        // TODO: create exception
        throw std::exception();
    }

    xt::xarray<double> X(std::vector<size_t>({samples, parameters}));
    xt::xarray<double> y(std::vector<size_t>({samples}));

    for (auto i = 0; i < samples; i++) {
        auto time = data_props.sampleToTime(i);
        auto value = (*data)(time);

        for (auto j = parameters; j > 0; j--) {
            X(i, j - 1) = pow(time, parameters - j);
        }
        y(i) = value;
    }

    xt::xarray<double> xTx = xt::linalg::dot(xt::transpose(X), X);
    xt::xarray<double> xTy = xt::linalg::dot(xt::transpose(X), y);
    xt::xarray<double> xTx_inverse = xt::linalg::inv(xTx);
    xt::xarray<double> result = xt::linalg::dot(xTx_inverse, xTy);

    m_polynom.clear();
    std::copy(result.begin(), result.end(), std::back_inserter(m_polynom));
}

}