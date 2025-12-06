#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

class LanPeer {
   public:
    LanPeer();
    ~LanPeer();

    void broadcast_as_host();
    void receive_as_guest();

   private:
    constexpr static int PORT = 6969;
    int mSocketFd;
    struct sockaddr_in mBroadcastAddr;
};
