#pragma once

#include <ncursesw/ncurses.h>

#include "Player.h"
#include "Map.h"

namespace Cursed3DEngine
{

    class Renderer
    {
    private:
        WINDOW *window;
        const int screenWidth;
        const int screenHeight;

        static constexpr float FOV = 3.14159f / 4.0f;
        static constexpr float RAY_DEPTH = 16.0f;

        float clearScreenTime = 0.0f;
        
    public:
        Renderer(int screenWidth, int screenHeight);
        ~Renderer();
        void renderScene(float elapsedTime, const Player &player, const Map& map);
        
    private:
        void initializeColors();
        void renderWorld(const Map& map, const Player &player);
        void renderMap(const Map& map, const Player &player);
        void renderHUD(const Player &player, float elapsedTime);
    };
    
}