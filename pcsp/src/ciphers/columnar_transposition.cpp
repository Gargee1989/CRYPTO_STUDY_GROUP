#include "pcsp/ciphers/columnar_transposition.hpp"

#include <algorithm>
#include <cstddef>
#include <numeric>

namespace {

std::vector<std::size_t> column_order(const std::vector<uint8_t>& key) {
    std::vector<std::size_t> order(key.size());
    std::iota(order.begin(), order.end(), 0);

    std::stable_sort(order.begin(), order.end(),
                     [&key](std::size_t left, std::size_t right) {
                         if (key[left] == key[right]) {
                             return left < right;
                         }
                         return key[left] < key[right];
                     });

    return order;
}

std::size_t column_height(std::size_t text_len,
                          std::size_t column_count,
                          std::size_t column) {
    const std::size_t full_rows = text_len / column_count;
    const std::size_t remainder = text_len % column_count;
    return full_rows + (column < remainder ? 1U : 0U);
}

} // namespace

// Key field format: Variable -- keyword string
std::vector<uint8_t> ColumnarTranspositionCipher::encrypt(
    const std::vector<uint8_t>& plaintext,
    const std::vector<uint8_t>& key) const
{
    if (plaintext.empty() || key.empty()) {
        return plaintext;
    }

    const std::size_t columns = key.size();
    const std::size_t rows = (plaintext.size() + columns - 1) / columns;
    const std::vector<std::size_t> order = column_order(key);

    std::vector<uint8_t> ciphertext;
    ciphertext.reserve(plaintext.size());

    for (std::size_t column : order) {
        for (std::size_t row = 0; row < rows; ++row) {
            const std::size_t index = row * columns + column;
            if (index < plaintext.size()) {
                ciphertext.push_back(plaintext[index]);
            }
        }
    }

    return ciphertext;
}

std::vector<uint8_t> ColumnarTranspositionCipher::decrypt(
    const std::vector<uint8_t>& ciphertext,
    const std::vector<uint8_t>& key) const
{
    if (ciphertext.empty() || key.empty()) {
        return ciphertext;
    }

    const std::size_t columns = key.size();
    const std::vector<std::size_t> order = column_order(key);
    std::vector<uint8_t> plaintext(ciphertext.size());

    std::size_t input_index = 0;
    for (std::size_t column : order) {
        const std::size_t height = column_height(ciphertext.size(), columns, column);
        for (std::size_t row = 0; row < height; ++row) {
            const std::size_t output_index = row * columns + column;
            plaintext[output_index] = ciphertext[input_index++];
        }
    }

    return plaintext;
}
