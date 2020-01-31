#include <gtest/gtest.h>
#include <mutable_data_series.hpp>

class mutable_data_series_test : public ::testing::Test {
public:
    mutable_data_series_test()
    {
    };

    ~mutable_data_series_test() = default;

    class derived_mutable_data_series : public ds::mutable_data_series
    {
    public:
        derived_mutable_data_series(double frequency, double start = 0.0)
            : ds::mutable_data_series(0, frequency, start)
        {
        }
    };

protected:
};

TEST_F(mutable_data_series_test, constructor_properties_correct)
{
    derived_mutable_data_series series (10.0, 1.0);
    EXPECT_EQ(10.0, series.properties().frequency());
    EXPECT_EQ(1.0, series.properties().start());
    EXPECT_EQ(0, series.properties().samples());
    EXPECT_TRUE(isnan(series.properties().end()));

    series.push_back(1.0);
    EXPECT_EQ(1, series.properties().samples());
    EXPECT_EQ(1.0, series.properties().end());
    EXPECT_FALSE(isnan(series.properties().end()));
    series.push_back({2.0, 3.0, 4.0, 5.0});
    EXPECT_EQ(5, series.properties().samples());
    EXPECT_EQ(1.4, series.properties().end());
    EXPECT_FALSE(isnan(series.properties().end()));

    EXPECT_TRUE(std::equal(series.begin(), series.end(), std::begin({1.0, 2.0, 3.0, 4.0, 5.0})));
}
