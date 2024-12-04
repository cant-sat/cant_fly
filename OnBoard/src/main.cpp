#include "Wire.h"
#include "MPU6050_tockn.h"
#include "RF24.h"

#include "data.cpp"

MPU6050 mpu6050(Wire);

#define CE_PIN 7
#define CSN_PIN 8

RF24 radio(CE_PIN, CSN_PIN);

uint8_t send[24] = {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k'};
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

  radio.setPayloadSize(24);

  radio.openWritingPipe(PlaneAdress);
  radio.openReadingPipe(1, GroundAdress);
  radio.startListening();

  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}

unsigned long timer = 0;

void loop() {
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


    radio.stopListening();
    delay(5);

    sendStruct.convert(send);
    bool success = radio.write(&send, sizeof(send));
    if (success) {
      // Serial.println(F("Pong sent!"));
    } else {
      Serial.println(F("Failed to send pong"));
    }

    delay(5);
    radio.startListening();
  }

  sendStruct.update(mpu6050);

  
}
