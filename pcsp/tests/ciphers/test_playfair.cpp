#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <cstdint>

#include "pcsp/ciphers/playfair.hpp"

#ifdef _WIN32
#include <io.h>
#define isatty _isatty
#define fileno _fileno
#else
#include <unistd.h>
#endif

namespace {

const char* GREEN = "\033[32m";
const char* RED   = "\033[31m";
const char* RESET = "\033[0m";

std::vector<uint8_t> toBytes(const std::string& s)
{
    return {s.begin(), s.end()};
}

std::string toString(const std::vector<uint8_t>& v)
{
    return {v.begin(), v.end()};
}

std::vector<uint8_t> normalizePlayfair(const std::string& text)
{
    std::vector<uint8_t> cleaned;

    for (unsigned char ch : text) {
        if (!std::isalpha(ch))
            continue;

        ch = static_cast<unsigned char>(std::toupper(ch));

        if (ch == 'J')
            ch = 'I';

        cleaned.push_back(ch);
    }

    std::vector<uint8_t> normalized;

    size_t i = 0;

    while (i < cleaned.size()) {

        uint8_t a = cleaned[i];

        if (i + 1 < cleaned.size()) {

            uint8_t b = cleaned[i + 1];

            if (a == b) {

                normalized.push_back(a);
                normalized.push_back('X');
                ++i;

            } else {

                normalized.push_back(a);
                normalized.push_back(b);
                i += 2;
            }

        } else {

            normalized.push_back(a);
            normalized.push_back('X');
            ++i;
        }
    }

    return normalized;
}

bool runTest(
    PlayfairCipher& cipher,
    const std::string& key,
    const std::string& plaintext,
    const std::string& name)
{
    auto k = toBytes(key);
    auto p = toBytes(plaintext);

    auto expected = normalizePlayfair(plaintext);

    auto enc = cipher.encrypt(p, k);
    auto dec = cipher.decrypt(enc, k);

    bool ok = (dec == expected);

    std::cout
        << (ok ? GREEN : RED)
        << (ok ? "[PASS] " : "[FAIL] ")
        << RESET
        << name << '\n';

    if (!ok) {
        std::cout << "  key         : " << key << '\n';
        std::cout << "  plaintext   : " << plaintext << '\n';
        std::cout << "  expected    : " << toString(expected) << '\n';
        std::cout << "  ciphertext  : " << toString(enc) << '\n';
        std::cout << "  decrypted   : " << toString(dec) << '\n';
    }

    return ok;
}

}

int main()
{
    PlayfairCipher cipher;

    int passed = 0;
    int total = 0;

    auto TEST = [&](const std::string& key,
                    const std::string& text,
                    const std::string& name)
    {
        ++total;
        if (runTest(cipher, key, text, name))
            ++passed;
    };

    //
    // Default edge cases
    //

    TEST("keyword", "", "Empty string");
    TEST("keyword", "A", "Single character");
    TEST("keyword", "HELLO", "Repeated letters");
    TEST("keyword", "BALLOON", "Multiple repeated letters");
    TEST("keyword", "JIGGLE", "Contains J");
    TEST("keyword", "Hide the gold in the tree stump",
         "Classic Playfair example");
    TEST("playfair example",
         "Attack at dawn!",
         "Spaces and punctuation");
    TEST("abcdefghijklmnopqrstuvwxyz",
         "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
         "Full alphabet");

    //
    // Optional redirected input
    //
    // Expected format:
    //
    // line 1 -> key
    // line 2 -> plaintext
    //

    if (!isatty(fileno(stdin))) {

        std::string key;
        std::string text;

        if (std::getline(std::cin, key) &&
            std::getline(std::cin, text))
        {
            TEST(key, text, "Redirected input");
        }
    }

    std::cout << '\n';

    if (passed == total)
        std::cout << GREEN;
    else
        std::cout << RED;

    std::cout
        << passed
        << "/"
        << total
        << " tests passed."
        << RESET
        << '\n';

    return passed == total ? 0 : 1;
}
