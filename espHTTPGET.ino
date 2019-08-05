/*
 mqtt_openHAB_2 
 Example program for esp8266-12 on the yellow carrier board.
 Used to interact over local intranet with a mosquitto server and openHAB running on a seperate computer.

 Ver. 2 Changed mqtt messaging structure to support multiple esp boards addressed by openHAB at different IP addresses

  Copyright (c) 2015 R. Wynn. All rights reserved.
  This is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This software is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA


*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "itekdung_secure";
const char* password = "semogacepatlulus";

byte mqttServer[] = { 192, 168, 1, 114 };//Hardcode of mosquitto server IP on local intranet connected computer.
byte IP[]         = { 192, 168, 1, 111 };//Hardcode for this esp8266 node
byte Netmask[]    = { 255, 255, 255, 0 };//Local router info
byte Gateway[]    = { 192, 168, 1, 1 };  //Local router info

WiFiClient   wifiClient;
PubSubClient mqttClient(mqttServer, 1883, mqttCallback, wifiClient);

long millis_now;         // For non-blocking delay
long millis_prev;        // For non-blocking delay

long LEDredValue = 0;
long LEDgreenValue = 0;
long LEDblueValue = 0;

long lightsensor = 0;

//----------------------------------------------------------------------------
// Callback payload parsing inspired from code found at http://blue-pc.net
//----------------------------------------------------------------------------
void mqttCallback(char* topic, byte* payload, unsigned int length) {

 int i = 0;
 char message_buff[100];

 for(i=0; i<length; i++) {
 message_buff[i] = payload[i];
 }
 message_buff[i] = '\0';
 
 String msgString = String(message_buff);
 
 //Serial.print("topic: ");
 //Serial.println(String(topic));
 //Serial.print("payload: ");
 //Serial.println(String(msgString));

 if (String(topic) == "openHAB/esp_111/RED") {
   LEDredValue = round(msgString.toInt() * 10.2);
   //Serial.print("Red: ");
   //Serial.println(LEDredValue);
   analogWrite(15,LEDredValue);
 }
 if (String(topic) == "openHAB/esp_111/GREEN") {
   LEDgreenValue = round(msgString.toInt() * 10.2);
   //Serial.print("Green: ");
   //Serial.println(LEDgreenValue);
   analogWrite(12,LEDgreenValue);
 }
 if (String(topic) == "openHAB/esp_111/BLUE") {
   LEDblueValue = round(msgString.toInt() * 10.2);
   //Serial.print("Blue: ");
   //Serial.println(LEDblueValue);
   analogWrite(13,LEDblueValue);
 }
 if (String(topic) == "openHAB/esp_111/led1") {
  if (msgString == "ON") digitalWrite( 2, 0 );  //red leds are inverted output 
  else                   digitalWrite( 2, 1 );  
 }
 if (String(topic) == "openHAB/esp_111/led2") {
  if (msgString == "ON") digitalWrite( 0, 0 );  //red leds are inverted output
  else                   digitalWrite( 0, 1 );  
 }
 if (String(topic) == "openHAB/esp_111/led3") {
  if (msgString == "ON") digitalWrite( 4, 0 );  //red leds are inverted output
  else                   digitalWrite( 4, 1 );  
 }
 if (String(topic) == "openHAB/esp_111/led4") {
  if (msgString == "ON") digitalWrite( 5, 0 );  //red leds are inverted output
  else                   digitalWrite( 5, 1 );  
 }
 if (String(topic) == "openHAB/esp_111/led5") {
  if (msgString == "ON") digitalWrite( 14, 0 );  //red leds are inverted output
  else                   digitalWrite( 14, 1 );  
 }
 if (String(topic) == "openHAB/esp_111/led6") {
  if (msgString == "ON") digitalWrite( 16, 0 );  //red leds are inverted output
  else                   digitalWrite( 16, 1 );  
 }
}

//----------------------------------------------------------------------------

void setup()
{
    // ESP-12 yellow carrier board GPIO assignments
  pinMode(A0, INPUT);  // analog light sensor input
  pinMode(2, OUTPUT);  // 00000X  discrete red led position
  pinMode(0, OUTPUT);  // 0000X0  discrete red led position
  pinMode(4, OUTPUT);  // 000X00  discrete red led position
  pinMode(5, OUTPUT);  // 00X000  discrete red led position
  pinMode(14,OUTPUT);  // 0X0000  discrete red led position
  pinMode(16,OUTPUT);  // X00000  discrete red led position
  
  pinMode(15,OUTPUT);  // RGB led red
  pinMode(12,OUTPUT);  // RGB led green
  pinMode(13,OUTPUT);  // RGB led blue
  
  // initialize GPIO 'inverted' led outputs to off
  digitalWrite(2, 1);  // red leds are inverted output 
  digitalWrite(0, 1);  // red leds are inverted output
  digitalWrite(4, 1);  // red leds are inverted output
  digitalWrite(5, 1);  // red leds are inverted output
  digitalWrite(14,1);  // red leds are inverted output
  digitalWrite(16,1);  // red leds are inverted output
  
  digitalWrite(15,0);  // RGB led red not inverted
  digitalWrite(12,0);  // RGB led green not inverted
  digitalWrite(13,0);  // RGB led blue not inverted

  Serial.begin(115200);
  delay(10);
  
  Serial.println();
  Serial.println();
  Serial.print("WiFi connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  WiFi.config(IPAddress(IP[0],IP[1],IP[2],IP[3] ),  IPAddress(Gateway[0],Gateway[1],Gateway[2],Gateway[3] ) , IPAddress(Netmask[0],Netmask[1],Netmask[2],Netmask[3] ));
  WiFi.mode(WIFI_STA);   
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("ESP8266 IP address: ");
  Serial.println(WiFi.localIP());

  yield();  // Will this help get a mqtt connection the first time? It won't hurt. It ...might... help.
  
  mqttClient.connect("esp8266Client_111");
 
  // Will implement retry in loop() if not connected here
  if (mqttClient.connect("esp8266Client_111")) {
       mqttClient.publish("esp_111","Hello from esp8266 mosquitto client");
       mqttClient.subscribe("openHAB/esp_111/#");
      Serial.println("Connected to mosquitto mqtt server.");
  }
  else {
      Serial.println("Failed to connected with mosquitto mqtt server.");
  }
  
}

//----------------------------------------------------------------------------

void loop()
{
  mqttClient.loop(); //Must call this function to let mqtt subscribe callbacks work
   
  millis_now = millis();
  
  if (millis_now > (millis_prev + 10000)){  //1000 = 1sec Non-blocking delay
    static int counter = 0;
    String payloadxmit = "{\"micros\":";
    payloadxmit += micros();
    payloadxmit += ",\"counter\":";
    //payloadxmit += (String) counter;
    payloadxmit += counter;
    payloadxmit += "}";
  
    if (mqttClient.connected()){
      Serial.print("Sending payload: ");
      Serial.println(payloadxmit);
    
      if (mqttClient.publish("esp_111", (char*) payloadxmit.c_str())) {
        Serial.println("mqtt publish ok");
      }
      else {
        Serial.println("mqtt publish failed");
      }

      lightsensor = analogRead(A0);

      char sensor[8];
      itoa(lightsensor,sensor,10);  // integer to string, base 10 (decimal)
    
      if (mqttClient.publish("esp_111/sensor", sensor)) {
        Serial.println("mqtt publish ok");
      }
      else {
        Serial.println("mqtt publish failed");
      }
      
    }
    else{                           
        Serial.println("Attempting to reestablish mosquitto mqtt connection");
        mqttClient.connect("esp8266Client_111");
        mqttClient.publish("esp_111","Hello again from esp8266 mosquitto client");
        mqttClient.subscribe("openHAB/esp_111/#");

    }
    ++counter;
    millis_prev = millis_now;
   }

}
