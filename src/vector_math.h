#pragma once

#if KRZ_STANDALONE
struct Vector2
{
    float x;
    float y;
};
#endif

inline static Vector2 operator*(const Vector2 a, const float scalar)
{
    Vector2 result;
    result.x = a.x * scalar;
    result.y = a.y * scalar;

    return result;
}

inline static Vector2 operator+(const Vector2 a, const Vector2 b)
{
    Vector2 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;

    return result;
}

inline static Vector2 operator-(const Vector2 a, const Vector2 b)
{
    Vector2 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;

    return result;
}

inline static Vector2 operator-(const Vector2 a)
{
    Vector2 result;
    result.x = -a.x;
    result.y = -a.y;

    return result;
}

float clampFloat(float val, float min, float max)
{
    if (val > max)
        return max;
    else if (val < min)
        return min;
    return val;
}