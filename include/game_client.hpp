#pragma once
#include "mscore/local_status.hpp"
#include "scene_manager.hpp"

class GameClient {
   public:
    GameClient() : mSceneManager(mLocalStatus) {}
    void run();

   private:
    LocalStatus mLocalStatus;
    SceneManager mSceneManager;
};
