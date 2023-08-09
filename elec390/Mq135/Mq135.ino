


#include <SoftwareSerial.h>
#define rxPin 2
#define txPin 3
char input;
const int sensor_id = 12;
const int bt_state = 4;
const int bt_state_out=11;
const int analogSensorPin = A0; 
const int analogBatPin = A1;
const int charge_pin = 5;
const int warming_time = 20000;
int charge_percentage; 
bool warming=true;
unsigned long warming_start;
String message;

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
      mySerial.read();
     
       if(millis()-warming_start>warming_time){message=analogRead(analogSensorPin);}
       else{message="w";}
      
       charge_percentage = constrain((107.526*(5.0*float(analogRead(analogBatPin))/1024.0)-351),0,100);
       message=message+"/"+String(charge_percentage)+"/"+String(digitalRead(charge_pin))+"E";
       mySerial.println(message);  
        
       }
    }

  else{digitalWrite(bt_state_out,LOW);  warming=true;}


}
