#include "RF24.h"
#include "data.cpp"

#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>


#define CE_PIN D4
#define CSN_PIN D2

#define DEBUG_PIN D0

RF24 radio(CE_PIN, CSN_PIN);

uint8_t send[5];
uint8_t receive[28];

uint8_t PlaneAdress[6] = "Plane";
uint8_t GroundAdress[6] = "GStat";

ToSend sendStruct;

ToReceive receiveStruct;

long int failedPings = 0;
long int allPings = 0;

// Replace with your network credentials
const char *ssid = "HUAWEI-dr64";
const char *password = "588Y293w";

// WebSocket server details
const char *websockets_server_host = "192.168.100.177";
const uint16_t websockets_server_port = 443; // Make sure your server is listening on this port
const char *websockets_server_path = "/";    // WebSocket path if needed

// WebSocket server token
const char *token = "token";

// Create a WebSocket client object
WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case WStype_DISCONNECTED:
    Serial.println("WebSocket Disconnected");
    break;
  case WStype_CONNECTED:
    Serial.println("WebSocket Connected");
    webSocket.sendTXT(token);
    // Send a message when connected
    break;
  case WStype_TEXT:
    Serial.println("WebSocket message received");
    Serial.println((char *)payload);
    break;
  case WStype_BIN:
    Serial.println("WebSocket binary message received");
    break;
  case WStype_ERROR:
    Serial.println("Error");
    break;
  }
}

void setup()
{
  Serial.begin(9600);

  while (!radio.begin())
  {
    Serial.println(F("Radio hardware is not responding!"));
    delay(1000);
  }

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  // Initialize WebSocket
  webSocket.begin(websockets_server_host, websockets_server_port, websockets_server_path);
  webSocket.onEvent(webSocketEvent);

  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(108);
  radio.setDataRate(RF24_250KBPS);

  radio.setPayloadSize(28);

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

    // Serial.println(F("Sent ping"));

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
      receiveStruct.parse(receive);
      digitalWrite(DEBUG_PIN, HIGH);

      if(webSocket.isConnected() || true){
                String json = receiveStruct.stringify();
                Serial.println(json);
        webSocket.sendTXT(json);
      }
    }
    else
    {
      digitalWrite(DEBUG_PIN, LOW);
      // Serial.println(F("No pong received"));
      failedPings += 1;
    }
  }
  else
  {
    digitalWrite(DEBUG_PIN, LOW);
    // Serial.println(F("Failed to send ping"));
    failedPings += 1;
  }

  if (false)
  {
    Serial.print("Total Packages: ");
    Serial.print(allPings);
    Serial.print("   Lost Packages: ");
    Serial.println(failedPings);

    Serial.print("Package loss: ");
    Serial.print((100 * failedPings / allPings));
    Serial.println("%");
  }

  delay(20);
}
