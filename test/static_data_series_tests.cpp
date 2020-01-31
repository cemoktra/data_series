#include <gtest/gtest.h>
#include <static_data_series.hpp>

class static_data_series_test : public ::testing::Test {
public:
    static_data_series_test()
    {
    };

    ~static_data_series_test() = default;

    class derived_static_data_series : public ds::static_data_series
    {
    public:
        derived_static_data_series(const ds::data_series_properties& props, double initial_value = std::numeric_limits<double>::quiet_NaN())
            : ds::static_data_series(0, props, initial_value)
        {
        }

        derived_static_data_series(const ds::data_series_properties& props, std::function<double(double)> func)
            : ds::static_data_series(0, props, func)
        {
        }

        derived_static_data_series(std::initializer_list<double> list, double frequency, double start = 0.0)
            : ds::static_data_series(0, list, frequency, start)
        {
        }

        derived_static_data_series(const std::vector<double>& vector, double frequency, double start = 0.0)
            : ds::static_data_series(0, vector, frequency, start)
        {
        }
    };

protected:
};

TEST_F(static_data_series_test, constructor_single_value_param)
{
    derived_static_data_series series (ds::data_series_properties(1, 1.0), 2.5);
    for (auto value : series)
        EXPECT_EQ(2.5, value);
}

TEST_F(static_data_series_test, constructor_function_param)
{
    derived_static_data_series series (ds::data_series_properties(3, 1.0), [](double x) { return x; });
    double expected = 0.0;
    for (auto value : series) {
        EXPECT_EQ(expected, value);
        expected += 1.0;
    }
}

TEST_F(static_data_series_test, constructor_initializer_list)
{
    derived_static_data_series series ({ 2.0, 4.0, 6.0 }, 1.0, 0.0);
    double expected = 2.0;
    for (auto value : series) {
        EXPECT_EQ(expected, value);
        expected += 2.0;
    }
}

TEST_F(static_data_series_test, constructor_vector)
{
    std::vector<double> input = { 3.0, 6.0, 9.0 };
    derived_static_data_series series (input, 1.0, 0.0);

    EXPECT_TRUE(std::equal(input.begin(), input.end(), series.begin()));
}

TEST_F(static_data_series_test, constructor_parameters)
{
    derived_static_data_series series (ds::data_series_properties(2.0, -1.0, 1.0), 2.5);
    
    EXPECT_EQ(0, series.id());
    EXPECT_EQ(ds::data_series::static_data, series.type());
    EXPECT_EQ(-1.0, series.min());
    EXPECT_EQ(1.0, series.max());
    EXPECT_EQ(0.5, series.step());
}