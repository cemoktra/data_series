#ifndef _RESAMPLES_DATA_SERIES_HPP_
#define _RESAMPLES_DATA_SERIES_HPP_

#include "static_data_series.hpp"
#include <future>

namespace ds {

class resampled_data_series : public static_data_series
{
    friend class data_collection;

public:
    resampled_data_series() = delete;
    resampled_data_series(const resampled_data_series&) = delete;
    ~resampled_data_series() = default;

    size_t hash() const override;
    data_series::data_type_t type() const override;
    double operator()(double x) override;    
    nlohmann::json to_json(data_encoding::encoding_type_t type) const override;

    inline uint64_t source_id() const { return m_source ? m_source->id() : 0; }

    bool recalculate();
    std::future<bool> recalculate_async();

protected:
    explicit resampled_data_series(uint64_t id, double target_frequency, std::shared_ptr<data_series> source);
    explicit resampled_data_series(uint64_t id, uint64_t target_samples, std::shared_ptr<data_series> source);

private:
    void apply_resampling();
    void downsample();
    void upsample();
    std::pair<uint64_t, uint64_t> bucket_source_samples(uint64_t bucket);

    std::shared_ptr<data_series> m_source;
    size_t m_currentHash;
    std::vector<uint64_t> m_bucketSamples;

    double m_target_frequency;
    uint64_t m_target_samples;

    std::mutex m_recalculation_mutex;    
};

}

#endif