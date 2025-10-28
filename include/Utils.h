#pragma once

namespace Cursed3DEngine
{

    class TrigUtils
    {
    public:
        static constexpr float PI = 3.14159f;
        static constexpr float PI_2 = PI / 2.0f;
        static constexpr float PI_3 = PI / 3.0f;
        static constexpr float PI_4 = PI / 4.0f;
        static constexpr float _3PI_2 = 3.0f * PI_2;
        static constexpr float _3PI_4 = 3.0f * PI_4;
        static constexpr float _5PI_4 = 5.0f * PI_4;
        static constexpr float _7PI_4 = 7.0f * PI_4;
        static constexpr float _2PI = 2.0f * PI;

        static float getNormalizedAngle(float radians);
    };

}