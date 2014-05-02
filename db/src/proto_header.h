#ifndef PROTO_HEADER_H
#define PROTO_HEADER_H

struct blitz_proto_header_t
{
    uint32_t len;
    uint32_t seq;
    uint16_t cmd;
    uint32_t ret;
    uint32_t uid;
} __attribute__((packed));

#endif
