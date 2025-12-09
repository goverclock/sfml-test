#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

namespace lan {

class LanPeer {
   public:
    LanPeer();
    ~LanPeer();

    void start_broadcast();

   private:
    void broadcast_as_host();
    void receive_as_guest();

    constexpr static int PORT = 6969;
    int mSocketFd;
    struct sockaddr_in mBroadcastAddr;
};

};  // namespace lan
