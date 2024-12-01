#include "vector.h"
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
        memcpy(&ret[12], &acceleration.x, sizeof( float));
        memcpy(&ret[16], &acceleration.y, sizeof( float));
        memcpy(&ret[20], &acceleration.z, sizeof(float));
    };
};


struct ToReceive{
    SVector2 rawStick;
    Vector2 stick;

    Vector2 midPoint = NULLVECTOR;

    void parse(uint8_t* par, bool format = true){
        memcpy(&rawStick.x, &par[0], sizeof(short));
        memcpy(&rawStick.y, &par[2], sizeof(short));

        
        

        if(format){
            if(midPoint == NULLVECTOR || par[4] != 0){
                midPoint.x = rawStick.x;
                midPoint.y = rawStick.y;


            }

            this->format();
        }
    };

    void format(){
        stick.x = FormatAxis(rawStick.x, midPoint.x);
        stick.y = FormatAxis(rawStick.y, midPoint.y);
    }

    
};

