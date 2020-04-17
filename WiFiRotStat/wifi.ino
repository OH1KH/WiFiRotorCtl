//-------------------------------------------------------
//-------------------------------------------------------
void connectWifi() {

 if (WiFi.getMode() != WIFI_STA) {
    WiFi.mode(WIFI_STA);
    delay(10);
  }
  byte tryTime = 30; //time s to wait for connect
  Serial.println("Connecting as wifi client");
  WiFi.disconnect();
  WiFi.hostname(HOSTNAME);
  WiFi.begin ( ssid, pass );
  //if you need manually given IP address it should be done here
  
  // Wait for connection
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
  if (stat != 3) {
    WiFi.disconnect(); //not connected, leads to retry at main loop after retry timeout
    WiFi.mode(WIFI_AP);
    delay(10);
    MylocalAp();
  } else { 
    // OTA handles this Serial.println(MDNS.begin(HOSTNAME) ? "mDNS responder started" : "Error setting up MDNS responder!");  
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
void setupOta(){
  Serial.println("Setup OTA");
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
  ArduinoOTA.setHostname(HOSTNAME);
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
//-------------------------------------------------------
