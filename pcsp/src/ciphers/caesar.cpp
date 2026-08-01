#include "pcsp/ciphers/caesar.hpp"

// Key field format: 1 byte -- shift value
std::vector<uint8_t> CaesarCipher::encrypt(
    const std::vector<uint8_t>& plaintext,
    const std::vector<uint8_t>& key) const
{
    if (key.size() != 1) {
        return {};
    }

    const uint8_t shift = static_cast<uint8_t>(key[0] % 26);
    std::vector<uint8_t> ciphertext;
    ciphertext.reserve(plaintext.size());

    for (uint8_t value : plaintext) {
        ciphertext.push_back(static_cast<uint8_t>((value + shift) % 26));
    }

    return ciphertext;
}

std::vector<uint8_t> CaesarCipher::decrypt(
    const std::vector<uint8_t>& ciphertext,
    const std::vector<uint8_t>& key) const
{
    if (key.size() != 1) {
        return {};
    }

    const uint8_t shift = static_cast<uint8_t>(key[0] % 26);
    std::vector<uint8_t> plaintext;
    plaintext.reserve(ciphertext.size());

    for (uint8_t value : ciphertext) {
        plaintext.push_back(static_cast<uint8_t>((value + 26 - shift) % 26));
    }

    return plaintext;
}
