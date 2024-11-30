#include "vector.cpp"
#include "stdio.h"
#include "Arduino.h"

#define X_PIN A1
#define Y_PIN A0

struct ToReceive{
    Vector3 angle;

    Vector3 acceleration;
};


struct ToSend{
    SVector2 stick;

    void update(){
        stick.x = analogRead(X_PIN);
        stick.y = analogRead(Y_PIN);
    };

    void convert(uint8_t* ret){
        
    };
};