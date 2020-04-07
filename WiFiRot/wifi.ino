//-------------------------------------------------------
void wifisetup(){
   Serial.println("Entering WiFi setup");  
   digitalWrite(OnBoardLed, LedOn);
   delay(1);
   WiFiManager wifiManager;
      
   wifiManager.resetSettings();
   Serial.println("Settings reset done");
   wifiManager.setTimeout(120);  
   Serial.println("timeout set 120s");  

    if (!wifiManager.startConfigPortal(HOSTNAME)) {
    Serial.println("Not connected, restarting");  
       digitalWrite(OnBoardLed, LedOff);
       delay(2000);
      digitalWrite(OnBoardLed, LedOn);  //CW N
      delay(600);
      digitalWrite(OnBoardLed, LedOff);
      delay(200);
      digitalWrite(OnBoardLed, LedOn);
      delay(200);
      digitalWrite(OnBoardLed, LedOff);
      pinMode(TRIGGER_PIN, OUTPUT);
      digitalWrite(TRIGGER_PIN,HIGH);
      delay(2000);
      ResetEsp(ResEsp);
      }  else {
      Serial.println("Connected WiFi");
      digitalWrite(OnBoardLed, LedOff);
      pinMode(TRIGGER_PIN, OUTPUT);
      digitalWrite(TRIGGER_PIN,HIGH); 
      delay(2000);
      digitalWrite(OnBoardLed, LedOn);
      delay(200);
      digitalWrite(OnBoardLed, LedOff);
      delay(200);
      digitalWrite(OnBoardLed, LedOn);  //CW R
      delay(600);
      digitalWrite(OnBoardLed, LedOff);
      delay(200);
      digitalWrite(OnBoardLed, LedOn);
      delay(200);
      digitalWrite(OnBoardLed, LedOff);
     }
  digitalWrite(OnBoardLed, LedOff);

}
//-------------------------------------------------------
void printMac() {
  
  Serial.print(mac[0],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.println(mac[5],HEX);
} 
//-------------------------------------------------------
void shoWiFi(int stat) {
 
  switch (stat) {
    case   0: Serial.println ( "WL_IDLE_STATUS" ); break;
    case   1: Serial.println ( "WL_NO_SSID_AVAIL" ); break;
    case   2: Serial.println ( "WL_SCAN_COMPLETED" ); break;
    case   3: Serial.println ( "WL_CONNECTED" ); break;
    case   4: Serial.println ( "WL_CONNECT_FAILED" ); break;
    case   5: Serial.println ( "WL_CONNECTION_LOST" ); break;
    case   6: Serial.println ( "WL_DISCONNECTED" ); break;
    case 255: Serial.println ( "WL_NO_SHIELD" ); break;
  }
}
//-------------------------------------------------------
void Myinfo() {
if ( debug ) {
  int s = WiFi.status();
  Serial.print ( "WiFi status: " );
  shoWiFi( s );
      if (s == WL_CONNECTED) {
        Serial.print("Myname is: ");
        Serial.println(HOSTNAME);
        Serial.print("My IP address: ");
        Serial.println(WiFi.localIP());
        WiFi.macAddress(mac);
         printMac(); 
       } 

}
} 
//-------------------------------------------------------
