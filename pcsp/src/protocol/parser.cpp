#include "pcsp/protocol.hpp"

#include "pcsp/diagnostics.hpp"
#include "pcsp/icipher.hpp"
#include "pcsp/status.hpp"

#include <cerrno>
#include <exception>
#include <iomanip>
#include <iostream>
#include <sys/socket.h>

namespace {

bool recv_exact(int fd, void* buffer, std::size_t length) {
    auto* out = static_cast<uint8_t*>(buffer);
    std::size_t total = 0;

    while (total < length) {
        const ssize_t received = recv(fd, out + total, length - total, MSG_WAITALL);
        if (received > 0) {
            total += static_cast<std::size_t>(received);
            continue;
        }

        if (received < 0 && errno == EINTR) {
            continue;
        }

        return false;
    }

    return true;
}

void print_status(PCSPStatus status) {
    std::cerr << "PCSP diagnostic status: 0x"
              << std::hex << std::setw(2) << std::setfill('0')
              << static_cast<unsigned int>(status)
              << std::dec << std::setfill(' ') << std::endl;
}

std::vector<uint8_t> build_raw_stream(const PCSPHeader& header,
                                      const std::vector<uint8_t>& key,
                                      const std::vector<uint8_t>& payload) {
    const PCSPHeader net_header = serialize_header(header);
    const auto* header_bytes = reinterpret_cast<const uint8_t*>(&net_header);

    std::vector<uint8_t> raw;
    raw.reserve(sizeof(PCSPHeader) + key.size() + payload.size());
    raw.insert(raw.end(), header_bytes, header_bytes + sizeof(PCSPHeader));
    raw.insert(raw.end(), key.begin(), key.end());
    raw.insert(raw.end(), payload.begin(), payload.end());
    return raw;
}

} // namespace

bool parse_incoming_stream(int client_fd) {
    PCSPHeader net_header{};
    if (!recv_exact(client_fd, &net_header, sizeof(net_header))) {
        return false;
    }

    const PCSPHeader header = deserialize_header(net_header);
    if (header.version != 0x01) {
        print_status(PCSPStatus::ERR_UNSUPPORTED_VERSION);
        return false;
    }

    std::vector<uint8_t> key(header.key_length);
    std::vector<uint8_t> payload(header.payload_len);

    if (!key.empty() && !recv_exact(client_fd, key.data(), key.size())) {
        print_status(PCSPStatus::ERR_LENGTH_MISMATCH);
        return false;
    }

    if (!payload.empty() && !recv_exact(client_fd, payload.data(), payload.size())) {
        print_status(PCSPStatus::ERR_LENGTH_MISMATCH);
        return false;
    }

    return dispatch_and_report(header, key, payload);
}

bool dispatch_and_report(const PCSPHeader& header,
                          const std::vector<uint8_t>& key,
                          const std::vector<uint8_t>& payload) {
    const PCSPStatus status = validate_packet(header, key.size(), payload.size());
    if (status != PCSPStatus::OK) {
        print_status(status);
        return false;
    }

    try {
        std::unique_ptr<ICipher> cipher = create_cipher(header.cipher_id);
        if (!cipher) {
            print_status(PCSPStatus::ERR_UNSUPPORTED_CIPHER);
            return false;
        }

        const std::vector<uint8_t> plaintext = cipher->decrypt(payload, key);
        log_server_telemetry(build_raw_stream(header, key, payload), payload, plaintext);
    } catch (const std::exception& error) {
        std::cerr << "PCSP dispatch error: " << error.what() << std::endl;
        return false;
    } catch (...) {
        std::cerr << "PCSP dispatch error: unknown exception" << std::endl;
        return false;
    }

    return true;
}
