#ifndef PCSP_STATUS_HPP
#define PCSP_STATUS_HPP

#include <cstdint>
#include <cstddef>
#include "pcsp/protocol.hpp"

// =============================================================================
// Non-Functional Requirements & Safety (REQ-NFR-802, API Doc Sec 6.2).
// Owner: Safety & QA. Depends only on the factory's nullptr contract and the
// length fields already parsed by the state machine — never on cipher math
// or socket code directly.
// =============================================================================
enum class PCSPStatus : uint8_t {
    OK                        = 0x00,
    ERR_UNSUPPORTED_VERSION   = 0x98,
    ERR_UNSUPPORTED_CIPHER    = 0x99,
    ERR_MATRIX_NOT_INVERTIBLE = 0x9A,
    ERR_LENGTH_MISMATCH       = 0x9B
};

// Extended Euclidean check: a Hill key matrix is invertible mod 26 iff
// gcd(determinant mod 26, 26) == 1 (API Doc Sec 6.2).
bool is_invertible_mod26(int determinant);

// Validates byte counts read against the header's advertised lengths and
// confirms the cipher id resolves via create_cipher(). On any non-OK status
// the caller must print the diagnostic code and reset the socket channel
// WITHOUT terminating the server process (REQ-NFR-802).
PCSPStatus validate_packet(const PCSPHeader& header,
                           std::size_t key_bytes_read,
                           std::size_t payload_bytes_read);

#endif // PCSP_STATUS_HPP
