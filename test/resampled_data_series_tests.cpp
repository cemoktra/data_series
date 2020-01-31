#include <gtest/gtest.h>
#include <resampled_data_series.hpp>
#include <functional_data_series.hpp>

class resampled_data_series_test : public ::testing::Test {
public:
    resampled_data_series_test()
    {
    };

    ~resampled_data_series_test() = default;

    class derived_resampled_data_series : public ds::resampled_data_series
    {
    public:
        derived_resampled_data_series(uint64_t target_samples, std::shared_ptr<ds::data_series> source)
            : ds::resampled_data_series(0, target_samples, source)
        {
        }

        derived_resampled_data_series(double target_frequency, std::shared_ptr<ds::data_series> source)
            : ds::resampled_data_series(0, target_frequency, source)
        {
        }
    };

    class derived_static_data_series : public ds::static_data_series
    {
    public:
        derived_static_data_series(std::initializer_list<double> list, double frequency, double start = 0.0)
            : ds::static_data_series(0, list, frequency, start)
        {
        }

        derived_static_data_series(const std::vector<double>& vector, double frequency, double start = 0.0)
            : ds::static_data_series(0, vector, frequency, start)
        {
        }
    };

    class derived_functional_data_series : public ds::functional_data_series
    {
    public:
        derived_functional_data_series(function_type_t func_type, const ds::data_series_properties& props)
            : ds::functional_data_series(func_type, 0, props, nullptr)
        {
        }
    };
};

TEST_F(resampled_data_series_test, downsample_recalculate_when_iterating)
{
    std::shared_ptr<ds::data_series> source (new derived_static_data_series({0.0, 0.5, 1.0, 1.5, 2.0, 1.5, 1.0, 0.5, 0.0, -0.5, -1.0, -1.5, -2.0, -1.5, -1.0, -0.5, -0.0}, 16));
    derived_resampled_data_series resampled(uint64_t(5), source);

    int count = 0;
    for (auto x : resampled)
        count++;
    EXPECT_EQ(5, count);
    EXPECT_FALSE(resampled.recalculate());
    EXPECT_EQ(0.0, resampled(0.0));
    EXPECT_EQ(0.0, resampled(1.0));
}

TEST_F(resampled_data_series_test, downsample_async_recalculate_only_once)
{
    std::shared_ptr<ds::data_series> source (new derived_static_data_series({0.0, 0.5, 1.0, 1.5, 2.0, 1.5, 1.0, 0.5, 0.0, -0.5, -1.0, -1.5, -2.0, -1.5, -1.0, -0.5, -0.0}, 16));
    derived_resampled_data_series resampled(uint64_t(5), source);
    auto recalc1 = resampled.recalculate_async();
    auto recalc2 = resampled.recalculate_async();
    EXPECT_TRUE(recalc1.get());
    EXPECT_FALSE(recalc2.get());
    EXPECT_EQ(0.0, resampled(0.0));
    EXPECT_EQ(0.0, resampled(1.0));
    EXPECT_EQ(5, resampled.properties().samples());
}

TEST_F(resampled_data_series_test, downsample_sinus)
{
    std::shared_ptr<ds::data_series> sinus(new derived_functional_data_series(ds::functional_data_series::sin_function, ds::data_series_properties(100.0, 0.0, 3.0)));
    derived_resampled_data_series resampled(10.0, sinus);

    resampled.recalculate();

    for (auto i = 0; i < resampled.properties().samples(); i++) {
        auto time = resampled.properties().sampleToTime(i);
        auto value = resampled(time);
        auto real_value = sin(time);
        EXPECT_NEAR(value, real_value, 0.02);
    }
}

TEST_F(resampled_data_series_test, upsample_sinus)
{
    std::shared_ptr<ds::data_series> sinus(new derived_functional_data_series(ds::functional_data_series::sin_function, ds::data_series_properties(10.0, 0.0, 3.0)));
    derived_resampled_data_series resampled(100.0, sinus);

    resampled.recalculate();

    for (auto i = 0; i < resampled.properties().samples(); i++) {
        auto time = resampled.properties().sampleToTime(i);
        auto value = resampled(time);
        auto real_value = sin(time);
        EXPECT_NEAR(value, real_value, 0.02);
    }
}
