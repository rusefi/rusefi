#pragma once
#include "pch.h"

struct sockaddr { };
struct sockaddr_in {
    int sin_family;
    uint16_t sin_port;
    struct { uint32_t s_addr; } sin_addr;
};
using socklen_t = unsigned;
constexpr int AF_INET = 2, SOCK_STREAM = 1, IPPROTO_TCP = 6;
constexpr int TCP_NODELAY = 1, SOL_SOCKET = 0xfff, SO_KEEPALIVE = 8;
constexpr int TCP_KEEPIDLE = 2, TCP_KEEPINTVL = 3, TCP_KEEPCNT = 4;
constexpr uint32_t INADDR_ANY = 0;
inline uint16_t htons(uint16_t value) { return (value >> 8) | (value << 8); }
int lwip_socket(int, int, int);
int lwip_bind(int, const sockaddr*, socklen_t);
int lwip_listen(int, int);
int lwip_accept(int, sockaddr*, socklen_t*);
int lwip_setsockopt(int, int, int, const void*, socklen_t);
int lwip_close(int);
int lwip_send(int, const void*, size_t, int);
int lwip_recv(int, void*, size_t, int);
