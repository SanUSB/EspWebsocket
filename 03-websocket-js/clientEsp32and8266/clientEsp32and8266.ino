#include <Arduino.h>
#if defined(ESP8266)  
    #include <ESP8266WiFi.h>  
#elif defined(ESP32)
    #include <WiFi.h>
#endif

#include <WebSocketsClient.h> //https://github.com/Links2004/arduinoWebSockets
WebSocketsClient webSocket;
const char* ssid = "SanUSB";
const char* password = "********";
const char* ip_host = "192.168.1.193";
const uint16_t port = 3000;
const int LED = 2;
const int BTN = 0;
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("SanUSB Disconnected!\n");
      break;
    case WStype_CONNECTED:
      {
        Serial.printf("SanUSB Connected to url: %s\n", payload);
      }
      break;
    case WStype_TEXT:
      Serial.printf("SanUSB get text: %s\n", payload);
      if (strcmp((char*)payload, "LED_ON") == 0) {
        digitalWrite(LED, 0); // invertido, acende com "0"
      } else if (strcmp((char*)payload, "LED_OFF") == 0) {
        digitalWrite(LED, 1); 
      }
      break;
    case WStype_BIN:
      Serial.printf("SanUSB get binary length: %u\n", length);
      break;
  }
}
void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BTN, INPUT);
  Serial.begin(115200);
  Serial.println("Esp Websocket Client");
  #if defined(ESP8266)
  WiFi.hostname("Esp-SanUSB.org");
  Serial.println(WiFi.hostname());
  #elif defined(ESP32)
  WiFi.setHostname("Esp-SanUSB.org");
  Serial.println(WiFi.getHostname());
  #endif
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  webSocket.begin(ip_host, port);
  webSocket.onEvent(webSocketEvent);
}
void loop() {
  webSocket.loop();
  static bool isPressed = false;
  if (!isPressed && digitalRead(BTN) == 0) { 
    isPressed = true;
    webSocket.sendTXT("BTN_PRESSED");
  } else if (isPressed && digitalRead(BTN)) { 
    isPressed = false;
    webSocket.sendTXT("BTN_RELEASE");
  }
}
