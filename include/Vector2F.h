#pragma once

namespace Cursed3DEngine
{

    class Vector2F
    {
    public:
        float x;
        float y;

    public:
        Vector2F();
        Vector2F(float x, float y);
        Vector2F(const Vector2F& ref);
        Vector2F(Vector2F&& ref) noexcept;

        ~Vector2F();

        Vector2F& operator=(const Vector2F& rhs);
        Vector2F& operator=(Vector2F&& rhs) noexcept;

        Vector2F operator+(const Vector2F& rhs) const;
        Vector2F& operator+=(const Vector2F& rhs);

        Vector2F operator-(const Vector2F& rhs) const;
        Vector2F& operator-=(const Vector2F& rhs);

        Vector2F operator*(float scalar) const;
        Vector2F& operator*=(float scalar);

        bool operator==(const Vector2F& rhs) const;
        bool operator!=(const Vector2F& rhs) const;

        float getNorm() const;
        float normalize();

        float dotProduct(const Vector2F& vec) const;
    };

}