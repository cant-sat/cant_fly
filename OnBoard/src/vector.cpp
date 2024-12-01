#include "vector.h"

const Vector2 NULLVECTOR(0, 0);

Vector2::Vector2(float x, float y) : x(x), y(y) {}

bool Vector2::operator==(const Vector2 &other) const {
    return x == other.x && y == other.y;
}

bool Vector2::operator!=(const Vector2 &other) const {
    return !(*this == other);
}

Vector2 &Vector2::operator=(const Vector2 &other) {
    if (this != &other) { // Avoid self-assignment
        x = other.x;
        y = other.y;
    }
    return *this; // Return *this to allow chaining
}

SVector2::SVector2(short x, short y) : x(x), y(y) {}

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

float FormatAxis(float cur, float mid, float min, float max) {
    if (cur < mid) {
        return (cur - mid) / (mid - min);
    } else {
        return (cur - mid) / (max - mid);
    }
}