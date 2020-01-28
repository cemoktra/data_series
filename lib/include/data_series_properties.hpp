#ifndef _data_series_properties_HPP_
#define _data_series_properties_HPP_

#include <stdint.h>
#include <math.h>

namespace ds
{

class data_series_properties
{
public:
    explicit data_series_properties(uint64_t samples, double frequency, double start = 0.0)
        : m_samples(samples)
        , m_frequency(frequency)
        , m_start(start)
    {}

    explicit data_series_properties(double frequency, double start, double end, bool end_exclusive = true)
        : m_frequency(frequency)
        , m_start(start)
    {
        m_samples = static_cast<uint64_t>((end - start) * frequency);
        if (!end_exclusive)
            m_samples++;
    }

    data_series_properties() = delete;
    data_series_properties(const data_series_properties& rhs)
    {
        m_start = rhs.m_start;
        m_samples = rhs.m_samples;
        m_frequency = rhs.m_frequency;
    }

    ~data_series_properties() = default;

    bool operator==(const data_series_properties& rhs) {
        return m_samples == rhs.m_samples && m_frequency == rhs.m_frequency  && m_start == rhs.m_start;
    }
    bool operator!=(const data_series_properties& rhs) { return !(*this == rhs); }

    inline uint64_t samples() const   { return m_samples; }
    inline double   frequency() const { return m_frequency; }
    inline double   start() const     { return m_start; }
    inline double   end() const       { return sampleToTime(m_samples - 1); }

    inline double   sampleToTime(uint64_t sample) const { return m_start + (sample / m_frequency); }
    inline uint64_t timeToSample(double time) const     { return round((time - m_start) * m_frequency); }

private:
    uint64_t m_samples;
    double m_frequency;
    double m_start;
};

}

#endif