#pragma once

#include <iostream>

namespace Cursed3DEngine
{
    enum MapSymbol
    {
        AIR_SYMBOL    = '.',
        WALL_SYMBOL   = '#',
        PLAYER_SYMBOL = '@'
    };

    class Map
    {
    private:
        const int width;
        const int height;
        std::wstring layout;

    public:
        Map();
        Map(int width, int height, std::wstring layout);
        ~Map();

        int getWidth() const;
        int getHeight() const;
        std::wstring getLayout() const;
        wchar_t operator[](int index) const;
    };
}