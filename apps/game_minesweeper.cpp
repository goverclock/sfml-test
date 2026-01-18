#include <cassert>

#include "minesweeper/minesweeper.h"

int main(int argc, char* argv[]) {
    // the args are player nickname and ip, passed by lobby program
    std::vector<ms::PlayerInfo> players_info;
    for (int i = 1; i < argc; i += 2) {
        assert(i + 1 < argc);
        ms::PlayerInfo pi;
        pi.nickname = std::string(argv[i]);
        pi.ip = std::string(argv[i + 1]);
        players_info.push_back(pi);
    }

    ms::run(players_info);

    return 0;
}
