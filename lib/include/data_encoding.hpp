#ifndef _DATA_ENCODING_HPP_
#define _DATA_ENCODING_HPP_

#include <vector>
#include <string>

namespace ds {

class data_encoding
{
public:
    enum encoding_type_t {
        base64,
        msgpack
    };

    data_encoding() = delete;
    ~data_encoding() = delete;

    static std::string encode(encoding_type_t type, const std::vector<double>& datas);
    static std::vector<double> decode(encoding_type_t type, const std::string& encoded_datas);
    
private:
    static std::string encode_base64(const std::vector<double>& datas);
    static std::string encode_msgpack(const std::vector<double>& datas);

    static std::vector<double> decode_base64(const std::string& encoded_datas);
    static std::vector<double> decode_msgpack(const std::string& encoded_datas);

    static std::string compress(const std::vector<uint8_t>& bytes);
    static std::vector<uint8_t> decompress(const std::string& message);

};

}

#endif