#include "pcsp/protocol.hpp"
#include <arpa/inet.h>

// TODO(REQ-NET-104): htons() key_length and payload_len before transmission.
// version/cipher_id are single bytes and pass through unchanged.
PCSPHeader serialize_header(const PCSPHeader& host_header){
    PCSPHeader net_header = host_header;
    net_header.key_length = htons(host_header.key_length);
    net_header.payload_len = htons(host_header.payload_len);
    return net_header;
}

// TODO(REQ-NET-104): ntohs() key_length and payload_len immediately upon
// ingestion, before any length field is read by the caller.
PCSPHeader deserialize_header(const PCSPHeader& net_header){
    PCSPHeader host_header = net_header;
    host_header.key_length = ntohs(net_header.key_length);
    host_header.payload_len = ntohs(net_header.payload_len);
    return host_header;
}
