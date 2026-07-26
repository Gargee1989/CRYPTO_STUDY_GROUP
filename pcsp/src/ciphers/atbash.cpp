#include "pcsp/ciphers/atbash.hpp"

namespace {

uint8_t atbash_map(uint8_t value) {
    return static_cast<uint8_t>(25 - value);
}

} // namespace

std::vector<uint8_t> AtbashCipher::encrypt(
    const std::vector<uint8_t>& plaintext,
    const std::vector<uint8_t>& key) const
{
    (void)key;

    std::vector<uint8_t> ciphertext;
    ciphertext.reserve(plaintext.size());

    for (uint8_t value : plaintext) {
        ciphertext.push_back(atbash_map(value));
    }

    return ciphertext;
}

std::vector<uint8_t> AtbashCipher::decrypt(
    const std::vector<uint8_t>& ciphertext,
    const std::vector<uint8_t>& key) const
{
    (void)key;

    std::vector<uint8_t> plaintext;
    plaintext.reserve(ciphertext.size());

    for (uint8_t value : ciphertext) {
        plaintext.push_back(atbash_map(value));
    }

    return plaintext;
}
