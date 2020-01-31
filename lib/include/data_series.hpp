#ifndef _DATA_SERIES_HPP_
#define _DATA_SERIES_HPP_

#include "data_series_properties.hpp"
#include "data_series_iterator.hpp"
#include "data_encoding.hpp"

#include <stdint.h>
#include <memory>


namespace ds {
    class data_series {
    public:
        enum data_type_t {
            static_data,
            function_data,
            operational_data,
            mutable_data,
        };

        ~data_series() = default;
        data_series(const data_series& rhs);

        inline const data_series_properties& properties() const { return m_properties; }

        inline data_type_t type() const { return m_type; };
        inline uint64_t id() const      { return m_id; };
        inline double min() const       { return m_properties.start(); }
        inline double max() const       { return m_properties.sampleToTime(m_properties.samples() - 1); }
        inline double step() const      { return 1.0 / m_properties.frequency(); }

        virtual double operator()(double x) const = 0;

        virtual nlohmann::json to_json(data_encoding::encoding_type_t type) const {
            return nlohmann::json{{JSON_DATAS_ID, id()}, {JSON_DATAS_TYPE, m_type}, {JSON_DATAS_PROPERTIES, properties().to_json()}};
        }

        data_series_iterator begin() { return data_series_iterator(0, this); }
        data_series_iterator end() { return data_series_iterator(m_properties.samples(), this); }

        bool operator==(const data_series& rhs) const {
            return m_type == rhs.m_type && m_id == rhs.m_id && m_properties == rhs.m_properties;
        }

    protected:
        data_series(uint64_t id, const data_series_properties& props, data_type_t type)
            : m_id(id)
            , m_properties(props)
            , m_type(type)
        {}

        data_series_properties m_properties;

    private:
        uint64_t m_id;
        data_type_t m_type;
    };
}

#endif