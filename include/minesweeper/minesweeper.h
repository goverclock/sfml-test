#pragma once
#include <string>
#include <vector>

namespace ms {

typedef struct {
    std::string nickname;
    std::string ip;
} PlayerInfo;

void run(const std::vector<PlayerInfo> &players_info);

};  // namespace ms
