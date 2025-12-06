#include <unistd.h>

#include <cassert>
#include <cstring>
#include <print>

#include "lan/lan.hpp"

LanPeer::LanPeer() {
    mSocketFd = socket(AF_INET, SOCK_DGRAM, 0);
    assert(mSocketFd);
    int val = 1;
    int ret =
        setsockopt(mSocketFd, SOL_SOCKET, SO_BROADCAST, &val, sizeof(val));
    assert(!ret);
    ret = setsockopt(mSocketFd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    assert(!ret);

    struct sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(LanPeer::PORT);
    local_addr.sin_addr.s_addr = INADDR_ANY;
    ret = bind(mSocketFd, (struct sockaddr*)&local_addr, sizeof(local_addr));
    assert(!ret);

    mBroadcastAddr = local_addr;
    mBroadcastAddr.sin_addr.s_addr = inet_addr("255.255.255.255");
}

LanPeer::~LanPeer() {
    if (mSocketFd > 0) close(mSocketFd);
}

void LanPeer::broadcast_as_host() {
    const char* msg = "hello there is a host";
    int len = sendto(mSocketFd, msg, strlen(msg), 0,
                     (struct sockaddr*)&mBroadcastAddr, sizeof(mBroadcastAddr));
    std::println("broadcasting with {} bytes", len);
    assert(len == strlen(msg));
}

void LanPeer::receive_as_guest() {
    char buffer[65536];
    struct sockaddr_in host_addr;
    socklen_t host_addr_len = sizeof(host_addr);
    ssize_t len = recvfrom(mSocketFd, buffer, sizeof(buffer) - 1, 0,
                           (struct sockaddr*)&host_addr, &host_addr_len);

    //    assert(len > 0);
    buffer[len] = '\0';
    std::string sender_ip = inet_ntoa(host_addr.sin_addr);
    std::println("received {} bytes from {}:{}, msg: '{}'", len, sender_ip,
                 ntohs(host_addr.sin_port), buffer);
}
