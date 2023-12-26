#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <ESP8266WiFiMulti.h>
#include <Arduino.h>
#include <ArduinoJson.h>

const char* ssid     = "SanUSB";         // The SSID (name) of the Wi-Fi network you want to connect 2.4ghz!
const char* password = "silveira19";     // The password of the Wi-Fi network
#define ledPin 2

#include <WebSocketsClient.h>
#include <SocketIOclient.h>
#include <Hash.h>
SocketIOclient socketIO;
#define USE_SERIAL Serial

void socketIOEvent(socketIOmessageType_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case sIOtype_DISCONNECT:
      USE_SERIAL.printf("SanUSB Disconnected!\n");
      break;
    case sIOtype_CONNECT:
      USE_SERIAL.printf("SanUSB Connected to url: %s\n", payload);

      // join default namespace (no auto join in Socket.IO V3)
      socketIO.send(sIOtype_CONNECT, "/");
      break;
    case sIOtype_EVENT:
      USE_SERIAL.printf("SanUSB get event: %s\n", payload);
            
            char subs[5];
            memcpy(subs, &payload[16], 4); 
            subs[4] = '\0';
            
            Serial.printf("LED status: %s\n", subs);
            if(strcmp(subs,"true") == 0) {
              digitalWrite(ledPin, HIGH);
            } else {
              digitalWrite(ledPin, LOW);
            }
            delay(500);
      break;
    case sIOtype_ACK:
      USE_SERIAL.printf("SanUSB get ack: %u\n", length);
      hexdump(payload, length);
      break;
    case sIOtype_ERROR:
      USE_SERIAL.printf("SanUSB get error: %u\n", length);
      hexdump(payload, length);
      break;
    case sIOtype_BINARY_EVENT:
      USE_SERIAL.printf("SanUSB get binary: %u\n", length);
      hexdump(payload, length);
      break;
    case sIOtype_BINARY_ACK:
      USE_SERIAL.printf("SanUSB get binary ack: %u\n", length);
      hexdump(payload, length);
      break;
  }
}
void setup() {
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  pinMode(2, OUTPUT);
  delay(10);
  Serial.println('\n');

  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer


  //put your server ip address
   socketIO.begin("192.168.1.193", 4200, "/socket.io/?EIO=4");

    // event handler
    socketIO.onEvent(socketIOEvent);

}

void loop() { 
    socketIO.loop();
  //
  }
