#pragma once

namespace Cursed3DEngine
{

    enum PressedKey
    {
        KBD_NONE = 0,
        KBD_Q,
        KBD_W,
        KBD_S,
        KBD_A,
        KBD_D
    };

    class KeyboardHandler
    {
    public:
        KeyboardHandler();
        ~KeyboardHandler();
        PressedKey getPressedKey() const;
    };
}