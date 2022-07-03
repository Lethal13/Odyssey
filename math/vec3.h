#ifndef ODYSSEY_MATH_VEC3_H
#define ODYSSEY_MATH_VEC3_H

#include "odyssey_math.h"

namespace ods
{

typedef struct vec3
{
    float x, y, z;

    vec3() = default;

    vec3(float a, float b, float c)
        :x(a), y(b), z(c)
    {
        ODS_ASSERT(!has_NaNs());
    }

    vec3(const vec3& v)
        :x(v.x), y(v.y), z(v.z)
    {
        ODS_ASSERT(!has_NaNs());
    }

    vec3& operator=(const vec3& v)
    {
        x = v.x;
        y = v.y;
        z = v.z;

        return *this;
    }

    vec3& operator+=(const vec3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        ODS_ASSERT(!has_NaNs());

        return *this;
    }

    vec3& operator-=(const vec3& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        ODS_ASSERT(!has_NaNs());

        return *this;
    }

    vec3& operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        ODS_ASSERT(!has_NaNs());

        return *this;
    }

    vec3& operator/=(float scalar)
    {
        ODS_ASSERT(scalar != 0);
        scalar = 1.0f / scalar;
        x *= scalar;
        y *= scalar;
        z *= scalar;
        ODS_ASSERT(!has_NaNs());

        return *this;
    }

    float& operator[](uint32_t i)
    {
        ODS_ASSERT(i >= 0 && i <= 2);
        return (&x)[i];
    }

    const float& operator[](uint32_t i) const
    {
        ODS_ASSERT(i >= 0 && i <= 2);
        return (&x)[i];
    }

    // TODO: Access with negative values [] ?

    // true = has non a number
    // false = has not a non a number.
    int32_t has_NaNs() const
    {
        return !(x == x) || !(y == y) || !(z == z);
    }

} vec3, *pVec3;

// TODO: Do asserts properly, for debug mode at least.

inline vec3 operator+(const vec3& a, const vec3& b)
{
    return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline vec3 operator-(const vec3& a, const vec3& b)
{
    return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline vec3 operator-(const vec3& v)
{
    return vec3(-v.x, -v.y, -v.z);
}

inline vec3 operator*(const vec3& v, float scalar)
{
    return vec3(v.x * scalar, v.y * scalar, v.z * scalar );
}

inline vec3 operator*(float scalar, const vec3& v)
{
    return vec3(v.x * scalar, v.y * scalar, v.z * scalar);
}

inline vec3 operator/(const vec3& v, float scalar)
{
    ODS_ASSERT(scalar != 0);
    scalar = 1.0f / scalar;
    return vec3(v.x * scalar, v.y * scalar, v.z * scalar);
}

inline vec3 operator/(float scalar, const vec3& v)
{
    ODS_ASSERT(scalar != 0);
    scalar = 1.0f / scalar;
    return vec3(v.x * scalar, v.y * scalar, v.z * scalar);
}

inline int32_t operator==(const vec3& a, const vec3& b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

inline int32_t operator!=(const vec3& a, const vec3& b)
{
    return a.x != b.x || a.y != b.y || a.z != b.z;
}

inline float magnitude(const vec3& v)
{
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

inline float magnitude_squared(const vec3& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

inline vec3 normalize(const vec3& v)
{
    return v / magnitude(v);
}

inline float dot(const vec3& a, const vec3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// The Cross Product, axb (reads "a cross b"),
// is a vector that is perpendicular to both a and b vectors.
inline vec3 cross(const vec3& a, const vec3& b)
{
    return vec3(a.y * b.z - a.z * b.y,
                    a.z * b.x - a.x * b.z,
                    a.x * b.y - a.y * b.x);
}

inline float min_component(const vec3 &v)
{
    return Min(v.x, Min(v.y, v.z));
}

inline float max_component(const vec3 &v)
{
    return Max(v.x, Max(v.y, v.z));
}

inline vec3 permute(const vec3 &v, uint32_t x, uint32_t y, uint32_t z)
{
    return vec3(v[x], v[y], v[z]);
}

}

#endif
