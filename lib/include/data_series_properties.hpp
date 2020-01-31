#ifndef _data_series_properties_HPP_
#define _data_series_properties_HPP_

#include <stdint.h>
#include <math.h>
#include <nlohmann/json.hpp>
#include "consts.hpp"


namespace ds
{

class data_series_properties
{
public:
    explicit data_series_properties(nlohmann::json json)
    {
        m_start = json[JSON_PROP_START];
        m_samples = json[JSON_PROP_SAMPLES];
        m_frequency = json[JSON_PROP_FREQUENCY];
    }

    explicit data_series_properties(uint64_t samples, double frequency, double start = 0.0)
        : m_samples(samples)
        , m_frequency(frequency)
        , m_start(start)
    {}

    explicit data_series_properties(double frequency, double start, double end)
        : m_frequency(frequency)
        , m_start(start)
    {
        m_samples = static_cast<uint64_t>((end - start) * frequency) + 1;
    }

    data_series_properties() = delete;
    data_series_properties(const data_series_properties& rhs)
    {
        m_start = rhs.m_start;
        m_samples = rhs.m_samples;
        m_frequency = rhs.m_frequency;
    }

    ~data_series_properties() = default;

    data_series_properties resample(uint64_t target_samples) const
    {
        auto freq = (target_samples - 1) / (end() - start());
        return data_series_properties(target_samples, freq, m_start);
    }

    data_series_properties resample(double target_frequency) const
    {
        return data_series_properties(target_frequency, start(), end());
    }

    static data_series_properties resample(const data_series_properties& props, uint64_t target_samples)
    {
        return props.resample(target_samples);
    }

    static data_series_properties resample(const data_series_properties& props, double target_frequency)
    {
        return props.resample(target_frequency);
    }

    bool operator==(const data_series_properties& rhs) const {
        return m_samples == rhs.m_samples && m_frequency == rhs.m_frequency  && m_start == rhs.m_start;
    }
    bool operator!=(const data_series_properties& rhs) const { return !(*this == rhs); }

    inline uint64_t samples() const   { return m_samples; }
    inline double   frequency() const { return m_frequency; }
    inline double   start() const     { return m_start; }
    inline double   end() const       { return m_samples == 0 ? std::numeric_limits<double>::quiet_NaN() : sampleToTime(m_samples - 1); }

    inline double   sampleToTime(uint64_t sample) const { return m_start + (sample / m_frequency); }
    inline uint64_t timeToSample(double time) const     { return round((time - m_start) * m_frequency); }

    virtual nlohmann::json to_json() const {
        return nlohmann::json{{JSON_PROP_SAMPLES, samples()}, {JSON_PROP_FREQUENCY, frequency()}, {JSON_PROP_START, start()}};
    }

    void add_samples(size_t samples) { m_samples += samples; }

private:
    uint64_t m_samples;
    double m_frequency;
    double m_start;
};

}

#endif