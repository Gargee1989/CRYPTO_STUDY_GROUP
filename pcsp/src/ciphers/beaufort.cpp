#include "pcsp/ciphers/beaufort.hpp"
#include <cctype>
// @ved: copied from vigenere code
// ref used: https://uomustansiriyah.edu.iq/media/lectures/5/5_2023_11_13!11_35_06_PM.pdf

namespace {

// ascii only
uint8_t index_norm(uint8_t ch) {
    if (std::isupper(ch)) {
        return ch - 'A';
    }
    return ch - 'a';
}

uint8_t C(uint8_t P, uint8_t K) {
    uint8_t P_norm = index_norm(P), K_norm = index_norm(K);
    return (K_norm - P_norm + 26) % 26; 
}

uint8_t P(uint8_t C, uint16_t K) {
    uint8_t C_norm = index_norm(C), K_norm = index_norm(K);
    return (K_norm - C_norm + 26) % 26;
}

// even if atleast one character is alphabetic
bool is_str_alpha(const std::vector<uint8_t>& s) {
    if (s.empty()) return false;
    for (uint8_t c : s) {
        if (std::isalpha(c)) { return true; }
    }
    return false;
}

} // namespace

// TODO(REQ-CIP-502): implement BeaufortCipher::encrypt
// Key field format: Variable -- ASCII string
std::vector<uint8_t> BeaufortCipher::encrypt(
    const std::vector<uint8_t>& plaintext,
    const std::vector<uint8_t>& key) const
{
    // reject empty key
    if (key.size() <= 0 || !is_str_alpha(key)) {
        // not including the error message here right now.
        return {};
    }
    std::vector<uint8_t> ciphertext;
    ciphertext.reserve(plaintext.size());
    size_t key_index = 0;

    for (size_t i = 0; i < plaintext.size(); i++) {
        if (! std::isalpha(plaintext[i])) {
            ciphertext.push_back(plaintext[i]);
            continue;
        }
        while (! std::isalpha(key[key_index % key.size()])) {
            key_index = (key_index + 1) % key.size();
        }
        
        bool l = plaintext[i] >= 'a';
        ciphertext.push_back(
            (l * 'a' + (!l) * 'A') + C(plaintext[i], key[key_index])
        );
        key_index = (key_index + 1) % key.size();
    }

    return ciphertext; 
}

// TODO(REQ-CIP-502): implement BeaufortCipher::decrypt
std::vector<uint8_t> BeaufortCipher::decrypt(
    const std::vector<uint8_t>& ciphertext,
    const std::vector<uint8_t>& key) const
{
    if (key.size() <= 0 || !is_str_alpha(key)) {
        // not including the error message here right now.
        return {};
    }
    std::vector<uint8_t> plaintext;
    plaintext.reserve(ciphertext.size());

    size_t key_index = 0;

    for (size_t i = 0; i < ciphertext.size(); i++) {
        if (! std::isalpha(ciphertext[i])) {
            plaintext.push_back(ciphertext[i]);
            continue;
        }
        while (! std::isalpha(key[key_index % key.size()])) {
            key_index = (key_index + 1) % key.size();
        }
        bool l = ciphertext[i] >= 'a';
        plaintext.push_back(
            (l * 'a' + (!l) * 'A') + P(ciphertext[i], key[key_index])
        );
        key_index = (key_index + 1) % key.size();
    }

    return plaintext;
}
