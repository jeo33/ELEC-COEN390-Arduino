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
#define D8 15
String line;
String templine="";

//GPS 
String position, longi, lati, lat_min;
char c,v;
int index_c, long_start, long_end, lat_start, lat_end;
float longi_f=0;
float lati_f=0;
//GPS end



//SoftwareSerial serial1(D1, D2);  // RX: D1, TX: D2
SoftwareSerial serial1(D5, D6);  // RX: D5, TX: D6


void setup() {
  Wire.begin();
   RTC.begin();

  pinMode(D3,OUTPUT);
  pinMode(D4,OUTPUT);
  pinMode(D7,OUTPUT);
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
delay(150);
while(serial1.available()){serial1.read();}
serial1.print('s');
    templine=serial1.readStringUntil('\n');
    if(templine[templine.length() - 2]=='E'){
      templine.remove(templine.length() - 1);templine.remove(templine.length() - 1);
      line =  line +"/"+templine;
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

   //---------------------------
digitalWrite(D3,HIGH);
digitalWrite(D4,HIGH);
digitalWrite(D7,LOW);
delay(50);


top:
  while (serial1.available()==false){}
  if(serial1.read()=='$' ){
    while(serial1.available()==false){}
    if(serial1.read()=='G'){
      while(serial1.available()==false){}
      if(serial1.read()=='P'){
        while(serial1.available()==false){}
        if(serial1.read()=='R'){position=serial1.readStringUntil('$');
        }else{goto top;}
        }else{goto top;}
        }else{goto top;}
        }else{goto top;}
  
  index_c=0;long_start=0; long_end=0; lat_start=0; lat_end=0;
for(int i=0; i<2; i++){index_c=position.indexOf(',',index_c+1);}
for(int i=0; i<3; i++){lat_start=position.indexOf(',',lat_start+1);} lat_end=lat_start;
for(int i=0; i<1; i++){lat_end=position.indexOf(',',lat_end+1);}
for(int i=0; i<5; i++){long_start=position.indexOf(',',long_start+1);} long_end=long_start;
for(int i=0; i<1; i++){long_end=position.indexOf(',',long_end+1);}

v = position[index_c+1];
if(v=='A'){
lati=position.substring(lat_start+1,lat_end);
longi=position.substring(long_start+1,long_end);
lati_f=lati.substring(2).toFloat(); lati.remove(2);
longi_f=longi.substring(3).toFloat();longi.remove(3);
lati_f = lati_f/60.0; lati_f = lati_f + lati.toInt();
longi_f = longi_f/60.0; longi_f = longi_f + longi.toInt();
if(char(position[lat_end+1])=='S'){lati_f = -lati_f;}
if(char(position[long_end+1])=='W'){longi_f = -longi_f;}
templine="/A/"+String(lati_f,6)+"/"+String(longi_f,6);
line =  line +templine;
}else{
  templine="/V/"+String(lati_f,6)+"/"+String(longi_f,6);
  line =  line +templine;}

//-----------------------------------------------------

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
