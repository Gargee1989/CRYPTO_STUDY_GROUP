// #include <iostream>
// #include <vector>
// #include <cstdint>
// #include "pcsp/ciphers/rail_fence.hpp"

// // Standalone test harness for RailFenceCipher (REQ-CIP-701).
// // Build in isolation, no networking/protocol code required:
// //   make -C tests rail_fence
// // (API Doc Sec 7 item 2 -- crypto devs verify math without touching sockets.)
// int main() {
//     RailFenceCipher cipher;

//     std::vector<uint8_t> plaintext = { /* TODO: sample plaintext bytes */ };
//     std::vector<uint8_t> key       = { /* TODO: sample key, format: 1 byte -- rail count */ };

//     std::vector<uint8_t> ciphertext = cipher.encrypt(plaintext, key);
//     std::vector<uint8_t> recovered  = cipher.decrypt(ciphertext, key);

//     std::cout << "TODO: assert recovered == plaintext for RailFenceCipher" << std::endl;
//     return 0; // TODO: return 1 on mismatch once real assertions are added
// }
#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include <sstream>

#include "pcsp/ciphers/rail_fence.hpp"


#define GREEN "\033[32m"
#define RED   "\033[31m"
#define RESET "\033[0m"


struct Failure {
    std::string phrase;
    uint8_t key;
    std::vector<uint8_t> encrypted;
    std::vector<uint8_t> decrypted;
};


std::vector<uint8_t> toBytes(const std::string& text)
{
    return std::vector<uint8_t>(
        text.begin(),
        text.end()
    );
}


std::string fromBytes(const std::vector<uint8_t>& bytes)
{
    return std::string(
        bytes.begin(),
        bytes.end()
    );
}


void printBytes(const std::vector<uint8_t>& data)
{
    for (uint8_t b : data) {
        if (b >= 32 && b <= 126)
            std::cout << static_cast<char>(b);
        else
            std::cout << "\\x"
                      << std::hex
                      << static_cast<int>(b)
                      << std::dec;
    }
}


bool runTest(
    RailFenceCipher& cipher,
    const std::string& phrase,
    uint8_t key,
    Failure& failure)
{
    auto plaintext = toBytes(phrase);
    auto keyBytes = std::vector<uint8_t>{key};

    auto encrypted =
        cipher.encrypt(plaintext, keyBytes);

    auto decrypted =
        cipher.decrypt(encrypted, keyBytes);


    if (decrypted != plaintext) {

        failure.phrase = phrase;
        failure.key = key;
        failure.encrypted = encrypted;
        failure.decrypted = decrypted;

        return false;
    }

    return true;
}


int main()
{
    RailFenceCipher cipher;

    std::vector<Failure> failures;

    size_t passed = 0;
    size_t total = 0;


    std::cout << "Running edge cases...\n";


    std::vector<std::string> edgeCases = {
        "",
        "A",
        "AB",
        "ABC",
        "AAAA",
        "123456789",
        "The quick brown fox jumps over the lazy dog"
    };


    for (const auto& phrase : edgeCases) {

        /*
         * Key 1 is included deliberately.
         * Rail fence with 1 rail should be an identity operation.
         */
        for (size_t key = 1; key <= std::max<size_t>(1, phrase.size()); key++) {

            total++;

            Failure failure;

            if (runTest(
                    cipher,
                    phrase,
                    static_cast<uint8_t>(key),
                    failure))
            {
                passed++;
            }
            else {
                failures.push_back(failure);
            }
        }
    }


    std::cout << "\nRunning wordlist tests...\n";


    std::string line;

    while (std::getline(std::cin, line)) {

        /*
         * Skip empty lines from wordlists.
         */
        if (line.empty())
            continue;


        for (size_t key = 1; key <= line.size(); key++) {

            total++;

            Failure failure;

            if (runTest(
                    cipher,
                    line,
                    static_cast<uint8_t>(key),
                    failure))
            {
                passed++;
            }
            else {
                failures.push_back(failure);
            }
        }
    }



    std::cout
        << "\n========== RESULTS ==========\n";


    std::cout
        << GREEN
        << "Passed: "
        << passed
        << RESET
        << "\n";


    std::cout
        << RED
        << "Failed: "
        << failures.size()
        << RESET
        << "\n";


    std::cout
        << "Total: "
        << total
        << "\n";


    if (!failures.empty()) {

        std::cout
            << "\n========== FAILURE REPORT ==========\n";


        for (size_t i = 0; i < failures.size(); i++) {

            const auto& f = failures[i];

            std::cout
                << RED
                << "\nFailure #"
                << i + 1
                << RESET
                << "\n";


            std::cout
                << "Phrase: "
                << f.phrase
                << "\n";


            std::cout
                << "Key: "
                << static_cast<int>(f.key)
                << "\n";


            std::cout
                << "Encrypted: ";

            printBytes(f.encrypted);

            std::cout << "\n";


            std::cout
                << "Decrypted: ";

            printBytes(f.decrypted);

            std::cout << "\n";
        }
    }


    return failures.empty() ? 0 : 1;
}