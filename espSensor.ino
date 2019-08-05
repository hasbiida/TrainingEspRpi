#include <WiFi.h>
#include <HTTPClient.h> 
//#include <stdlib.h>
const char* ssid = "Bukan_Wifi";
const char* password = "1sampai9";
int id = 1;
void setup() {

  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
}
void loop() {

  if ((WiFi.status() == WL_CONNECTED)) { 

    HTTPClient http;
    int data=rand()%255;
    char bufferstr[80];
    sprintf(bufferstr, "http://192.168.0.100:8090/getdata/%i/%i", id, data);
    http.begin(bufferstr);
    int httpCode = http.GET();                                        

    if (httpCode > 0) {

        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
      }

    else {
      Serial.println("Error on HTTP request");
    }

    http.end();
  }

  delay(1000);

}


