#ifndef VECTOR_H
#define VECTOR_H

struct Vector2 {
    float x;
    float y;

    Vector2(float x = 0, float y = 0);
    bool operator==(const Vector2 &other) const;
    bool operator!=(const Vector2 &other) const;
    Vector2 &operator=(const Vector2 &other);
};

extern const Vector2 NULLVECTOR;

struct SVector2 {
    short x;
    short y;

    SVector2(short x = 0, short y = 0);
};

struct Vector3 {
    float x;
    float y;
    float z;

    Vector3(float x = 0, float y = 0, float z = 0);
};

float FormatAxis(float cur, float mid, float min = 30, float max = 1023);

#endif // VECTOR_H
