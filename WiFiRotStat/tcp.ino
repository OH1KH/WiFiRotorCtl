void disClient(){
  if (serverClients[CliNr] && serverClients[CliNr].connected()){
    serverClients[CliNr].stop();
    Serial.println("Client "+String(CliNr)+" forced disconnected");
  }
}
//-------------------------------------------------------
void serveTCP() {
  
  //check if there are any new clients connecting
  if (server.hasClient()){
    for(CliNr = 0; CliNr < MAX_SRV_CLIENTS; CliNr++){
      //find free/disconnected spot
      if (!serverClients[CliNr] || !serverClients[CliNr].connected()){
        if(serverClients[CliNr]) serverClients[CliNr].stop();
        serverClients[CliNr] = server.available();
        if (debug) { Serial.print("New client: #"); Serial.println(CliNr); }
        //reset dataRX timer
        CliTO[CliNr] = millis();
        continue;
      }
    }
    //no free/disconnected spot so reject
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }
 }  
//-------------------------------------------------------
void readCli() {
    
  //check TCP clients for commands
  for(CliNr = 0; CliNr < MAX_SRV_CLIENTS; CliNr++){
    if (serverClients[CliNr] && serverClients[CliNr].connected()){
      if(serverClients[CliNr].available()){
        //reset dataRX timer
        CliTO[CliNr] = millis();
        //get data from the telnet client 
        while(serverClients[CliNr].available()) { // serve clients in order
          unsigned long LastCh = millis();
          while( (serverClients[CliNr].available() > 0)  && ((millis()-LastCh) < IFtimeout)) { //wait for end of string or timeout
             char OneRead=serverClients[CliNr].read();
             if ((OneRead == '\n')) {               //end of string
               ShowIfDebug("Received from TCP client "+String(CliNr)+" : "+RigCmd);
               ParseCommand();
               RigCmd = ""; 
              } 
            if ((OneRead > 31) && (OneRead < 127))  RigCmd = RigCmd+OneRead;   
            yield();
          } // while char     
        } // while curr client
      } else {  // if client nr has data else is dataRX timeout
        if ((millis()-CliTO[CliNr])> DTO ) {
          disClient(); 
          }
      }
    } // if client nr is connected
  } // for all clients

if (RigCmd == ""); {
 do { 
  while (Serial.available() ) {
     ReadByte = Serial.read();
      if (ReadByte == '\n') {
        ShowIfDebug("Received from serial client: "+RigCmd);
        ParseCommand();
        RigCmd = ""; 
       } else  if ((ReadByte > 31) && (ReadByte < 127)) RigCmd = RigCmd+ReadByte;
      }
   } while ((ReadByte != '\n' )&& Serial.available());
 }
}
//-------------------------------------------------------
void writeCli(String answer) {
  // client is TCP
  size_t len = answer.length()+1;
  char sbuf[len];
  answer.toCharArray(sbuf,len);
  if (serverClients[CliNr] && serverClients[CliNr].connected()){
    serverClients[CliNr].write(sbuf, len);
    delay(5);
    Serial.println(answer);
   }
}
//-------------------------------------------------------
bool HasCli() {
  bool has=false;
  for(CliNr = 0; CliNr < MAX_SRV_CLIENTS; CliNr++){
    if ((!has) && (serverClients[CliNr].connected())){
      has=true;
    }
  }
 return has;
}
