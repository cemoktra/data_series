#include "resampled_data_series.hpp"
#include <xtensor/xarray.hpp>
#include <xtensor-blas/xlinalg.hpp>

namespace ds {

resampled_data_series::resampled_data_series(uint64_t id, double target_frequency, std::shared_ptr<data_series> source)
    : static_data_series(id, data_series_properties::resample(source->properties(), target_frequency))
    , m_source(source)
    , m_currentHash(0)
    , m_target_frequency(target_frequency)
    , m_target_samples(0)
{
    apply_resampling();
}

resampled_data_series::resampled_data_series(uint64_t id, uint64_t target_samples, std::shared_ptr<data_series> source)
    : static_data_series(id, data_series_properties::resample(source->properties(), target_samples))
    , m_source(source)
    , m_currentHash(0)
    , m_target_frequency(std::numeric_limits<double>::quiet_NaN())
    , m_target_samples(target_samples)
{
    apply_resampling();
}

void resampled_data_series::apply_resampling()
{
    if (isnan(m_target_frequency))
        m_properties = m_source->properties().resample(m_target_samples);
    else
        m_properties = m_source->properties().resample(m_target_frequency);

    // bucket sample buffer
    if (m_bucketSamples.size() != properties().samples()) {
        m_bucketSamples.resize(properties().samples());
        m_bucketSamples[0] = 0;
        m_bucketSamples[properties().samples() - 1] = m_source->properties().samples() - 1;
    }

    // data buffer
    if (m_data.size() != properties().samples())
        m_data.resize(properties().samples());
}

size_t resampled_data_series::hash() const
{
    return m_source->hash();
}

data_series::data_type_t resampled_data_series::type() const
{
    return data_series::resampled_data;
}

double resampled_data_series::operator()(double x) 
{
    recalculate();
    return static_data_series::operator()(x);
}

nlohmann::json resampled_data_series::to_json(data_encoding::encoding_type_t) const
{
    nlohmann::json j;
    j[JSON_DATAS_ID] = id();
    j[JSON_DATAS_TYPE] = type();
    j[JSON_RESAMPLING_SOURCE] = source_id();
    if (isnan(m_target_frequency))
        j[JSON_RESAMPLING_SAMPLES] = m_target_samples;
    else
        j[JSON_RESAMPLING_FREQUENCY] = m_target_frequency;
    return j;
}

void resampled_data_series::downsample()
{    
    // Largest Triangle Three Buckets downsampling
    // https://skemman.is/bitstream/1946/15343/3/SS_MSthesis.pdf

    for (auto bucket = 0; bucket < properties().samples(); bucket++) {
        if (bucket == 0)
            m_data[bucket] = (*m_source)(m_source->properties().sampleToTime(0));
        else if (bucket == properties().samples() - 1)
            m_data[bucket] = (*m_source)(m_source->properties().sampleToTime(m_source->properties().samples() - 1));
        else {
            m_data[bucket] = std::numeric_limits<double>::quiet_NaN();

            // previous bucket sample
            auto previousSampleTime = m_source->properties().sampleToTime(m_bucketSamples[bucket - 1]);
            auto previousSample = std::make_pair(previousSampleTime, (*m_source)(previousSampleTime));

            // calculate buckets
            auto next_bucket = bucket_source_samples(bucket + 1);
            auto current_bucket = bucket_source_samples(bucket);
            
            // next bucket average
            auto avg = std::make_pair(0.0, 0.0);
            int count = 0;
            for (auto sample = next_bucket.first; sample < next_bucket.second; sample++)
            {
                auto time = m_source->properties().sampleToTime(sample);
                auto value = (*m_source)(time);
                if (!isnan(value)) {
                    avg.first += time;
                    avg.second += value;
                    count++;
                }
            }
            avg.first /= count;
            avg.second /= count;

            // find max area of all samples of current bucket
            xt::xarray<double> area_matrix = xt::ones<double>(std::vector<size_t>({3, 3}));
            area_matrix(0, 0) = previousSample.first;
            area_matrix(1, 0) = previousSample.second;
            area_matrix(0, 2) = avg.first;
            area_matrix(1, 2) = avg.second;
            
            double max = 0.0;
            m_bucketSamples[bucket] = 0;
            for (auto sample = current_bucket.first; sample < current_bucket.second; sample++)
            {
                auto time = m_source->properties().sampleToTime(sample);
                auto value = (*m_source)(time);

                area_matrix(0, 1) = time;
                area_matrix(1, 1) = value;

                auto det = fabs(xt::linalg::det(area_matrix));
                if (det > max) {
                    max = det;
                    m_bucketSamples[bucket] = sample;
                    m_data[bucket] = value;
                }
            }

            if (m_bucketSamples[bucket] == 0)
                throw std::exception();
        }
    }
}

void resampled_data_series::upsample()
{
    // TODO:
    throw std::exception();
}

bool resampled_data_series::recalculate()
{
    std::lock_guard<std::mutex> lock(m_recalculation_mutex);
    if (hash() == m_currentHash)
        return false;
    
    apply_resampling();
    if (properties().frequency() > m_source->properties().frequency())
        upsample();
    else
        downsample();
    m_currentHash = hash();        
    return true;
}

std::future<bool> resampled_data_series::recalculate_async()
{
    return std::async(std::launch::async, [this]() {
        return recalculate();
    });
}

std::pair<uint64_t, uint64_t> resampled_data_series::bucket_source_samples(uint64_t bucket)
{
    if (bucket == 0)
        return std::make_pair(0, 1);
    else if (bucket == properties().samples() - 1)
        return std::make_pair(m_source->properties().samples() - 1, m_source->properties().samples());
    else {
        auto bucketStartTime = properties().sampleToTime(bucket - 1) + 0.5 / (properties().frequency());
        auto bucketEndTime = properties().sampleToTime(bucket) + 0.5 / (properties().frequency());
        auto bucketStartSample = m_source->properties().timeToSample(bucketStartTime);
        auto bucketEndSample = m_source->properties().timeToSample(bucketEndTime);

        if (bucket == 1)
            return std::make_pair(1, bucketEndSample);
        else if (bucket == properties().samples() - 2)
            return std::make_pair(bucketStartSample, m_source->properties().samples() - 1);
        else
            return std::make_pair(bucketStartSample, bucketEndSample);
    }
}

}