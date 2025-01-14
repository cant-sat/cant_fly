#include "vector.cpp"
#include "stdio.h"
#include "Arduino.h"

#define JOYSTICK_X_PIN D3
#define JOYSTICK_Y_PIN D8
#define JOYSTICK_READ_PIN A0
#define SWITCH_PIN D1

struct ToReceive{
    Vector3 angle;

    Vector3 acceleration;
};


struct ToSend{
    SVector2 stick;
    uint8_t Switch;

    void update(){
        digitalWrite(JOYSTICK_Y_PIN,  LOW);
        digitalWrite(JOYSTICK_X_PIN,  HIGH);
        stick.x = analogRead(JOYSTICK_READ_PIN);

        digitalWrite(JOYSTICK_Y_PIN,  HIGH);
        digitalWrite(JOYSTICK_X_PIN,  LOW);
        stick.y = analogRead(JOYSTICK_READ_PIN);

        digitalWrite(JOYSTICK_Y_PIN,  LOW);
        digitalWrite(JOYSTICK_X_PIN,  LOW);

        Switch = digitalRead(SWITCH_PIN) == 0 ? 1 : 0;
    };

    void convert(uint8_t* ret){
        memcpy(&ret[0], &stick.x, sizeof(short));
        memcpy(&ret[2], &stick.y, sizeof(short));
        memcpy(&ret[4], &Switch, sizeof(uint8_t));
    };
};