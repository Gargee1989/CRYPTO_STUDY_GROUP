#include "pcsp/ciphers/rail_fence.hpp"
#include <deque>
#include <algorithm>

// cipher to rails (decryptor helper function)
std::vector<std::deque<uint8_t>> cipherToRails(
    const std::vector<uint8_t>& text,
    const uint8_t key)
{
    std::vector<std::deque<uint8_t>> rails(key);

    bool down;
    size_t t_idx = 0;
    size_t r_idx = 0;

    for (std::deque<uint8_t>& rail : rails) {
        if (t_idx >= text.size()) {
            break;
        }
        
        // key converted to size_t to deal with compiler warnings
        if (r_idx == 0 || r_idx == size_t(key) - 1) {
            for (size_t i = r_idx; i < text.size(); i += 2 * (key - 1)) {
                rail.push_back(text[t_idx]);
                t_idx++;
            }
        }
        else {
            size_t i = r_idx;
            size_t add_factor;

            down = true;

            while (i < text.size()) {
                if (down) {
                    add_factor = 2 * (key - 1 - r_idx);
                }
                else {
                    add_factor = 2 * r_idx;
                }

                i += add_factor;
                down = !down;

                rail.push_back(text[t_idx]);
                t_idx++;
            }
        }

        r_idx++;
    }
    return rails;
}
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
    if (o_key >= plaintext.size()) { 
        o_key = plaintext.size();
    }

    if (o_key <= 1) {
        return plaintext;
    }
    
    std::vector<uint8_t> ciphertext;
    ciphertext.reserve(plaintext.size());

    std::vector<std::vector<uint8_t>> rails(o_key);
    size_t rail_idx = 0;
    bool down = true;

    for (const uint8_t ch : plaintext) {
        rails[rail_idx].push_back(ch);
        if ((rail_idx + 1) >= o_key or (not down and (rail_idx < 1))) {
            down = not down;
        }
        rail_idx += (down * 1 + (not down) * (-1));
    }
    
    for (const std::vector<uint8_t>& rail : rails) {
        ciphertext.insert(ciphertext.end(), rail.begin(), rail.end());
    }

    return ciphertext;
}

// TODO(REQ-CIP-701): implement RailFenceCipher::decrypt
std::vector<uint8_t> RailFenceCipher::decrypt(
    const std::vector<uint8_t>& ciphertext,
    const std::vector<uint8_t>& key) const
{
    if (key.empty() || ciphertext.empty()) {
        return {};
    }

    uint8_t o_key = key.at(0);
    if (o_key >= ciphertext.size()) {
        o_key = ciphertext.size();
    }
      
    if (o_key <= 1) {
        return ciphertext;
    }

    
    // get rails repr of the cipher text
    std::vector<std::deque<uint8_t>> rails = cipherToRails(ciphertext, o_key);

    std::vector<uint8_t> plaintext;
    plaintext.reserve(ciphertext.size());

    bool down = true;
    size_t r_idx = 0;
    // size_t p_idx = 0;
    while (std::any_of(rails.begin(), rails.end(), 
            [](const auto& d) {return !d.empty(); })) {
        if (not rails[r_idx].empty()) {
            plaintext.push_back(
                rails[r_idx].front()
            );
            rails[r_idx].pop_front();

        }
        if ((r_idx + 1 >= o_key) or (not down and r_idx < 1)) {
            down = not down;
        }
        // ugly but works
        r_idx += (down * 1 + (-1) * (not down));
    }



    return plaintext;
}
