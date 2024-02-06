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
  pinMode(OnBoardLed, OUTPUT);
  digitalWrite(ResEsp,HIGH);
  digitalWrite(CW,LOW);
  digitalWrite(CCW,LOW);
  digitalWrite(OnBoardLed,LedOff);
  
      config.MCW =919; //clockwise reading max
      config.MCCW = 282; //counterclockwise reading max
      config.LCW = 835;
      config.LCCW = 350;
      config.truefix = -40;
      config.negdeg = 0;
      ShowIfDebug("CW & CCW max are now set as defaults");

  Serial.println(F("mounting FS...")); 
  
  SPIFFSConfig cfg;
  cfg.setAutoFormat(false);
  SPIFFS.setConfig(cfg);
  
  if (SPIFFS.begin()) {
    Serial.println(F("mounted file system"));
   } else {
    Serial.println(F("failed to mount FS"));
    Serial.println(F("Trying with format FS"));
    SPIFFSConfig cfg;
    cfg.setAutoFormat(true);
    SPIFFS.setConfig(cfg);
    if (SPIFFS.begin()) {
       Serial.println(F("mounted formatted file system"));
       } else {
        Serial.println(F("failed to format/mount FS"));
       }
   }  
 
 ReadSettings();
 if (debug) { 
   PrintConfig();
 }
 chklimits();

  connectWifi(); 
  setupOta();
  server.begin();
  server.setNoDelay(true);
  delay(5);

 ShowIfDebug("Starting rot version:"+vers+" as "+HOSTNAME);
 debug = false;
 Blink(4);
}
