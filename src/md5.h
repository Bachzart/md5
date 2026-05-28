/**
 * @brief The MD5 hash class.
 * @author bachzart
 * @date May 28, 2026
 * @github https://github.com/bachzart
 */

#pragma once

#include <string>
#include <cstdint>

class MD5 {
    using byte = uint8_t;
    using bit32 = uint32_t;
    using bit64 = uint64_t;
public:
    std::string hexdigest();
    void update(const std::string& data);
    void update(const byte* data, size_t data_len);
    void reset();
private:
    struct MD5_CTX {
        bit32 state[4];
        bit64 count;
        byte buffer[64];

        MD5_CTX();
        void md5_update(byte* data, size_t data_len);
        void md5_transform(byte data[64]);
        void md5_final(byte digest[16]);
    };

    MD5_CTX ctx;
    byte digest[16] = {0};
};