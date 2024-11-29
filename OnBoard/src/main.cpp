/*
 * See documentation at https://nRF24.github.io/RF24
 * See License information at root directory of this library
 * Author: Brendan Doherty (2bndy5)
 */



#include "Wire.h"
#include "MPU6050_tockn.h"

MPU6050 mpu6050(Wire);

#include "RF24.h"

#define CE_PIN D4
#define CSN_PIN D3

RF24 radio(CE_PIN, CSN_PIN);

uint8_t receive[8] = {'a', 's','d', 'f','g', 'h','j', 'k'};

uint8_t send[4] = {'a','s','d','f'};

uint8_t PlaneAdress[6] = "Plane";
uint8_t GroundAdress[6] = "GStat";

void setup()
{

  Serial.begin(9600);

  // initialize the transceiver on the SPI bus
  while(!radio.begin())
  {
    Serial.println(F("radio hardware is not responding!!"));
    delay(1000);
    
  }

  // Set the PA Level low to try preventing power supply related problems
  // because these examples are likely run with nodes in close proximity to
  // each other.
  radio.setPALevel(RF24_PA_LOW); // RF24_PA_MAX is default.

  radio.setPayloadSize(sizeof(receive));

  //comm between plane -> ground
  radio.openWritingPipe(PlaneAdress);



 //comm between ground -> plane
  radio.openReadingPipe(1, GroundAdress); // using pipe 1



  radio.startListening(); // put radio in RX mode
  


  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

} 

unsigned long timer = 0;

void loop()
{

  // This device is a RX node
  uint8_t pipe;
  if (radio.available(&pipe))
  {           
         
                  // is there a payload? get the pipe number that received it
    uint8_t bytes = radio.getPayloadSize(); // get the size of the payload

    
    radio.read(&receive, bytes);            // fetch payload from FIFO
    Serial.print(F("Received "));
    Serial.print(bytes); // print the size of the payload
    Serial.print(F(" bytes : "));
    Serial.println(*receive); // print the payload's value

    radio.openWritingPipe(PlaneAdress);
    radio.stopListening();

    delay(100);
    

    bool report = radio.write(&receive, sizeof(receive));

    if(report){
      Serial.println("ponged") ;
    }
    else{
      Serial.println("could not send pong back");
    }

    radio.openReadingPipe(1, GroundAdress); // using pipe 1
    radio.startListening();
  }

  mpu6050.update();

  if(timer < millis()){
    timer = millis() + 2000;

    Serial.println("---");
    Serial.print("angle X :");
    Serial.println(mpu6050.getAngleX());
    Serial.print("angle Y :");
    Serial.println(mpu6050.getAngleY());
    Serial.print("angle Z :");
    Serial.println(mpu6050.getAngleZ());
    Serial.println("---");
  }

} // loop
