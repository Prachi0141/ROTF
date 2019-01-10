#include <SoftwareSerial.h>
char mystr[10]; //Initialized variable to store recieved data

SoftwareSerial Serial1(9,10);//rx,tx
void setup() {
  // Begin the Serial at 9600 Baud
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop() {
  Serial1.write("get String");
  while(!Serial1.available()){
    Serial.println("reading.....");
  }
  Serial1.readBytes(mystr,5); //Read the serial data and store in var
  Serial.println(mystr); //Print data on Serial Monitor
  delay(1000);
}
