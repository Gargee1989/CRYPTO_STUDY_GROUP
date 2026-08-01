#include <iostream>
#include <cassert>
#include <arpa/inet.h>

#include "pcsp/protocol.hpp"

// Standalone roundtrip check for serialize_header()/deserialize_header()
// (REQ-NET-104). Build in isolation: `make -C tests protocol`.
int main() {
    PCSPHeader h{};
    h.version     = 0x01;
    h.cipher_id   = 0x20;   // Vigenere, for illustration
    h.key_length  = 5;
    h.payload_len = 16;

    PCSPHeader net  = serialize_header(h);
    PCSPHeader back = deserialize_header(net);

    assert(back.version == h.version);
    assert(back.cipher_id == h.cipher_id);
    assert(back.key_length == h.key_length);
    assert(back.payload_len == h.payload_len);
    assert(net.key_length == htons(h.key_length));
    assert(net.payload_len == htons(h.payload_len));

    std::cout << "protocol header serialization roundtrip passed" << std::endl;
    return 0;
}
