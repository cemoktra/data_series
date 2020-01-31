#include <gtest/gtest.h>
#include <data_series_properties.hpp>

class data_series_properties_test : public ::testing::Test {
public:
    data_series_properties_test()
    {
    };

    ~data_series_properties_test() = default;

protected:
};

TEST_F(data_series_properties_test, constructor_with_samples)
{
    auto props = ds::data_series_properties(uint64_t(100), 10.0, -2.0);
    EXPECT_EQ(100, props.samples());
    EXPECT_EQ(-2.0, props.start());
    EXPECT_EQ(7.9, props.end());
}

TEST_F(data_series_properties_test, constructor_with_start_and_end)
{
    auto props = ds::data_series_properties(10.0, -3.0, 7.0);
    EXPECT_EQ(101, props.samples());
    EXPECT_EQ(-3.0, props.start());
    EXPECT_EQ(7.0, props.end());
}

TEST_F(data_series_properties_test, copy_constructor)
{
    auto props = ds::data_series_properties(uint64_t(100), 10.0, -2.0);
    auto copy(props);
    EXPECT_EQ(100, copy.samples());
    EXPECT_EQ(-2.0, copy.start());
    EXPECT_EQ(7.9, copy.end());
    EXPECT_TRUE(props == copy);
}

TEST_F(data_series_properties_test, sample_to_time)
{
    auto props = ds::data_series_properties(10.0, -3.0, 7.0);
    EXPECT_EQ(-3.0, props.sampleToTime(0));
    EXPECT_EQ(-2.9, props.sampleToTime(1));
    EXPECT_EQ(-2.0, props.sampleToTime(10));
    EXPECT_EQ(7.0, props.sampleToTime(100));
}

TEST_F(data_series_properties_test, time_to_sample)
{
    auto props = ds::data_series_properties(10.0, -3.0, 7.0);
    EXPECT_EQ(0, props.timeToSample(-3.0));
    EXPECT_EQ(1, props.timeToSample(-2.9));
    EXPECT_EQ(2, props.timeToSample(-2.81));
    EXPECT_EQ(2, props.timeToSample(-2.79));
    EXPECT_EQ(3, props.timeToSample(-2.75));
    EXPECT_EQ(10, props.timeToSample(-2.0));
    EXPECT_EQ(100, props.timeToSample(7.0));
}

TEST_F(data_series_properties_test, resample_down_to_samples)
{
    auto props = ds::data_series_properties(101, 100);
    auto down_sampled = props.resample(uint64_t(51));
    EXPECT_EQ(props.start(), down_sampled.start());
    EXPECT_EQ(props.end(), down_sampled.end());
    EXPECT_EQ(down_sampled.frequency(), 50.0);
}

TEST_F(data_series_properties_test, resample_down_to_frequency)
{
    {
        auto props = ds::data_series_properties(101, 100);
        auto down_sampled = props.resample(50.0);
        EXPECT_EQ(props.start(), down_sampled.start());
        EXPECT_EQ(props.end(), down_sampled.end());
        EXPECT_EQ(down_sampled.samples(), 51);
    }
    {
        auto props = ds::data_series_properties(100.0, 0.0, 1.0);
        auto down_sampled = props.resample(50.0);
        EXPECT_EQ(props.start(), down_sampled.start());
        EXPECT_EQ(props.end(), down_sampled.end());
        EXPECT_EQ(down_sampled.samples(), 51);
    }
}

TEST_F(data_series_properties_test, resample_up_to_samples)
{
    auto props = ds::data_series_properties(51, 50.0);
    auto down_sampled = props.resample(uint64_t(101));
    EXPECT_EQ(props.start(), down_sampled.start());
    EXPECT_EQ(props.end(), down_sampled.end());
    EXPECT_EQ(down_sampled.frequency(), 100.0);
}

TEST_F(data_series_properties_test, resample_up_to_frequency)
{
    {
        auto props = ds::data_series_properties(51, 50);
        auto down_sampled = props.resample(100.0);
        EXPECT_EQ(props.start(), down_sampled.start());
        EXPECT_EQ(props.end(), down_sampled.end());
        EXPECT_EQ(down_sampled.samples(), 101);
    }
    {
        auto props = ds::data_series_properties(50.0, 0.0, 1.0);
        auto down_sampled = props.resample(100.0);
        EXPECT_EQ(props.start(), down_sampled.start());
        EXPECT_EQ(props.end(), down_sampled.end());
        EXPECT_EQ(down_sampled.samples(), 101);
    }
}