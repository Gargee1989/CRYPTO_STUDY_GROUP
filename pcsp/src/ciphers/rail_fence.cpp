#include "pcsp/ciphers/rail_fence.hpp"

// TODO(REQ-CIP-701): implement RailFenceCipher::encrypt
// Key field format: 1 byte -- rail count
std::vector<uint8_t> RailFenceCipher::encrypt(
    const std::vector<uint8_t>& plaintext,
    const std::vector<uint8_t>& key) const
{
    // works 
    if (key.empty() || plaintext.empty()) {
        return {};
    }
    uint8_t o_key = key.at(0);
    
    std::vector<uint8_t> ciphertext;
    ciphertext.reserve(plaintext.size());

    std::vector<std::vector<uint8_t>> rails(o_key);
    uint8_t rail_idx = 0;
    bool down = true;

    for (const uint8_t ch : plaintext) {
        rails[rail_idx].push_back(ch);
        if ((rail_idx + 1) >= o_key or (not down and (rail_idx < 1))) {
            down = not down;
        }
        rail_idx += (down * 1 + (not down) * (-1));
    }
    
    for (const std::vector<uint8_t> rail : rails) {
        ciphertext.insert(ciphertext.end(), rail.begin(), rail.end());
    }

    return ciphertext;
}

// TODO(REQ-CIP-701): implement RailFenceCipher::decrypt
std::vector<uint8_t> RailFenceCipher::decrypt(
    const std::vector<uint8_t>& ciphertext,
    const std::vector<uint8_t>& key) const
{
    (void)key;
    return ciphertext; // placeholder identity transform -- replace with real math
}
