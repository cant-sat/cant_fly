#include <Servo.h>
#include <Arduino.h>
#include "vector.h"

#define max(a, b) ( (a) > (b) ? (a) : (b))
#define min(a, b) ( (a) < (b) ? (a) : (b))

#define stepSize 1

enum responses{
    forward,
    reverse,
    none
};

int convertResponse(responses resp){
    if(resp == none){
        return 0;
    }
    if(resp == forward){
        return 1;
    }
    if(resp == reverse){
        return -1;
    }
}


struct servo
{
    private : bool init = false;

    int range;
    int mid;


    Servo servoObject;

    enum responses XAxis;
    enum responses YAxis;

    public : bool initiated(){
        return init;
    }

    public : void initiate(int pin, int range, int mid, responses XAxis, responses YAxis){
        servoObject.attach(pin, mid - range, mid + range);
        servoObject.write(mid);

        this->XAxis = XAxis;
        this->YAxis = YAxis;

        this->range = range;
        this->mid = mid;

        this->cur = mid;


        init = true;
    }
    
    public : int cur;
    
    public : void updateServo(Vector2 stick){
        if(!init){
            return;
        }
        
        int target = min(mid + range, max(mid - range, mid + stick.x * convertResponse(XAxis) * range + stick.y * convertResponse(YAxis)* range));
        
        // if(target != cur){
        //     cur += stepSize * (target-cur)/(abs(target-cur));
        // }
        servoObject.write(target);

    }
};

#define MaxPins 4

servo servoPins[MaxPins];

void UpdatePins(Vector2 stick) {
    for (int i = 0; i < MaxPins; i++) {
        if (servoPins[i].initiated()) {
            
            servoPins[i].updateServo(stick);
            
            delay(15);
        }
    }
}

void AddPin(int pin, int range, int mid, responses XAxis, responses YAxis) {
    int i = 0;
    while (i < MaxPins) {
        if (servoPins[i].initiated() == false) {
            break;
        }
        i++;
    }

    if (i == MaxPins) {
        Serial.println("could not add pin, max pin reached");
        return;
    }

    servoPins[i].initiate(pin, range, range, XAxis, YAxis);
}