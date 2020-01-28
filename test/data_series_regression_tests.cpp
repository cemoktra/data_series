#include <gtest/gtest.h>
#include <data_collection.hpp>
#include <data_series_regression.hpp>
#include <random>
#include <iostream>

class data_series_regression_test : public ::testing::Test {
public:
    data_series_regression_test()
        : m_collection(ds::data_collection::create_collection())
    {};

    ~data_series_regression_test() = default;

protected:
    std::shared_ptr<ds::data_collection> m_collection;
};


TEST_F(data_series_regression_test, polynom_1)
{
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<> d{0.0, 0.2};

    std::vector<double> polynom = {2.0, 5.0};
    auto props = ds::data_series_properties(1001, 100.0);

    auto polynom_series = m_collection->create_polynom(props, polynom);
    auto noise_series = m_collection->create_static_series(props, [&d, &gen](double x) { return d(gen); });
    auto polynom_noise_series = m_collection->create_operational_data_series(ds::operational_data_series::add_operation, {polynom_series, noise_series});
    
    ds::data_series_regression regression(1);
    regression.analyze(polynom_noise_series);
    auto regressed_polynom = regression.polynom();

    for (auto i = 0; i < regressed_polynom.size(); i++)
        EXPECT_NEAR(polynom[i], regressed_polynom[i], 0.05);
}

TEST_F(data_series_regression_test, polynom_2)
{
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<> d{0.0, 0.2};

    std::vector<double> polynom = {1.0, -2.0, 5.0};
    auto props = ds::data_series_properties(1001, 100.0);

    auto polynom_series = m_collection->create_polynom(props, polynom);
    auto noise_series = m_collection->create_static_series(props, [&d, &gen](double x) { return d(gen); });
    auto polynom_noise_series = m_collection->create_operational_data_series(ds::operational_data_series::add_operation, {polynom_series, noise_series});
    
    ds::data_series_regression regression(2);
    regression.analyze(polynom_noise_series);
    auto regressed_polynom = regression.polynom();

    for (auto i = 0; i < regressed_polynom.size(); i++)
        EXPECT_NEAR(polynom[i], regressed_polynom[i], 0.05);
}