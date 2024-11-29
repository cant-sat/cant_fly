
#include "RF24.h"

#define CE_PIN 7
#define CSN_PIN 8

RF24 radio(CE_PIN, CSN_PIN);

uint8_t address[][6] = {"1Node", "2Node"};
// It is very helpful to think of an address as a path instead of as
// an identifying device destination

// to use different addresses on a pair of radios, we need a variable to
// uniquely identify which address this radio will use to transmit
bool radioNumber = 0; // 0 uses address[0] to transmit, 1 uses address[1] to transmit

// Used to control whether this node is sending or receiving
bool role = false; // true = TX role, false = RX role

// For this example, we'll be using a payload containing
// a single float number that will be incremented
// on every successful transmission
uint8_t send[8] = {'a', 's','d', 'f','g', 'h','j', 'k'};

uint8_t receive[4] = {'a','s','d','f'};



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

  radio.setPayloadSize(sizeof(send));

  // comm between plane -> ground
  radio.openReadingPipe(1, PlaneAdress); // using pipe 1

  // comm between ground -> plane
  radio.openWritingPipe(GroundAdress);

  radio.stopListening(); // put radio in TX mode
}

void loop()
{

  // This device is a TX node
  bool report = radio.write(&send, sizeof(send)); // transmit & save the report

  if (report)
  {
    Serial.print(F("Transmission successful! ")); // payload was delivered
    Serial.print(F("Sent: "));
    Serial.println(*send); // print payload sent

      // comm between plane -> ground
  radio.openReadingPipe(1, PlaneAdress); // using pipe 1
    radio.startListening();
    unsigned long timeout = millis() + 2000;

    uint8_t pipe;
    while (timeout < millis() && !radio.available(&pipe))
    {
      // waits for a response or timeout
    }
    if (radio.available(&pipe))
    {
      uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
      radio.read(&send, bytes);            // fetch payload from FIFO
      Serial.print(F("Received "));
      Serial.print(bytes); // print the size of the payload
      Serial.print(F(": "));
      Serial.println(*send); // print the payload's value
    }
    // timeout
    else
    {
      Serial.println("got no answer back");
    }

      // comm between ground -> plane
  radio.openWritingPipe(GroundAdress);
    radio.stopListening();
  }
  else
  {
    Serial.println(F("Transmission failed or timed out")); // payload was not delivered
  }

  // to make this example readable in the serial monitor
  delay(1000); // slow transmissions down by 1 second

} // loop
