/**
 * @brief The implementation of the MD5 hash class.
 * @author bachzart
 * @date May 28, 2026
 * @github https://github.com/bachzart
 */

#include "md5.h"

#include <algorithm>

namespace {

// Constants
constexpr uint32_t T[64] = {
    0xD76AA478, 0xE8C7B756, 0x242070DB, 0xC1BDCEEE,
    0xF57C0FAF, 0x4787C62A, 0xA8304613, 0xFD469501,
    0x698098D8, 0x8B44F7AF, 0xFFFF5BB1, 0x895CD7BE,
    0x6B901122, 0xFD987193, 0xA679438E, 0x49B40821,
    0xF61E2562, 0xC040B340, 0x265E5A51, 0xE9B6C7AA,
    0xD62F105D, 0x02441453, 0xD8A1E681, 0xE7D3FBC8,
    0x21E1CDE6, 0xC33707D6, 0xF4D50D87, 0x455A14ED,
    0xA9E3E905, 0xFCEFA3F8, 0x676F02D9, 0x8D2A4C8A,
    0xFFFA3942, 0x8771F681, 0x6D9D6122, 0xFDE5380C,
    0xA4BEEA44, 0x4BDECFA9, 0xF6BB4B60, 0xBEBFBC70,
    0x289B7EC6, 0xEAA127FA, 0xD4EF3085, 0x04881D05,
    0xD9D4D039, 0xE6DB99E5, 0x1FA27CF8, 0xC4AC5665,
    0xF4292244, 0x432AFF97, 0xAB9423A7, 0xFC93A039,
    0x655B59C3, 0x8F0CCC92, 0xFFEFF47D, 0x85845DD1,
    0x6FA87E4F, 0xFE2CE6E0, 0xA3014314, 0x4E0811A1,
    0xF7537E82, 0xBD3AF235, 0x2AD7D2BB, 0xEB86D391,
};
constexpr uint8_t S[64] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

/**
 * @brief Rotate left.
 * @param x The number to rotate.
 * @param n The number of bits to rotate.
 * @return The rotated number.
 */
constexpr uint32_t rotate_left(uint32_t x, uint8_t n) {
    return (x << n) | (x >> (32 - n));
}

/**
 * @brief Calculate the F function.
 * @param x The first number.
 * @param y The second number.
 * @param z The third number.
 * @return The result of the F function.
 */
constexpr uint32_t F(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) | (~x & z);
}

/**
 * @brief Calculate the G function.
 * @param x The first number.
 * @param y The second number.
 * @param z The third number.
 * @return The result of the G function.
 */
constexpr uint32_t G(uint32_t x, uint32_t y, uint32_t z) {
    return (x & z) | (y & ~z);
}

/**
 * @brief Calculate the H function.
 * @param x The first number.
 * @param y The second number.
 * @param z The third number.
 * @return The result of the H function.
 */
constexpr uint32_t H(uint32_t x, uint32_t y, uint32_t z) {
    return (x ^ y ^ z);
}

/**
 * @brief Calculate the I function.
 * @param x The first number.
 * @param y The second number.
 * @param z The third number.
 * @return The result of the I function.
 */
constexpr uint32_t I(uint32_t x, uint32_t y, uint32_t z) {
    return (y ^ (x | ~z));
}

}

/**
 * @brief Get the hexdigest of the MD5 hash.
 * @return The hexdigest of the MD5 hash.
 */
std::string MD5::hexdigest() {
    std::string hex = "";
    std::string hex_table = "0123456789abcdef";
    for(byte b: digest) {
        hex.push_back(hex_table[b >> 4]);
        hex.push_back(hex_table[b & 0xF]);
    }
    return hex;
}

/**
 * @brief Update the MD5 hash.
 * @param data The data string to update the hash with.
 */
void MD5::update(const std::string& data) {
    ctx.md5_update((byte*)data.c_str(), data.size());
    ctx.md5_final(digest);
}

/**
 * @brief Update the MD5 hash.
 * @param The data to update to the hash with.
 */
void MD5::update(const byte* data, size_t data_len) {
    ctx.md5_update(const_cast<byte*>(data), data_len);
    ctx.md5_final(digest);
}

/**
 * @brief Reset the MD5 context.
 */
void MD5::reset() {
    ctx = MD5_CTX();
    std::fill(digest, digest + 16, 0);
};

/**
 * @brief Initialize the MD5 context.
 */
MD5::MD5_CTX::MD5_CTX() {
    count = 0;
    state[0] = 0x67452301;
    state[1] = 0xEFCDAB89;
    state[2] = 0x98BADCFE;
    state[3] = 0x10325476;
}

/**
 * @brief Update the MD5 hash.
 * @param data The data to update the hash with.
 * @param data_len The length of the data.
 */
void MD5::MD5_CTX::md5_update(byte* data, size_t data_len) {
    size_t buffer_idx = (count / 8) % 64;
    count += data_len * 8;
    size_t fill = 64 - buffer_idx;

    if(data_len >= fill) {
        std::copy(data, data + fill, &buffer[buffer_idx]);
        md5_transform(buffer);
        data += fill;
        data_len -= fill;
        while(data_len >= 64) {
            md5_transform(data);
            data += 64;
            data_len -= 64;
        }
        buffer_idx = 0;
    }
    std::copy(data, data + data_len, &buffer[buffer_idx]);
}

/**
 * @brief Transform the MD5 hash.
 * @param data The data to transform.
 */
void MD5::MD5_CTX::md5_transform(byte data[64]) {
    bit32 a = state[0], b = state[1], c = state[2], d = state[3];
    bit32 x[16];

    for(int i = 0; i < 16; ++i) {
        x[i] = static_cast<bit32>(data[i * 4]) |
               static_cast<bit32>(data[i * 4 + 1]) << 8 |
               static_cast<bit32>(data[i * 4 + 2]) << 16 |
               static_cast<bit32>(data[i * 4 + 3]) << 24;
    }

    for(int i = 0; i < 64; ++i) {
        bit32 f, g;
        if(i < 16) {
            f = F(b, c, d); g = i;
        } else if(i < 32) {
            f = G(b, c, d); g = (5 * i + 1) % 16;
        } else if(i < 48) {
            f = H(b, c, d); g = (3 * i + 5) % 16;
        } else {
            f = I(b, c, d); g = (7 * i) % 16;
        }
        bit32 temp = d;
        d = c;
        c = b;
        b = b + rotate_left(a + f + T[i] + x[g], S[i]);
        a = temp;
    }

    state[0] += a, state[1] += b, state[2] += c, state[3] += d;
}


/**
 * @brief Finalize the MD5 hash.
 * @param digest The digest to store the hash in.
 */
void MD5::MD5_CTX::md5_final(byte digest[16]) {
    bit64 bits = count;

    byte padding[64] = {0x80};
    size_t buffer_idx = (count / 8) % 64;
    size_t pad_len = (buffer_idx < 56) ? (56 - buffer_idx) : (120 - buffer_idx);

    md5_update(padding, pad_len);

    byte len_buf[8];
    for(int i = 0; i < 8; ++i) {
        len_buf[i] = (bits >> (i * 8)) & 0xFF;
    }
    md5_update(len_buf, 8);

    for(int i = 0; i < 4; ++i) {
        digest[i * 4]     = static_cast<byte>(state[i]);
        digest[i * 4 + 1] = static_cast<byte>(state[i] >> 8);
        digest[i * 4 + 2] = static_cast<byte>(state[i] >> 16);
        digest[i * 4 + 3] = static_cast<byte>(state[i] >> 24);
    }
}