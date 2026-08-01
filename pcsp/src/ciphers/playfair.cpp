#include "playfair.hpp"

#include <array>
#include <cctype> 
#include <set>
#include <utility>

namespace {

using KeySquare = std::array<std::array<uint8_t, 5>, 5>;
using PositionMap = std::array<std::pair<int, int>, 26>; // indexed by letter - 'A'

KeySquare generateKeySquare(const std::vector<uint8_t>& key) {
    std::set<uint8_t> seen;
    std::vector<uint8_t> square;
    square.reserve(25);

    for (uint8_t raw : key) {
        if (!std::isalpha(raw)) continue;
        uint8_t ch = static_cast<uint8_t>(std::toupper(raw));
        if (ch == 'J') ch = 'I';
        if (seen.find(ch) == seen.end()) {
            seen.insert(ch);
            square.push_back(ch);
        }
    }

    for (uint8_t ch = 'A'; ch <= 'Z'; ++ch) {
        if (ch == 'J') continue; // I/J share a cell
        if (seen.find(ch) == seen.end()) {
            seen.insert(ch);
            square.push_back(ch);
        }
    }

    KeySquare matrix{};
    for (int i = 0; i < 25; ++i) {
        matrix[static_cast<size_t>(i / 5)][static_cast<size_t>(i % 5)] = square[static_cast<size_t>(i)];
    }
    return matrix;
}
PositionMap buildPositionMap(const KeySquare& square) {
    PositionMap positions{};
    for (int r = 0; r < 5; ++r) {
        for (int c = 0; c < 5; ++c) {
            uint8_t ch = square[static_cast<size_t>(r)][static_cast<size_t>(c)];
            positions[static_cast<size_t>(ch - 'A')] = std::make_pair(r, c);
        }
    }
    // I and J occupy the same cell, so J mirrors I's position.
    positions[static_cast<size_t>('J' - 'A')] = positions[static_cast<size_t>('I' - 'A')];
    return positions;
}

std::vector<std::pair<uint8_t, uint8_t>> prepareDigrams(const std::vector<uint8_t>& text) {
    std::vector<uint8_t> cleaned;
    cleaned.reserve(text.size());
    for (uint8_t raw : text) {
        if (!std::isalpha(raw)) continue;
        uint8_t ch = static_cast<uint8_t>(std::toupper(raw));
        if (ch == 'J') ch = 'I';
        cleaned.push_back(ch);
    }

    std::vector<std::pair<uint8_t, uint8_t>> digrams;
    size_t i = 0;
    while (i < cleaned.size()) {
        uint8_t a = cleaned[i];
        if (i + 1 < cleaned.size()) {
            uint8_t b = cleaned[i + 1];
            if (a == b) {
                digrams.emplace_back(a, static_cast<uint8_t>('X'));
                i += 1; // 'b' stays to start the next pair
            } else {
                digrams.emplace_back(a, b);
                i += 2;
            }
        } else {
            digrams.emplace_back(a, static_cast<uint8_t>('X'));
            i += 1;
        }
    }

    return digrams;
}
} // namespace
// TODO(REQ-CIP-601): implement PlayfairCipher::encrypt
// Key field format: Variable -- keyword string
std::vector<uint8_t> PlayfairCipher::encrypt(
    const std::vector<uint8_t>& plaintext,
    const std::vector<uint8_t>& key) const
{
    const KeySquare matrix = generateKeySquare(key);
    const PositionMap positions = buildPositionMap(matrix);
    const auto digrams = prepareDigrams(plaintext);

    std::vector<uint8_t> ciphertext;
    ciphertext.reserve(digrams.size() * 2);

    for (const auto& digram : digrams) {
        uint8_t a = digram.first;
        uint8_t b = digram.second;
        const auto& [ra, ca] = positions[static_cast<size_t>(a - 'A')];
        const auto& [rb, cb] = positions[static_cast<size_t>(b - 'A')];

        if (ra == rb) {
            ciphertext.push_back(matrix[static_cast<size_t>(ra)][static_cast<size_t>((ca + 1) % 5)]);
            ciphertext.push_back(matrix[static_cast<size_t>(rb)][static_cast<size_t>((cb + 1) % 5)]);
        } else if (ca == cb) {
            ciphertext.push_back(matrix[static_cast<size_t>((ra + 1) % 5)][static_cast<size_t>(ca)]);
            ciphertext.push_back(matrix[static_cast<size_t>((rb + 1) % 5)][static_cast<size_t>(cb)]);
        } else {
            ciphertext.push_back(matrix[static_cast<size_t>(ra)][static_cast<size_t>(cb)]);
            ciphertext.push_back(matrix[static_cast<size_t>(rb)][static_cast<size_t>(ca)]);
        }
    }

    return ciphertext;
}

// TODO(REQ-CIP-601): implement PlayfairCipher::decrypt
std::vector<uint8_t> PlayfairCipher::decrypt(
    const std::vector<uint8_t>& ciphertext,
    const std::vector<uint8_t>& key) const
{
    const KeySquare matrix = generateKeySquare(key);
    const PositionMap positions = buildPositionMap(matrix);

    std::vector<uint8_t> cleaned;
    cleaned.reserve(ciphertext.size());
    for (uint8_t raw : ciphertext) {
        if (!std::isalpha(raw)) continue;
        uint8_t ch = static_cast<uint8_t>(std::toupper(raw));
        if (ch == 'J') ch = 'I';
        cleaned.push_back(ch);
    }

    std::vector<uint8_t> plaintext;
    plaintext.reserve(cleaned.size());

    for (size_t i = 0; i + 1 < cleaned.size(); i += 2) {
        uint8_t a = cleaned[i];
        uint8_t b = cleaned[i + 1];
        const auto& [ra, ca] = positions[static_cast<size_t>(a - 'A')];
        const auto& [rb, cb] = positions[static_cast<size_t>(b - 'A')];

        if (ra == rb) {
            plaintext.push_back(matrix[static_cast<size_t>(ra)][static_cast<size_t>((ca - 1 + 5) % 5)]);
            plaintext.push_back(matrix[static_cast<size_t>(rb)][static_cast<size_t>((cb - 1 + 5) % 5)]);
        } else if (ca == cb) {
            plaintext.push_back(matrix[static_cast<size_t>((ra - 1 + 5) % 5)][static_cast<size_t>(ca)]);
            plaintext.push_back(matrix[static_cast<size_t>((rb - 1 + 5) % 5)][static_cast<size_t>(cb)]);
        } else {
            plaintext.push_back(matrix[static_cast<size_t>(ra)][static_cast<size_t>(cb)]);
            plaintext.push_back(matrix[static_cast<size_t>(rb)][static_cast<size_t>(ca)]);
        }
    }

    return plaintext;
}
