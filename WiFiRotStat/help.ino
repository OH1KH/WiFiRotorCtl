void helpCee(){
 writeCli("\n");
 writeCli("Parameters for command C\n");
 writeCli("MCW  [val] set parameter to be as current rotator position or give value \n");
 writeCli("MCCW [val] set parameter to be as current rotator position or give value \n"); 
 writeCli("LCW    M   parameter sets same as max , else current position \n");
 writeCli("LCCW   M   parameter sets same as max , else current position \n");
 writeCli("SOUTH [val] set south to be as current rotator position or give value {saved}\n");
 writeCli("TRACE  0|1 to trace poti reading in serial port. Needs also DEBUG\n"); 
 writeCli("DEBUG  0|1 to print debug messages to serial port \n");
 writeCli("NEGDEG 0|1 values either 0..360 or -180..180  \n");
 writeCli("\n");
 writeCli("To set all current values send following commands\n\n");

 writeCli("C MCW "+String(MCW)+"\n");
 writeCli("C LCW "+String(LCW)+"\n");
 writeCli("C MCCW "+String(MCCW)+"\n");
 writeCli("C LCCW "+String(LCCW)+"\n");
 writeCli("C SOUTH "+String(truefix)+"\n");
 writeCli("C NEGDEG "+String(negdeg)+"\n\n");

 ok = true;
}
//-------------------------------------------------------
void ShowIfDebug(String show) {
 if (debug) {
  Serial.println(show);
  if (DbgCliNr!= 99){
    show=show+"\n";
    size_t len = show.length()+1;
    char sbuf[len];
    show.toCharArray(sbuf,len);
    if (serverClients[DbgCliNr] && serverClients[DbgCliNr].connected()){
      serverClients[DbgCliNr].write(sbuf, len);
      delay(5);
     }
  }
    
 }
}
