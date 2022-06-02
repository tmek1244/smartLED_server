#include <Arduino.h>

#include <WiFi.h>
// #include <analogWrite.h>
#include "secrets.h"

// WiFiServer wifiServer(80);
IPAddress localIP(192, 168, 0, 80);
// Set your Gateway IP address
IPAddress gateway(192, 168, 0, 1);

IPAddress subnet(255, 255, 0, 0);


WiFiUDP Udp;
const int udpPort = 3333;
char packetBuffer[255];

const int redPin = 25;
const int greenPin = 26;
const int bluePin = 14;

// setting PWM properties
const int freq = 1000;
const int ledChannelRed = 0;
const int ledChannelGreen = 1;
const int ledChannelBlue = 2;
const int resolution = 8;

unsigned long previousMillis = 0;
unsigned long interval = 30000;  // 30s

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info);
void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);

void setUpLeds();
void parsePacket(int packetSize);


void setup() {
  Serial.begin(115200);

  delay(1000);

  // ESP32 station connected to AP
  WiFi.onEvent(WiFiStationConnected, ARDUINO_EVENT_WIFI_STA_CONNECTED);
  // ESP32 station disconnected from AP
  WiFi.onEvent(WiFiStationDisconnected, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);


  if (!WiFi.config(localIP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  WiFi.begin(ssid, password);

  setUpLeds();
}

int red;
int green;
int blue;

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    int packetSize = Udp.parsePacket();

    if (packetSize) {
      parsePacket(packetSize);
    }
  }

  delay(10);
}


void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());

  Udp.begin(WiFi.localIP(), udpPort);
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("Disconnected from WiFi access point");
  Serial.println("Trying to Reconnect");
  WiFi.begin(ssid, password);
}

void setUpLeds() {
  ledcSetup(ledChannelRed, freq, resolution);
  ledcSetup(ledChannelGreen, freq, resolution);
  ledcSetup(ledChannelBlue, freq, resolution);

  ledcAttachPin(redPin, ledChannelRed);
  ledcAttachPin(greenPin, ledChannelGreen);
  ledcAttachPin(bluePin, ledChannelBlue);
}

void parsePacket(int packetSize) {
  Serial.printf("Received packet of size %d\n", packetSize);

  int len = Udp.read(packetBuffer, 255);

  if (packetSize == 4) {
    red = packetBuffer[1];
    green = packetBuffer[2];
    blue = packetBuffer[3];

    ledcWrite(ledChannelRed, red);
    ledcWrite(ledChannelGreen, green);
    ledcWrite(ledChannelBlue, blue);
  }

  Serial.printf("Red: %d\nGreen: %d\nBlue: %d\n", red, green, blue);
}