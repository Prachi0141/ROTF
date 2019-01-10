#include <SoftwareSerial.h>

SoftwareSerial Serial3(D1,D2);//rx,tx
String mystr = "r2l2f2b2e0"; //String data
void setup() {
  // Begin the Serial at 9600 Baud
  Serial3.begin(9600);
}

void loop() {
    Serial3.println(mystr); //Write the serial data
    delay(1000);
    Serial3.end();
}
