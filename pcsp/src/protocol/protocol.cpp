#include "pcsp/protocol.hpp"
#include <arpa/inet.h>

PCSPHeader serialize_header(const PCSPHeader& host_header) {
    PCSPHeader net_header{};
    net_header.version = host_header.version;
    net_header.cipher_id = host_header.cipher_id;
    net_header.key_length = htons(host_header.key_length);
    net_header.payload_len = htons(host_header.payload_len);
    return net_header;
}

PCSPHeader deserialize_header(const PCSPHeader& net_header) {
    PCSPHeader host_header{};
    host_header.version = net_header.version;
    host_header.cipher_id = net_header.cipher_id;
    host_header.key_length = ntohs(net_header.key_length);
    host_header.payload_len = ntohs(net_header.payload_len);
    return host_header;
}
