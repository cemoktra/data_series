#ifndef _DATA_COLLECTION_HPP_
#define _DATA_COLLECTION_HPP_


#include "data_series.hpp"
#include "data_series_properties.hpp"
#include "functional_data_series.hpp"
#include "operational_data_series.hpp"

#include <atomic>
#include <memory>
#include <vector>
#include <functional>
#include <optional>
#include <nlohmann/json.hpp>


namespace ds {
    class data_collection : public std::enable_shared_from_this<data_collection>
    {
    public:
        data_collection(const data_collection& rhs) = delete;
        ~data_collection();

        static std::shared_ptr<data_collection> create_collection();
        static std::shared_ptr<data_collection> from_json(nlohmann::json json);
        static std::shared_ptr<data_collection> from_json_file(const std::string& file);

        std::shared_ptr<data_series> create_static_series(const data_series_properties& props, double initial_value = std::numeric_limits<double>::quiet_NaN());
        std::shared_ptr<data_series> create_static_series(const data_series_properties& props, std::function<double(double)> func);        
        std::shared_ptr<data_series> create_static_series(std::initializer_list<double> list, double frequency, double start = 0.0);
        std::shared_ptr<data_series> create_static_series(const std::vector<double>& vector, double frequency, double start = 0.0);

        std::shared_ptr<data_series> create_polynom(const data_series_properties& props, const std::vector<double>& polynom);

        std::shared_ptr<data_series> create_functional_data_series(functional_data_series::function_type_t type, const data_series_properties& props, std::shared_ptr<data_series> source = nullptr);
        std::shared_ptr<data_series> create_operational_data_series(operational_data_series::operation_type_t type, std::vector<std::shared_ptr<data_series>> sources);

        nlohmann::json to_json(data_encoding::encoding_type_t type) const;
        void to_json_file(const std::string& file, data_encoding::encoding_type_t type) const;

        inline uint64_t id() const { return m_id; };

        std::shared_ptr<data_series> data_series_by_id(uint64_t id);

    private:        
        data_collection(uint64_t id);

        std::shared_ptr<data_series> add_raw_pointer(data_series *raw_ptr);
        void functional_data_from_json(nlohmann::json json);
        void operational_data_from_json(nlohmann::json json);

        static std::atomic_uint64_t m_next_id;
        
        uint64_t m_id;
        std::vector<std::shared_ptr<data_series>> m_datas;
    };
}

#endif