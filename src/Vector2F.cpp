#include "Vector2F.h"

#include <cmath>

using namespace Cursed3DEngine;

Vector2F::Vector2F() : x(0.0f), y(0.0f) {}

Vector2F::Vector2F(float x, float y) : x(x), y(y) {}

Vector2F::Vector2F(const Vector2F& ref) : x(ref.x), y(ref.y) {}

Vector2F::Vector2F(Vector2F&& ref) noexcept : x(ref.x), y(ref.y)
{
    ref.x = 0.0f;
    ref.y = 0.0f;
}

Vector2F::~Vector2F() {}

Vector2F& Vector2F::operator=(const Vector2F& rhs)
{
    if (this != &rhs)
    {
        x = rhs.x;
        y = rhs.y;
    }
    return *this;
}

Vector2F& Vector2F::operator=(Vector2F&& rhs) noexcept
{
    if (this != &rhs)
    {
        x = rhs.x;
        y = rhs.y;
        rhs.x = 0.0f;
        rhs.y = 0.0f;
    }
    return *this;
}

Vector2F Vector2F::operator+(const Vector2F& rhs) const
{
    return Vector2F(this->x + rhs.x, this->y + rhs.y);
}

Vector2F& Vector2F::operator+=(const Vector2F& rhs)
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}

Vector2F Vector2F::operator-(const Vector2F& rhs) const
{
    return Vector2F(this->x - rhs.x, this->y - rhs.y);
}

Vector2F& Vector2F::operator-=(const Vector2F& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

Vector2F Vector2F::operator*(float scalar) const
{
    return Vector2F(x * scalar, y * scalar);
}

Vector2F& Vector2F::operator*=(float scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
}

bool Vector2F::operator==(const Vector2F& rhs) const
{
    return this->x == rhs.x && this->y == rhs.y;
}

bool Vector2F::operator!=(const Vector2F& rhs) const
{
    return !(*this == rhs);
}

float Vector2F::getNorm() const
{
    return std::sqrt(x * x + y * y);
}

float Vector2F::normalize()
{
    float norm = getNorm();
    if (norm > 0.0f) {
        x /= norm;
        y /= norm;
    }
    return norm;
}

float Vector2F::dotProduct(const Vector2F& vec) const
{
    return x * vec.x + y * vec.y;
}