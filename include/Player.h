#pragma once

#include "Vector2F.h"
#include "Map.h"

namespace Cursed3DEngine
{

    enum PlayerMovement
    {
        MOVE_FORWARD,
        MOVE_BACKWARD,
        ROTATE_CR,
        ROTATE_CCR
    };

    class Player
    {
    private:
        Vector2F position;
        float rotationAngle;
        float speed = 10.0f;
    
    public:
        Player(float startingX, float startingY, float startingRotationAngle);
        ~Player();

        const Vector2F& getPosition() const;
        void setPosition(const Vector2F& position);

        float getRotationAngle() const;
        void setRotationAngle(float rotationAngle);

        float getSpeed() const;
        void setSpeed(float speed);

        void applyMovement(PlayerMovement movement, float elapsedTime, const Map &map);
    };

}