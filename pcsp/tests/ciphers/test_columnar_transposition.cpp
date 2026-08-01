#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include "pcsp/ciphers/columnar_transposition.hpp"

namespace {

std::vector<uint8_t> bytes(const std::string& value) {
    return std::vector<uint8_t>(value.begin(), value.end());
}

bool expect_eq(const std::vector<uint8_t>& actual,
               const std::vector<uint8_t>& expected,
               const std::string& label) {
    if (actual == expected) {
        return true;
    }

    std::cerr << label << " failed\n";
    std::cerr << "  expected: " << std::string(expected.begin(), expected.end()) << '\n';
    std::cerr << "  actual:   " << std::string(actual.begin(), actual.end()) << '\n';
    return false;
}

} // namespace

// Standalone test harness for ColumnarTranspositionCipher (REQ-CIP-702).
// Build in isolation, no networking/protocol code required:
//   make -C tests columnar_transposition
// (API Doc Sec 7 item 2 -- crypto devs verify math without touching sockets.)
int main() {
    ColumnarTranspositionCipher cipher;

    std::vector<uint8_t> plaintext = bytes("WEAREDISCOVEREDFLEEATONCE");
    std::vector<uint8_t> key = bytes("ZEBRAS");
    std::vector<uint8_t> expected = bytes("EVLNACDTESEAROFODEECWIREE");

    std::vector<uint8_t> ciphertext = cipher.encrypt(plaintext, key);
    std::vector<uint8_t> recovered  = cipher.decrypt(ciphertext, key);

    bool ok = true;
    ok = expect_eq(ciphertext, expected, "known ciphertext") && ok;
    ok = expect_eq(recovered, plaintext, "roundtrip") && ok;

    std::vector<uint8_t> duplicate_key = bytes("BALLOON");
    std::vector<uint8_t> binary_plaintext = {'A', 0x00, 'B', 'C', 0x7f, 'D', 'E'};
    ok = expect_eq(cipher.decrypt(cipher.encrypt(binary_plaintext, duplicate_key), duplicate_key),
                   binary_plaintext,
                   "duplicate-key binary roundtrip") && ok;

    if (!ok) {
        return 1;
    }

    std::cout << "ColumnarTranspositionCipher tests passed" << std::endl;
    return 0;
}
