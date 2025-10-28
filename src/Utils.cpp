#include "Utils.h"

using namespace Cursed3DEngine;

float TrigUtils::getNormalizedAngle(float radians)
{
    while (radians >= TrigUtils::_2PI) {
        radians -= TrigUtils::_2PI;
    }
    while (radians < 0) {
        radians += TrigUtils::_2PI;
    }
    return radians;
}