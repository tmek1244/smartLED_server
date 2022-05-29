#include "WiFi.h"
#include <analogWrite.h>
#include "secrets.h"
 
WiFiServer wifiServer(80);
IPAddress local_IP(192, 168, 0, 80);
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

void setup() {
 
  Serial.begin(115200);
 
  delay(1000);

  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());

  Udp.begin(WiFi.localIP(), udpPort); 
 
  wifiServer.begin();
  

  ledcSetup(ledChannelRed, freq, resolution);
  ledcSetup(ledChannelGreen, freq, resolution);
  ledcSetup(ledChannelBlue, freq, resolution);

  ledcAttachPin(redPin, ledChannelRed);
  ledcAttachPin(greenPin, ledChannelGreen);
  ledcAttachPin(bluePin, ledChannelBlue);
  Serial.println(WiFi.macAddress());
}

int red;
int green;
int blue;


 
void loop() {

  int packetSize = Udp.parsePacket();

  if(packetSize) {
    Serial.print("Received packet of size ");

    Serial.println(packetSize);

    int len = Udp.read(packetBuffer, 255);

    if(packetSize == 4){
      red = packetBuffer[1];
      green = packetBuffer[2];
      blue = packetBuffer[3];

//      analogWrite(redPin,red);
//      analogWrite(bluePin,blue);
//      analogWrite(greenPin,green);
      ledcWrite(ledChannelRed, red);
      ledcWrite(ledChannelGreen, green);
      ledcWrite(ledChannelBlue, blue);
    }
    

    if (len > 0) {
      packetBuffer[len] = 0;
    }

    Serial.println("Contents:");

    Serial.println(red);
    Serial.println(green);
    Serial.println(blue);
  }
  delay(10);
 
//  WiFiClient client = wifiServer.available();
// 
//  if (client) {
// 
//    while (client.connected()) {
// 
//      while (client.available()>0) {
//        char c = client.read();
//        Serial.write(c);
//      }
// 
//      delay(10);
//    }
// 
//    client.stop();
//    Serial.println("Client disconnected");
// 
//  }
}
