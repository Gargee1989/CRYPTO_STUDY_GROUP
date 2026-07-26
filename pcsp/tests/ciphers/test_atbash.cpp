#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include <numeric>
#include "pcsp/ciphers/atbash.hpp"

// Developed with assistance from Codex (GPT-5).
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
    AtbashCipher cipher;

    {
        // Atbash mirrors the alphabet: 0 <-> 25, 1 <-> 24, and so on.
        std::vector<uint8_t> plaintext = { 0, 1, 2, 7, 22, 25 };
        std::vector<uint8_t> key;
        std::vector<uint8_t> expected  = { 25, 24, 23, 18, 3, 0 };

        check(cipher.encrypt(plaintext, key) == expected,
              "known_vector_encrypt");
        check(cipher.decrypt(expected, key) == plaintext,
              "known_vector_decrypt");
    }

    {
        // Applying Atbash twice must return every alphabet value unchanged.
        std::vector<uint8_t> plaintext(26);
        std::iota(plaintext.begin(), plaintext.end(), 0);
        std::vector<uint8_t> key;

        std::vector<uint8_t> ciphertext = cipher.encrypt(plaintext, key);
        std::vector<uint8_t> recovered = cipher.decrypt(ciphertext, key);

        check(recovered == plaintext, "round_trip_full_alphabet_domain");
    }

    {
        std::vector<uint8_t> plaintext;
        std::vector<uint8_t> key;

        check(cipher.encrypt(plaintext, key).empty(),
              "empty_plaintext_yields_empty_ciphertext");
    }

    {
        // Atbash is keyless, so supplied key bytes must not affect its output.
        std::vector<uint8_t> plaintext = { 0, 13, 25 };
        std::vector<uint8_t> unexpected_key = { 1, 2, 3 };
        std::vector<uint8_t> expected = { 25, 12, 0 };

        check(cipher.encrypt(plaintext, unexpected_key) == expected,
              "key_is_ignored_for_keyless_cipher");
    }

    std::cout << g_failures << " failure(s)." << std::endl;
    return g_failures == 0 ? 0 : 1;
}
