

#include <SoftwareSerial.h>
#define rxPin 2
#define txPin 3
char input;
const int sensor_id = 10;
const int bt_state = 4;
const int bt_state_out=11;
const int analogSensorPin = A0; 
const int analogBatPin = A1;
const int charge_pin = 5;
const int warming_time = 20000;
int charge_percentage; 
bool warming=true;
unsigned long warming_start;

SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);

void setup() {
  pinMode(bt_state,INPUT);
  pinMode(bt_state_out,OUTPUT);

  Serial.begin(9600);
  pinMode(rxPin, INPUT);
  pinMode(charge_pin, INPUT);
  pinMode(txPin, OUTPUT);
  mySerial.begin(9600);

}

void loop() {
  if(digitalRead(bt_state)){
    digitalWrite(bt_state_out,HIGH);
    if(warming==true){warming_start=millis();warming=false;}
    if (mySerial.available() > 0) {
      input = mySerial.read();
      switch(input){
      
        case 's':
        if(millis()-warming_start>warming_time){mySerial.println(analogRead(analogSensorPin));}
        else{mySerial.println('w');}
        break;

        case 'b':
        charge_percentage = constrain((107.526*(5.0*float(analogRead(analogBatPin))/1024.0)-351),0,100);
        mySerial.println(charge_percentage);
        break;

        case 'c':
        if(digitalRead(charge_pin)){mySerial.println(1);}else{mySerial.println(0);}
        break;

        case 'i':
        mySerial.println(sensor_id);
        break;
        }
       }
    }

  else{digitalWrite(bt_state_out,LOW);  warming=true;}


}
