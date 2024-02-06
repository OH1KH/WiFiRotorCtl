void help() {
writeCli("\n");
writeCli("command prefixes   +,;|  defines output format\n");
writeCli("\n");
writeCli("short commands\n");
writeCli("\n");
writeCli("? : this help\n");
writeCli("p : get_pos\n");
writeCli("P : set_pos [Az El]\n");
writeCli("Q : disconnect Client\n");
writeCli("q : disconnect Client\n");
writeCli("M : set_move  [8 0] left  [16 0] right\n");
writeCli("C : set_conf [param value]\n");
writeCli("c : Help for C command parameters\n");
writeCli("K : Park same as Stop rotator\n");
writeCli("S : Stop rotator\n");
writeCli("R : reset controller\n");
writeCli("_ : get_info\n");
writeCli("\n");
writeCli("long mode commands start with \\\n");
writeCli("\n");
writeCli("dump_state\n");
writeCli("get_pos\n");
writeCli("get_info\n");
writeCli("set_pos\n");
writeCli("set_move\n");
writeCli("set_conf\n");
writeCli("stop\n");
writeCli("park\n");
writeCli("reset\n");
writeCli("\n");
 
ok = true;
}

void helpCee(){
 writeCli("\n");
 writeCli("Parameters for command C\n");
 writeCli("MCW  [val] set parameter to be as current rotator position or give value \n");
 writeCli("MCCW [val] set parameter to be as current rotator position or give value \n"); 
 writeCli("LCW  [M] [val] M parameter sets same as MCW , val sets value, else sets current position \n");
 writeCli("LCCW [M] [val] M parameter sets same as MCCW , val sets value, else sets current position \n");
 writeCli("SOUTH [val] set south to be as current rotator position or give value {saved}\n");
 writeCli("TRACE  0|1 to trace poti reading in serial port. Needs also DEBUG\n"); 
 writeCli("DEBUG  0|1 to print debug messages to serial port \n");
 writeCli("NEGDEG 0|1 values either 0..360 or -180..180  \n");
 writeCli("\n");
 writeCli("To set all current values send following commands\n\n");

 writeCli("C MCW "+String(config.MCW)+"\n");
 writeCli("C LCW "+String(config.LCW)+"\n");
 writeCli("C MCCW "+String(config.MCCW)+"\n");
 writeCli("C LCCW "+String(config.LCCW)+"\n");
 writeCli("C SOUTH "+String(config.truefix)+"\n");
 writeCli("C NEGDEG "+String(config.negdeg)+"\n\n");

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
