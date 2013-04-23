#pragma once

#include "SDLbase.h"
#include "Sprite.h"
#include "Planeta.h"
#include "Cavalo.h"

class GameManager{
    private:
        Sprite *bg;
        Sprite *planetSprite;
        Sprite *cavaloSprite;
        std::vector<Planeta*>planetArray;
        Cavalo *cavaloObj;
        Uint8 *keystate;
        Point camera;
        static const int SCREEN_WIDTH  = 800;
        static const int SCREEN_HEIGHT  = 600;
        static const int LEVEL_WIDTH  = 1024;
        static const int LEVEL_HEIGHT  = 728;

    public:
        GameManager();
        ~GameManager();
        void run();
        void addPlaneta();
        void checkPlaneta();
        void processEvents();
        void movimento();
        void update();
        void render();
};
