#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>

const char *ssid = "dhruv"; /////////////////////////// NAME OF WIFI
const char *password = "12345678"; /////////////////// WIFI PASSWORD

String message;
String dir;
int i = 0;
ESP8266WebServer server(80);
SoftwareSerial Serial2(D1,D2);//rx,tx

void handleSpecificArg() 
{ /////////////////////// url form example : http://IpAddressOfEsp/specificArgs?direction=5SL4SR7ST

message = "";

if (server.arg("direction")== "")
{     //Parameter not found

message = "direction not found";

}

else
{     //Parameter found

message = "direction = ";
dir = server.arg("direction");
message += dir;     //Gets the value of the query parameter
 Serial2.print(dir); //Write the serial data
    delay(1000);

}
Serial.println(message);


server.send(200, "text/plain", message);          //Returns the HTTP response

}


void setup() 
{

delay(1000);

Serial.begin(9600);

Serial.println();

/////////////////////  Connecting to already made wifi hotspot or router
///////////////////////////////////////////////////////////////////
WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) 
  {
 
    delay(1000);
    Serial.print("Connecting..");
 
  }
  Serial.print("Connected");
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

///////////////////////////////////////////////////////////

server.on("/specificArgs", handleSpecificArg);

server.begin();

Serial.println("HTTP server started");
Serial2.begin(9600);

}

void loop() 
{
  server.handleClient();
}
