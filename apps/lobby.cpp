#include "game_client.hpp"

int main(int argc, char* argv[]) {
    GameClient game_client(1920, 1080);
    game_client.run();

    return 0;
}
