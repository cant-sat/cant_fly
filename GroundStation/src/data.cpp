#include "vector.cpp"
#include "stdio.h"
#include "Arduino.h"
#include "ArduinoJson.h"

#define JOYSTICK_X_PIN D3
#define JOYSTICK_Y_PIN D8
#define JOYSTICK_READ_PIN A0
#define SWITCH_PIN D1

struct ToReceive
{
    Vector3 angle;

    Vector3 acceleration;

    float temp;

    unsigned long timestamp;

    void parse(uint8_t *par)
    {
        memcpy(&angle.x, &par[0], sizeof(float));
        memcpy(&angle.y, &par[4], sizeof(float));
        memcpy(&angle.z, &par[8], sizeof(float));

        memcpy(&acceleration.x, &par[12], sizeof(float));
        memcpy(&acceleration.y, &par[16], sizeof(float));
        memcpy(&acceleration.z, &par[20], sizeof(float));

        memcpy(&temp, &par[24], sizeof(float));

        timestamp = millis();
    };

    String stringify()
    {
    String output = "{";

    // Add temperature
    output += "\"temp\":";
    output += String(temp, 2); // 2 decimal places

    // Add timestamp
    output += ",\"timestamp\":";
    output += String(timestamp);

    // Add angle
    output += ",\"angle\":[";
    output += String(angle.x, 2) + ",";
    output += String(angle.y, 2) + ",";
    output += String(angle.z, 2);
    output += "]";

    // Add acceleration
    output += ",\"acc\":[";
    output += String(acceleration.x, 2) + ",";
    output += String(acceleration.y, 2) + ",";
    output += String(acceleration.z, 2);
    output += "]";

    output += "}";

    return output;
    };
};

struct ToSend
{
    SVector2 stick;
    uint8_t Switch;

    void update()
    {
        digitalWrite(JOYSTICK_Y_PIN, LOW);
        digitalWrite(JOYSTICK_X_PIN, HIGH);
        stick.x = analogRead(JOYSTICK_READ_PIN);

        digitalWrite(JOYSTICK_Y_PIN, HIGH);
        digitalWrite(JOYSTICK_X_PIN, LOW);
        stick.y = analogRead(JOYSTICK_READ_PIN);

        digitalWrite(JOYSTICK_Y_PIN, LOW);
        digitalWrite(JOYSTICK_X_PIN, LOW);

        Switch = digitalRead(SWITCH_PIN) == 0 ? 1 : 0;
    };

    void convert(uint8_t *ret)
    {
        memcpy(&ret[0], &stick.x, sizeof(short));
        memcpy(&ret[2], &stick.y, sizeof(short));
        memcpy(&ret[4], &Switch, sizeof(uint8_t));
    };
};