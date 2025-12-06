#pragma once
#include "lan/lan.hpp"
#include "mscore/local_status.hpp"
#include "msui/local_ui.hpp"

class GameClient {
   public:
    GameClient(size_t window_width, size_t window_height)
        : mLocalUI(mLocalStatus),
          mWindow(sf::VideoMode({1920, 1080}), "GG games") {};
    void run();

   private:
    void handle_input();
    void render();
    LanPeer mLanPeer;
    LocalStatus mLocalStatus;
    LocalUI mLocalUI;
    sf::RenderWindow mWindow;
};
