#ifndef _CONSTS_HPP_
#define _CONSTS_HPP_

#include <string>

namespace ds {
    static const std::string JSON_PROP_START = "start";
    static const std::string JSON_PROP_SAMPLES = "samples";
    static const std::string JSON_PROP_FREQUENCY = "frequency";
    static const std::string JSON_COLLECTION_ID = "id";
    static const std::string JSON_COLLECTION_DATAS = "datas";
    static const std::string JSON_DATAS_ID = "id";
    static const std::string JSON_DATAS_TYPE = "type";
    static const std::string JSON_DATAS_PROPERTIES = "properties";
    static const std::string JSON_STATIC_DATAS_CONST = "const";
    static const std::string JSON_STATIC_DATAS_VECTOR = "vec";
    static const std::string JSON_STATIC_DATAS_ENCODING = "encoding";
    static const std::string JSON_STATIC_DATAS_ENCODED = "encoded";
    static const std::string JSON_FUNC_DATAS_TYPE = "func_type";
    static const std::string JSON_FUNC_DATAS_SOURCE = "source";
    static const std::string JSON_OP_DATAS_TYPE = "op_type";
    static const std::string JSON_OP_DATAS_SOURCES = "sources";
    static const std::string JSON_RESAMPLING_FREQUENCY = "target_frequency";
    static const std::string JSON_RESAMPLING_SAMPLES = "target_sampples";
    static const std::string JSON_RESAMPLING_SOURCE = "source";

}

#endif