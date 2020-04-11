

/*
v1.0  The start
v1.1  added many conf commands, first release, tests start
v1.2  modularizing source
*/
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <ESP8266mDNS.h>        // Include the mDNS library
#include <ArduinoOTA.h>
#include <WiFiUdp.h>
#define TRIGGER_PIN 0              // access to WiFimanager when onboard led is blinking 
#define OnBoardLed 2               //ESP's onboard led pin (Check! May vary)
#define LedOn LOW                 //ESP's onboard led illuminated (Check! May vary, too)
#define LedOff HIGH               //ESP's onboard led NOT illuminated 

//how many clients should be able to telnet to this 
#define MAX_SRV_CLIENTS 3
WiFiServer server(4533);
WiFiClient serverClients[MAX_SRV_CLIENTS];
uint8_t CliNr; //client #nr
uint8_t DbgCliNr = 99; //client #nr that gets dbg 99=none

#define ResEsp 13 // pin connected to reset via 1k resistor
#define CW 4  //clockwise relay
#define CCW 5 //counterclockwise relay
#define POT A0 //potentiometer ADC (esp always 0)

#define turnwatch 3000 // timeout to check that some movement has happened (ms)
#define IFtimeout 800 //timeout for TCP response (ms)

#define ResetEsp(pin) {digitalWrite(pin, LOW);delay(5); digitalWrite(pin, HIGH); }

#ifndef APSSID
#define APSSID "rotctl-m";
#define APPSK  "myrotpass" //at least 8chrs
#endif
/* Set these to your desired credentials. */
const char *HOSTNAME = APSSID;
const char *assid = APSSID;
const char *apass = APPSK;

#ifndef WSSID
#define WSSID "oh1kh";
#define WPSK  "----" 
#endif
/* Set these to your WiFi credentials. */
const char *ssid = WSSID;
const char *pass = WPSK;
 
unsigned long ConTimeWait = 0;
unsigned long CTO = 120000;  // time to wait before new attempt to log in WiFI network

String vers = "v1.2";  //Version    OH1KH-2017
int MCW,LCW =1023; //clockwise reading,limit max
int MCCW,LCCW =0; //counterclockwise reading,limit max
float heading,fixheading = 0; //where rotator,antenna points
float digideg = 0; // factor to convert digital reading to degrees of turn
int truefix = 0; // degrees of turn between true south and current heading
int turnTo = -1; //where to turn next neg = nowere
boolean debug,trace,needCW, needCCW,tccw,tcw = false; //debug, order to turn+dir, turn+dir,
byte  negdeg = 0; // neg Az ok
char ReadByte = 0; // from serial line
int  dir,oldir = 0; //poti readiong raw
char cmd = 0; //header part of command 
String Az,El ="";
int i = 0; //common loop 
byte mac[6];                      //mac of WiFi

String RigCmd = "";          //received command from telnet
char prech = '\n';
String s = "";              // general string;

unsigned long turntime = 0;  //timer for motor running
boolean ok = false; //general boolean


//------------------------------------------------
//intro of subroutines
//---wifi----------------------------------------------------
void connectWifi(); 
void MylocalAp();
void printMac();
void shoWiFi(int stat);
void Myinfo();
void setupOta();
//---help---------------------------------------------------
void helpCee();
void ShowIfDebug(String show);
//---tcp----------------------------------------------------
void disClient();
void serveTCP();
void readCli();
void writeCli(String answer);
bool HasCli();
//---cmds----------------------------------------------------
void chklimits();
void turns();
void longTOshort(int count);
void dump_state();
  void get_pos();
void set_pos(int away);
void set_move(int away);
void set_conf(int away);
void set_park(int away);
void set_stop(int away);
void set_reset(int away);
void get_info();
void prefix();
void ParseCommand(); 
//-------------------------------------------------------
//-------------------------------------------------------
void setup() {
  delay(500);
  Serial.begin(115200);
  delay(500);
 
  debug = true;
  ShowIfDebug("\n");
  
  ShowIfDebug("\n\r \n\rStarting setup");
  pinMode(CW, OUTPUT);
  pinMode(CCW, OUTPUT);
  pinMode(ResEsp, OUTPUT);
  digitalWrite(ResEsp,HIGH);
  digitalWrite(CW,LOW);
  digitalWrite(CCW,LOW);
  
      MCW =1023; //clockwise reading max
      MCCW =0; //counterclockwise reading max
      LCW = MCW;
      LCCW = MCCW;
      ShowIfDebug("CW & CCW max are now set as defaults");
 
 chklimits();

  connectWifi(); 
  setupOta();
  server.begin();
  server.setNoDelay(true);
  delay(5);

 ShowIfDebug("Starting rot version:"+vers+" as "+HOSTNAME);
 debug = true;
}


void loop() { 
  ArduinoOTA.handle();
  //time check that we are still connected
  if ((millis()- ConTimeWait) > CTO) {
    ShowIfDebug("Check WiFi Connection");
    if (WiFi.getMode() != WIFI_STA) {
     ShowIfDebug("We are AP"); 
     if (!HasCli()) {
          ShowIfDebug("No AP clients, try WiFi");
          connectWifi();
          ConTimeWait = millis();
     } else {
      ShowIfDebug("We have client connected");
      ConTimeWait = millis();
     }
    } 
    else { // if WIFI_AP then if no clients try to connect WiFi network
      ShowIfDebug("We are STA");
      if (WiFi.status() != WL_CONNECTED){
         ShowIfDebug("We are not connected!"); 
         connectWifi(); 
         ConTimeWait = millis(); 
      } else {
        ShowIfDebug("We are connected!"); 
        ConTimeWait = millis();  
      }
    }
   }
 
  serveTCP();//check connects and disconnects
  readCli(); //read client command and execute it
  turns(); //produce turning and limit checks
  delay(200); //loop delay
}


/* 
  ROT interface. (c) OH1KH -2017
  This work is based on several examples collected together
  
   -WiFiTelnetToSerial - Copyright (c) 2015 Hristo Gochkov. 
    This file is part of the ESP8266WiFi library for Arduino environment.
  
   -WiFiManager          //https://github.com/tzapu/WiFiManager
    
  - command syntax from HamLib rotctld https://sourceforge.net/projects/hamlib/

  This program and it's parts are free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  A  copy of the GNU Lesser General Public License can be found from
  the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
