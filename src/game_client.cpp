#include <chrono>
#include <print>
#include <thread>

#include "game_client.hpp"
#include "scene_manager.hpp"

void GameClient::run() {
    while (mSceneManager.is_window_open()) {
        mLocalStatus.update();
        mSceneManager.handle_window_event();
        mSceneManager.display();
        // TODO: maybe we can do better, like only loop once when needed
        std::this_thread::sleep_for(
            std::chrono::milliseconds(16));  // e.g. 60 FPS
    }

    std::println("exiting, normally");
}
