

#include <SoftwareSerial.h>
#define rxPin 2
#define txPin 3
#define firePin1 4
#define firePin2 5
#define firePin3 6
#define firePin4 7
char input;
const int sensor_id = 13;
const int analogBatPin = A1;
const int charge_pin = 11;
int charge_percentage; 
String message;


SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);

void setup() {
  pinMode(firePin1,INPUT);
  pinMode(firePin2,INPUT);
  pinMode(firePin3,INPUT);
  pinMode(firePin4,INPUT);
  Serial.begin(9600);
  pinMode(rxPin, INPUT);
  pinMode(charge_pin, INPUT);
  pinMode(txPin, OUTPUT);
  mySerial.begin(9600);

}

void loop() {
    if (mySerial.available() > 0) {
     mySerial.read();   
     charge_percentage = constrain((107.526*(5.0*float(analogRead(analogBatPin))/1024.0)-351),0,100);
     message=String(digitalRead(firePin1))+String(digitalRead(firePin2))+String(digitalRead(firePin3))+String(digitalRead(firePin4))+"/"+String(charge_percentage)+"/"+String(digitalRead(charge_pin))+"E";
    mySerial.println(message);  
      
       }
    



}
