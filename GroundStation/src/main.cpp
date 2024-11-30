#include "RF24.h"
#include "data.cpp"

#define CE_PIN 7
#define CSN_PIN 8



RF24 radio(CE_PIN, CSN_PIN);

uint8_t send[4];
uint8_t receive[24];

uint8_t PlaneAdress[6] = "Plane";
uint8_t GroundAdress[6] = "GStat";

ToSend sendStruct;

ToReceive receiveStruct;

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
}

void loop()
{
  radio.stopListening();
  delay(5);

  bool success = radio.write(&send, sizeof(send));
  if (success)
  {
    Serial.println(F("Sent ping"));

    radio.startListening();
    unsigned long timeout = millis() + 2000;
    while (!radio.available() && millis() < timeout)
    {
      delay(5);
    }

if (radio.available()) {
    radio.read(&receive, sizeof(receive));
    Serial.println(F("Received: "));
    
    for (int i = 0; i < sizeof(receive); i += 4) { // Increment by 4 each iteration
        float value;
        memcpy(&value, &receive[i], sizeof(float)); // Copy 4 bytes starting at index `i`

        Serial.println(value); // Print the float value
    }
    Serial.println();
}
    else
    {
      Serial.println(F("No pong received"));
    }
  }
  else
  {
    Serial.println(F("Failed to send ping"));
  }

  delay(1000);
}
