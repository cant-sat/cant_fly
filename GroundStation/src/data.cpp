#include "vector.cpp"
#include "stdio.h"
#include "Arduino.h"

#define X_PIN A1
#define Y_PIN A0
#define SWITCH_PIN 3

struct ToReceive{
    Vector3 angle;

    Vector3 acceleration;
};


struct ToSend{
    SVector2 stick;
    uint8_t Switch;

    void update(){
        stick.x = analogRead(X_PIN);
        stick.y = analogRead(Y_PIN);

        Switch = digitalRead(SWITCH_PIN) == 0 ? 1 : 0;
    };

    void convert(uint8_t* ret){
        memcpy(&ret[0], &stick.x, sizeof(short));
        memcpy(&ret[2], &stick.y, sizeof(short));
        memcpy(&ret[4], &Switch, sizeof(uint8_t));
    };
};