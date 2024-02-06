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
