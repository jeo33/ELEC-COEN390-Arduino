//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// FirebaseDemo_ESP8266 is a sample that demo the different functions
// of the FirebaseArduino API.

#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <SoftwareSerial.h>
String sensorValue;
#define FIREBASE_HOST "test-f13b1-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "yF8p3LnqpyHxBxQPS67UEV71NfYplrjcPBEDvdSp"
#define WIFI_SSID "CellNew"
#define WIFI_PASSWORD "wubinator"
#define BAUD_RATE_SERIAL1 9600
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
String line;
String templine="";

//SoftwareSerial serial1(D1, D2);  // RX: D1, TX: D2
SoftwareSerial serial1(D5, D6);  // RX: D5, TX: D6

void setup() {
  Wire.begin();
   RTC.begin();

  pinMode(D3,OUTPUT);
  pinMode(D4,OUTPUT);
  Serial.begin(9600);
  serial1.begin(BAUD_RATE_SERIAL1);
  serial1.setTimeout(300);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
String message_name ="message/";
int n = 0;
int num ;
bool skip=false;


void loop() {
 DateTime now = RTC.now(); 
 templine = now.year()+String("/")+now.month()+String("/")+now.day()+String("/")+now.hour()+String("/")+now.minute()+String("/")+now.second(); 
 line = templine;

digitalWrite(D3,LOW);
digitalWrite(D4,LOW);
delay(100);
while(serial1.available()){serial1.read();}
serial1.print('s');
    templine=serial1.readStringUntil('\n');
    if(templine[templine.length() - 2]=='E'){
      templine.remove(templine.length() - 1);templine.remove(templine.length() - 1);
      line =  line +"/"+templine;;
    }else{line =line +"/CL/CL/CL";}

digitalWrite(D3,LOW);
digitalWrite(D4,HIGH);
delay(100);
while(serial1.available()){serial1.read();}
serial1.print('s');
    templine=serial1.readStringUntil('\n');
    if(templine[templine.length() - 2]=='E'){
      templine.remove(templine.length() - 1);templine.remove(templine.length() - 1);
      line = line +"/"+templine;
    }else{line =line +"/CL/CL/CL";}
 

digitalWrite(D3,HIGH);
digitalWrite(D4,LOW);
delay(100);
delay(100);
while(serial1.available()){serial1.read();}
serial1.print('s');
    templine=serial1.readStringUntil('\n');
    if(templine[templine.length() - 2]=='E'){
      templine.remove(templine.length() - 1);templine.remove(templine.length() - 1);
      line = line +"/"+templine;
    }else{line =line +"/CL/CL/CL";}
   Serial.print("Received from Serial1: ");
   Serial.println(line);


  

  
delay(100);
if(skip){  
   Firebase.setString(message_name+"current_reading", line+"/"+String(n));
   Firebase.setString(message_name+String(n), line);
  n++;  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());  
      return;
  }
}
 skip=true;
}
