//-------------------------------------------------------
int StaConnect(){
  byte tryTime = 20; //time s to wait for connect
    
  // Wait for connection 1
  while ((WiFi.status() != WL_CONNECTED) && tryTime > 0 ){
    delay(1000);
    tryTime--;
    if (tryTime%10 == 0) {
      Serial.println(".");
     } else {
      Serial.print(".");
     }  
  }
  Serial.println();
  int stat = WiFi.status();
  Serial.print("Connection result: ");
  shoWiFi(stat);
  return (stat);
  
}
//-------------------------------------------------------
void connectWifi() {

 Blink(1);
 if (WiFi.getMode() != WIFI_STA) {
    WiFi.mode(WIFI_STA);
    delay(100);
  }
  Serial.println("Connecting Ap1 as wifi client");
  WiFi.disconnect();
  WiFi.hostname(HOSTNAME);
  WiFi.begin ( ssid, pass );
  //if you need manually given IP address it should be done here

  int constat=StaConnect();
  
  if (constat != 3) {
    WiFi.disconnect(); //not connected, try spare network
    delay(100);
    Blink(2);
    
    Serial.println("Connecting Ap2 as wifi client");
    WiFi.hostname(HOSTNAME);
    WiFi.begin ( vssid, vpass );
    //if you need manually given IP address it should be done here
    
    constat=StaConnect();
    } 

  if (constat != 3) {
    WiFi.disconnect(); //not connected, selt local AP
    delay(100);
    Blink(3);
    if (WiFi.getMode() != WIFI_AP) {
      WiFi.mode(WIFI_AP);
    }
    delay(100);
    MylocalAp();
    
  } else {  
    Myinfo();
  }
  
 ConTimeWait = millis();   
}
//-------------------------------------------------------
void MylocalAp(){
IPAddress local_IP(192,168,4,2);
IPAddress gateway(192,168,4,2);
IPAddress subnet(255,255,255,0);

Serial.print("Setting soft-AP configuration ... ");
Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
Serial.print("Setting soft-AP ... ");
Serial.println(WiFi.softAP(assid,apass) ? "Ready" : "Failed!");
Serial.print("Soft-AP IP address = ");
Serial.println(WiFi.softAPIP());
printMac();
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

//-------------------------------------------------------
