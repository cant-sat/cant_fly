#ifndef SERVO_MANAGER_H
#define SERVO_MANAGER_H

#include <Servo.h>
#include "vector.h"

// Define enumeration for response types
enum responses {
    forward,
    reverse,
    none
};

// Convert the response enumeration to an integer value
int convertResponse(responses resp);

// Define the servo structure
struct servo {
private:
    bool init = false;

    int min;
    int max;
    int mid;

    Servo servoObject;

    enum responses XAxis;
    enum responses YAxis;

public:
    // Check if the servo has been initialized
    bool initiated();

    // Initialize the servo with the given parameters
    void initiate(int pin, int range, int mid, responses XAxis, responses YAxis);

    // Update the servo based on a Vector2 input
    void updateServo(Vector2 stick);
};

// Define maximum number of servo pins
#define MaxPins 4

// Servo objects array
extern servo servoPins[MaxPins];

// Function to update all pins with a Vector2 input
void UpdatePins(Vector2 stick);

// Function to add a servo pin
void AddPin(int pin, int range, int mid, responses XAxis, responses YAxis);

#endif // SERVO_MANAGER_H