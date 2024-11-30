#include "vector.cpp"
#include "cstring"

#include "stdio.h"
#include "MPU6050_tockn.h"


struct ToSend{
    Vector3 angle;

    Vector3 acceleration;

    void update(MPU6050 mpu6050){
        mpu6050.update();

        angle.x = mpu6050.getAngleX();
        angle.y = mpu6050.getAngleY();
        angle.z = mpu6050.getAngleZ();


        acceleration.x = mpu6050.getAccX();
        acceleration.y = mpu6050.getAccY();
        acceleration.z = mpu6050.getAccZ();
    };

    void convert(uint8_t* ret){
        // Serialize `angle`
        memcpy(&ret[0], &angle.x, sizeof(float));
        memcpy(&ret[4], &angle.y, sizeof(float));
        memcpy(&ret[8], &angle.z, sizeof(float));

        // Serialize `acceleration`
        memcpy(&ret[12], &acceleration.x, sizeof(float));
        memcpy(&ret[16], &acceleration.y, sizeof(float));
        memcpy(&ret[20], &acceleration.z, sizeof(float));
    };
};


struct ToReceive{
    Vector2 stick;
};