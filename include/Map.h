#pragma once

#include <iostream>

namespace Cursed3DEngine
{
    enum MapSymbol
    {
        AIR_SYMBOL    = '.',
        WALL_SYMBOL   = '#',
        PLAYER_FACING_N = '^',
        PLAYER_FACING_S = 'v',
        PLAYER_FACING_W = '<',
        PLAYER_FACING_E = '>',
    };

    class Map
    {
    private:
        int width;
        int height;
        std::wstring layout;

    public:
        Map();
        Map(int width, int height, std::wstring layout);
        Map(std::string mapFile);
        ~Map();

        int getWidth() const;
        int getHeight() const;
        std::wstring getLayout() const;
        wchar_t operator[](int index) const;
    };
}