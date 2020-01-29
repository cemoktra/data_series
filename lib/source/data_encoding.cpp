#include "data_encoding.hpp"

#include <nlohmann/json.hpp>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <zlib.h>
#include <sstream>
#include <iomanip>
#include <iostream>

namespace ds {

std::string data_encoding::encode(encoding_type_t type, const std::vector<double>& datas)
{
    switch (type)
    {
    case msgpack:
        return encode_msgpack(datas);
    default:
        return encode_base64(datas);
    }
}

std::vector<double> data_encoding::decode(encoding_type_t type, const std::string& encoded_datas)
{
    switch (type)
    {
    case msgpack:
        return decode_msgpack(encoded_datas);
    default:
        return decode_base64(encoded_datas);
    }
}


std::string data_encoding::encode_base64(const std::vector<double>& datas)
{
    BIO *bio, *b64;
	BUF_MEM *bufferPtr;

    b64 = BIO_new(BIO_f_base64());
	bio = BIO_new(BIO_s_mem());
	bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(bio, datas.data(), datas.size() * sizeof(double));
    BIO_flush(bio);
	BIO_get_mem_ptr(bio, &bufferPtr);
	BIO_set_close(bio, BIO_NOCLOSE);
	BIO_free_all(bio);
    
    return data_encoding::compress(std::vector<uint8_t>(bufferPtr->data, bufferPtr->data + bufferPtr->length));
}

std::string data_encoding::encode_msgpack(const std::vector<double>& datas)
{
    return data_encoding::compress(nlohmann::json::to_msgpack(nlohmann::json(datas)));
}

std::vector<double> data_encoding::decode_base64(const std::string& encoded_datas)
{
    BIO *bio, *b64;

    auto decompressed = data_encoding::decompress(encoded_datas);
    auto padding = 0;
    padding += decompressed[decompressed.size() - 1] == 61 ? 1 : 0;
    padding += decompressed[decompressed.size() - 2] == 61 ? 1 : 0;

    auto decodeLength = (3 * decompressed.size() / 4) - padding;
    auto double_values = decodeLength / sizeof(double);

    std::vector<double> decoded(double_values);

    bio = BIO_new_mem_buf(decompressed.data(), decompressed.size());
	b64 = BIO_new(BIO_f_base64());
	bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
	auto res = BIO_read(bio, decoded.data(), decodeLength);
	BIO_free_all(bio);

    return decoded;
}

std::vector<double> data_encoding::decode_msgpack(const std::string& encoded_datas)
{
    std::vector<double> decoded;
    auto decompressed = data_encoding::decompress(encoded_datas);
    auto json = nlohmann::json::from_msgpack(decompressed);
    std::copy(json.begin(), json.end(), std::back_inserter(decoded));
    return decoded;
}

std::string data_encoding::compress(const std::vector<uint8_t>& bytes)
{
    std::vector<uint8_t> compressed_bytes(bytes.size());

    z_stream defstream;
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    defstream.avail_in = bytes.size();
    defstream.next_in = (Bytef*) bytes.data();
    defstream.avail_out = compressed_bytes.size();
    defstream.next_out = (Bytef*) compressed_bytes.data();

    deflateInit(&defstream, Z_BEST_COMPRESSION);
    deflate(&defstream, Z_FINISH);
    deflateEnd(&defstream);

    compressed_bytes.resize(defstream.total_out);

    std::ostringstream oss;
    for (uint16_t byte : compressed_bytes)
        oss << std::hex << std::setfill('0') << std::setw(2) << byte;
    return oss.str();
}

std::vector<uint8_t> data_encoding::decompress(const std::string& message)
{
    std::vector<uint8_t> compressed_bytes;
    std::vector<uint8_t> decompressed_bytes;
    for (auto i = 0; i < message.length(); i+= 2)
    {
        int value;
        std::istringstream(message.substr(i, 2)) >> std::hex >> value;
        compressed_bytes.push_back(value);
    }

    const uint16_t buffer_size = 1024;
    uint8_t buffer[buffer_size];

    z_stream infstream;
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;
    infstream.avail_in = compressed_bytes.size();
    infstream.next_in = (Bytef *) compressed_bytes.data();
    infstream.avail_out = buffer_size;
    infstream.next_out = (Bytef *) buffer;
     
    inflateInit(&infstream);
    auto last_total_out = infstream.total_out;
    while (infstream.avail_in > 0) {
        inflate(&infstream, Z_NO_FLUSH);
        std::copy (buffer, buffer + infstream.total_out - last_total_out, std::back_inserter(decompressed_bytes));
        last_total_out = infstream.total_out;
        infstream.avail_out = buffer_size;
        infstream.next_out = (Bytef *) buffer;
    }
    inflateEnd(&infstream);
    return decompressed_bytes;

}

}