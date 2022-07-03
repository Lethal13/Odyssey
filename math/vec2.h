#ifndef ODYSSEY_MATH_VEC2_H
#define ODYSSEY_MATH_VEC2_H

#include "odyssey_math.h"

namespace ods
{

typedef struct vec2
{
    float x, y;

    vec2() = default;

    vec2(float a, float b)
        :x(a), y(b)
    {
        ODS_ASSERT(!has_NaNs());
    }

    vec2(float scalar)
        :x(scalar), y(scalar)
    {
        ODS_ASSERT(!has_NaNs());
    }

    vec2(const vec2& v)
        :x(v.x), y(v.y)
    {

    }

    vec2& operator=(const vec2& v)
    {
        x = v.x;
        y = v.y;

        return *this;
    }

    vec2& operator+=(const vec2& v)
    {
        x += v.x;
        y += v.y;

        ODS_ASSERT(!has_NaNs());

        return *this;
    }

    vec2& operator-=(const vec2& v)
    {
        x -= v.x;
        y -= v.y;

        ODS_ASSERT(!has_NaNs());

        return *this;
    }

    vec2& operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;

        ODS_ASSERT(!has_NaNs());

        return *this;
    }

    vec2& operator/=(float scalar)
    {
        ODS_ASSERT(scalar != 0);

        scalar = 1.0f / scalar;

        x *= scalar;
        y *= scalar;

        ODS_ASSERT(!has_NaNs());

        return *this;
    }

    float& operator[](uint32_t i)
    {
        ODS_ASSERT(i == 0 || i == 1);
        return (&x)[i];
    }

    const float& operator[](uint32_t i) const
    {
        ODS_ASSERT(i == 0 || i == 1);
        return (&x)[i];
    }

    // TODO: Access with negative values [] ?

    // true = has non a number
    // false = has not a non a number.
    int32_t has_NaNs() const
    {
        return !(x == x) || !(y == y);
    }
} vec2, *pVec2;;

// TODO: Do asserts properly, for debug mode at least.

inline vec2 operator+(const vec2& a, const vec2& b)
{
    return vec2(a.x + b.x, a.y + b.y);
}

inline vec2 operator-(const vec2& a, const vec2& b)
{
    return vec2(a.x - b.x, a.y - b.y);
}

inline vec2 operator-(const vec2& v)
{
    return vec2(-v.x, -v.y);
}

inline vec2 operator*(const vec2& v, float scalar)
{
    return vec2(v.x * scalar, v.y * scalar);
}

inline vec2 operator*(float scalar, const vec2& v)
{
    return vec2(v.x * scalar, v.y * scalar);
}

inline vec2 operator/(const vec2& v, float scalar)
{
    ODS_ASSERT(scalar != 0);
    scalar = 1.0f / scalar;
    return vec2(v.x * scalar, v.y * scalar);
}

inline vec2 operator/(float scalar, const vec2& v)
{
    ODS_ASSERT(scalar != 0);
    scalar = 1.0f / scalar;
    return vec2(v.x * scalar, v.y * scalar);
}

inline int32_t operator==(const vec2& a, const vec2& b)
{
    return a.x == b.x && a.y == b.y;
}

inline int32_t operator!=(const vec2& a, const vec2& b)
{
    return a.x != b.x || a.y != b.y;
}

inline float magnitude(const vec2& v)
{
    return std::sqrt(v.x * v.x + v.y * v.y);
}

inline float magnitude_squared(const vec2& v)
{
    return v.x * v.x + v.y * v.y;
}

inline vec2 normalize(const vec2& v)
{
    return v / magnitude(v);
}

inline float dot(const vec2& a, const vec2& b)
{
    return a.x * b.x + a.y * b.y;
}

inline float min_component(const vec2& v)
{
    return Min(v.x, v.y);
}

inline float max_component(const vec2& v)
{
    return Max(v.x, v.y);
}

inline vec2 permute(const vec2 &v, uint32_t x, uint32_t y)
{
    return vec2(v[x], v[y]);
}

}
#endif
