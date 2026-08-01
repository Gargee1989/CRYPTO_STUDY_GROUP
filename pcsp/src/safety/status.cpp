#include "pcsp/status.hpp"
#include "pcsp/icipher.hpp"

bool is_invertible_mod26(int determinant) {
    int a = ((determinant % 26) + 26) % 26;
    int b = 26;

    while (b != 0) {
        const int t = b;
        b = a % b;
        a = t;
    }

    return a == 1;
}

PCSPStatus validate_packet(const PCSPHeader& header,
                           std::size_t key_bytes_read,
                           std::size_t payload_bytes_read) {
    if (header.version != 0x01) {
        return PCSPStatus::ERR_UNSUPPORTED_VERSION;
    }

    if (key_bytes_read != header.key_length ||
        payload_bytes_read != header.payload_len) {
        return PCSPStatus::ERR_LENGTH_MISMATCH;
    }

    if (!create_cipher(header.cipher_id)) {
        return PCSPStatus::ERR_UNSUPPORTED_CIPHER;
    }

    return PCSPStatus::OK;
}
