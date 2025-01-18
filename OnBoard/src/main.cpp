#include "Wire.h"
#include "MPU6050_tockn.h"
#include "RF24.h"

#include "data.cpp"
#include "servo.h"

MPU6050 mpu6050(Wire);

#define CE_PIN 7
#define CSN_PIN 8

RF24 radio(CE_PIN, CSN_PIN);

uint8_t send[28] = {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k'};
uint8_t receive[5];

uint8_t PlaneAdress[6] = "Plane";
uint8_t GroundAdress[6] = "GStat";

ToSend sendStruct;

ToReceive receiveStruct;



void setup() {
  Serial.begin(9600);

  while (!radio.begin()) {
    Serial.println(F("Radio hardware is not responding!"));
    delay(1000);
  }

  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(108);
  radio.setDataRate(RF24_250KBPS);

  radio.setPayloadSize(28);

  
  radio.openWritingPipe(PlaneAdress);
  radio.openReadingPipe(1, GroundAdress);
  radio.startListening();

  Wire.begin();

  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);


  
  //AddPin(3, 20, 90, forward, forward);
  //AddPin(4, 20, 90, reverse, reverse);
  //AddPin(5, 20, 90, forward, none);
}

unsigned long timer = 0;

void loop() {
  radio.startListening();
  uint8_t pipe;
  if (radio.available(&pipe)) {
    radio.read(&receive, sizeof(receive));
    //Serial.println(F("Received: "));
    // for (unsigned int i = 0; i < sizeof(receive); i += 2) {
    //   short value;
    //   if(i == 0){
    //     receiveStruct.stick.x = value;
    //   }
    //   else if(i == 2){
    //     receiveStruct.stick.y = value;
    //   }
    //   memcpy(&value, &receive[i], sizeof(short));
    //   Serial.println(value);
    // }

    receiveStruct.parse(receive);

    Serial.println(receiveStruct.stick.x);
    Serial.println(receiveStruct.stick.y);

    UpdatePins(receiveStruct.stick);
    timer = millis() + 1000; 

    radio.stopListening();

    sendStruct.convert(send, mpu6050);
    bool success = radio.write(&send, sizeof(send));
    if (success) {

    } else {
      Serial.println(F("Failed to send pong"));
    }

    delay(5);
    radio.startListening();
  }

  if(millis() >= timer){
    receiveStruct.stick = NULLVECTOR;
  }
  //UpdatePins(receiveStruct.stick);

  delay(20);
  
}
