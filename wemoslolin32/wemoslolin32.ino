#include <WiFi.h>
 
// Replace with your network credentials
const char* ssid     = "IVORY_Hotel";
const char* password = "";
const int analogIn = A0;
 
int RawValue= 0;
double Voltage = 0;
double tempC = 0;

 
WiFiServer server(80);
 
const int led =  22;      // the number of the LED pin
 
// Client variables 
char linebuf[80];
int charcount=0;
 
void setup() 
{
  // initialize the LED as an output:
  pinMode(led, OUTPUT);
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while(!Serial) {
  }
   // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
   
  WiFi.begin(ssid, password);
   
  // attempt to connect to Wifi network:
  while(WiFi.status() != WL_CONNECTED) 
  {
    // Connect to WPA/WPA2 network.
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); 
  server.begin();
}
 
void loop() 
{
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) 
  {
    Serial.println("New client");
    memset(linebuf,0,sizeof(linebuf));
    charcount=0;
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) 
    {
      if (client.available()) 
      {
        char c = client.read();
        Serial.write(c);
        //read char by char HTTP request
        linebuf[charcount]=c;
        if (charcount<sizeof(linebuf)-1) charcount++;
 
        if (c == '\n' && currentLineIsBlank) 
        {
          RawValue = analogRead(analogIn);
          Voltage = (RawValue / 2048.0) * 3300; // 5000 to get millivots.
          tempC = Voltage * 0.1;
          Serial.print("Temp : ");
          Serial.println(tempC); 
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println();
          client.println("<!DOCTYPE HTML><html><head>");
          client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head>");
          client.println("<h1>ESP32 - Web Server</h1>");
          client.println(tempC);
          client.println("*C</p><p>");
          client.println("<p>LED <a href=\"on\"><button>ON</button></a>&nbsp;<a href=\"off\"><button>OFF</button></a></p>");
          client.println("</html>");
          break;
        }
        if (c == '\n') 
        {
          // you're starting a new line
          currentLineIsBlank = true;
          if (strstr(linebuf,"GET /on") > 0)
          {
            Serial.println("LED ON");
            digitalWrite(led, LOW);
          }
          else if (strstr(linebuf,"GET /off") > 0)
          {
            Serial.println("LED OFF");
            digitalWrite(led, HIGH);
          }
 
          // you're starting a new line
          currentLineIsBlank = true;
          memset(linebuf,0,sizeof(linebuf));
          charcount=0;
        } 
        else if (c != '\r') 
        {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
 
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}
