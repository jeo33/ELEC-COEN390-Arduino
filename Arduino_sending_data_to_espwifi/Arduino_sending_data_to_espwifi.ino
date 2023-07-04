#include <SoftwareSerial.h>

// SoftwareSerial object to communicate with ESP8266
SoftwareSerial mySerial(2, 3); // Arduino RX pin 2 (ESP8266 TX), Arduino TX pin 3 (ESP8266 RX)

void setup() {
  // Start the serial communication
  Serial.begin(9600);
  mySerial.begin(9600);
  
  // Wait for the ESP8266 to initialize
  delay(1000);
}
int n=0;
void loop() {
 boolean ready=false;
  n=analogRead(0);
 mySerial.println(n);
 
  delay(1000);
}
https://www.hackster.io/pulasthi-Narada/connecting-esp8266-to-firebase-to-send-receive-data-4adf66
