#include "RF24.h"
#include "data.cpp"

#define CE_PIN D4
#define CSN_PIN D2

#define DEBUG_PIN D0

RF24 radio(CE_PIN, CSN_PIN);

uint8_t send[5];
uint8_t receive[24];

uint8_t PlaneAdress[6] = "Plane";
uint8_t GroundAdress[6] = "GStat";

ToSend sendStruct;

ToReceive receiveStruct;

long int failedPings = 0;
long int allPings = 0;

void setup()
{
  Serial.begin(9600);

  while (!radio.begin())
  {
    Serial.println(F("Radio hardware is not responding!"));
    delay(1000);
  }

  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(108);
  radio.setDataRate(RF24_250KBPS);

  radio.setPayloadSize(24);

  radio.openWritingPipe(GroundAdress);
  radio.openReadingPipe(1, PlaneAdress);

  pinMode(JOYSTICK_READ_PIN, INPUT);
  pinMode(JOYSTICK_X_PIN, OUTPUT);
  pinMode(JOYSTICK_Y_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(DEBUG_PIN, OUTPUT);
}

void loop()
{
  radio.stopListening();
  delay(5);

  sendStruct.update();
  sendStruct.convert(send);

allPings += 1;
  bool success = radio.write(&send, sizeof(send));
  if (success)
  {
    
    //Serial.println(F("Sent ping"));

    radio.startListening();
    unsigned long timeout = millis() + 2000;
    while (!radio.available() && millis() < timeout)
    {
      delay(5);
    }

    if (radio.available())
    {
      digitalWrite(DEBUG_PIN, HIGH);
      radio.read(&receive, sizeof(receive));
      //Serial.println(F("Received: "));

      // for (int i = 0; i < sizeof(receive); i += 4)
      // { // Increment by 4 each iteration
      //   float value;
      //   memcpy(&value, &receive[i], sizeof(float)); // Copy 4 bytes starting at index `i`

      //   Serial.println(value); // Print the float value
      // }
      //Serial.println();
      digitalWrite(DEBUG_PIN, HIGH);
    }
    else
    {
      digitalWrite(DEBUG_PIN, LOW);
      //Serial.println(F("No pong received"));
      failedPings += 1;
    }
  }
  else
  {
    digitalWrite(DEBUG_PIN, LOW);
    //Serial.println(F("Failed to send ping"));
    failedPings += 1;
  }

  if(false){
    Serial.print("Total Packages: ");
  Serial.print(allPings);
  Serial.print("   Lost Packages: ");
  Serial.println(failedPings);
  
  Serial.print("Package loss: ");
  Serial.print((100 * failedPings/allPings));
  Serial.println("%");
  }

  delay(20);
}
