#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include "pcsp/ciphers/caesar.hpp"

// Cipher values represent letters as 0..25 rather than ASCII characters.
namespace {

int g_failures = 0;

void check(bool condition, const std::string& test_name) {
    if (condition) {
        std::cout << "[PASS] " << test_name << std::endl;
    } else {
        std::cout << "[FAIL] " << test_name << std::endl;
        ++g_failures;
    }
}

} // namespace

int main() {
    CaesarCipher cipher;

    {
        std::vector<uint8_t> plaintext = { 0, 1, 2, 25 };
        std::vector<uint8_t> key = { 3 };
        std::vector<uint8_t> expected = { 3, 4, 5, 2 };

        check(cipher.encrypt(plaintext, key) == expected,
              "known_vector_encrypt");
        check(cipher.decrypt(expected, key) == plaintext,
              "known_vector_decrypt");
    }

    {
        // A shift larger than 25 wraps around the 26-letter alphabet.
        std::vector<uint8_t> plaintext = { 0, 12, 25 };
        std::vector<uint8_t> key = { 29 };
        std::vector<uint8_t> expected = { 3, 15, 2 };

        check(cipher.encrypt(plaintext, key) == expected,
              "shift_wraps_modulo_alphabet_size");
    }

    {
        std::vector<uint8_t> plaintext = { 0, 7, 13, 25 };
        std::vector<uint8_t> key = { 11 };
        std::vector<uint8_t> ciphertext = cipher.encrypt(plaintext, key);

        check(cipher.decrypt(ciphertext, key) == plaintext,
              "round_trip");
    }

    {
        std::vector<uint8_t> plaintext;
        std::vector<uint8_t> key = { 5 };

        check(cipher.encrypt(plaintext, key).empty(),
              "empty_plaintext_yields_empty_ciphertext");
    }

    {
        std::vector<uint8_t> plaintext = { 1, 2, 3 };
        std::vector<uint8_t> no_key;
        std::vector<uint8_t> extra_key = { 4, 5 };

        check(cipher.encrypt(plaintext, no_key).empty(),
              "encrypt_rejects_missing_key");
        check(cipher.decrypt(plaintext, extra_key).empty(),
              "decrypt_rejects_wrong_size_key");
    }

    std::cout << g_failures << " failure(s)." << std::endl;
    return g_failures == 0 ? 0 : 1;
}
