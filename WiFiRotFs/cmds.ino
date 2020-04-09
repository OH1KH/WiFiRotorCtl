//-------------------------------------------------------
void chklimits(){
// max & min cant be same or MCW less than MCCW
   if (MCW <= MCCW) {
     MCW = 1023;
     MCCW = 0;
   }  
// limit can't be less/more than max limit
    if (LCW > MCW ) LCW = MCW;
    if (LCCW < MCCW ) LCCW = MCCW;
//LCW cant' be less than LCCW
    if (LCW < LCCW) LCW = LCCW + 20; // this +20 gives at least little space to move
    
    digideg =  360.0 / abs(MCW-MCCW) ; 
}
//-------------------------------------------------------
void turns() {
  //read position potentiometer and calculate relative and true degrees
  dir = analogRead(POT);
  heading = (dir-MCCW) * digideg;
  if (heading > 360) heading = heading - 360;
  if (heading < 0) heading = 360 + heading;
  fixheading = heading + truefix;
  if (fixheading > 360) fixheading = fixheading - 360;
  if (fixheading < 0) fixheading = 360 + fixheading;
  
  if (trace) {
    ShowIfDebug("Poti :"+String(dir)+" Reldir:"+String(heading)+" dir:"+String(fixheading));
   }
  
  if ((tcw || tccw) && (millis()-turntime >= turnwatch)){  // check that rotator is moving if relay on
    if (abs(dir-oldir) < 1) {
      digitalWrite(CW,LOW);
      digitalWrite(CCW,LOW);
      tcw= false;
      tccw= false;
      ShowIfDebug("Not moving "+String(dir));
    } else {
       ShowIfDebug("Moving "+String(dir));
    }
    turntime = millis();
    oldir = dir;
  }
  
  if (turnTo > -1) { //turning to position
    if (tcw || tccw) {  
     if (abs(turnTo - fixheading) < 2) {
      digitalWrite(CCW,LOW);
      tccw = false;
      digitalWrite(CW,LOW);
      tcw = false;
      turnTo = -1;
      ShowIfDebug("In position: "+String(fixheading));
     }
    }  
   }
   
  //start turn (from command) 
  if ((needCW) && (!tcw)) {
    needCW = false;
    digitalWrite(CCW,LOW);
    digitalWrite(CW,HIGH);
    tcw = true;
    turntime = millis();
    ShowIfDebug("Start CW");
   }
   if ((needCCW) && (!tccw)) {
    needCCW = false;
    digitalWrite(CW,LOW);
    digitalWrite(CCW,HIGH);
    tccw = true;
    turntime = millis();
    ShowIfDebug("Start CCW");
    }
 
  //if over limits and turning, stop
    ok = true;
    if (tcw  && (dir >= LCW || dir >= MCW)) ok = false;
    if (tccw  && (dir <= LCCW || dir <= MCCW)) ok = false;
    
      if (!ok) {
      digitalWrite(CW,LOW);
      digitalWrite(CCW,LOW);
      tcw= false;
      tccw= false;
      ShowIfDebug("Over limit");
    }
 
}   
//------------------------------------------------------- 
void longTOshort(int count) {  //remove command from beginning
 for (i=0;i< count;i++) RigCmd.setCharAt(i,32); //set it to space:
 RigCmd.trim(); // trim away
}
//-------------------------------------------------------
void dump_state() {
 /*

rotctld Protocol Ver: 0
Rotor Model: 1
Minimum Azimuth: -180.000000
Maximum Azimuth: 180.000000
Minimum Elevation: 0.000000
Maximum Elevation: 90.000000

*/

 if (prech == '\n') {
     if (negdeg == 0)writeCli("0\n999\n0.000000\n360.000000\n0.000000\n0.000000\n");
     else if (negdeg == 1) writeCli("0\n999\n-180.000000\n180.000000\n0.000000\n0.000000\n");
     }
  else if (prech == '+') { 
      prech = '\n';
      if (negdeg == 0){
               writeCli("dump_state:\nrotctld Protocol Ver: 0\nRotor Model: 999\nMinimum Azimuth: 0.000000\nMaximum Azimuth: 360.000000\nMinimum Elevation: 0.000000\nMaximum Elevation: 0.000000\nRPRT 0\n");
              }
          else if (negdeg == 1){
               writeCli("dump_state:\nrotctld Protocol Ver: 0\nRotor Model: 999\nMinimum Azimuth: -180.000000\nMaximum Azimuth: 180.000000\nMinimum Elevation: 0.000000\nMaximum Elevation: 0.000000\nRPRT 0\n");
              }
         }
         else writeCli("RPRT -1\n");
      
ok = true;
}
//-------------------------------------------------------
  void get_pos(){
  //return antenna position and tilt 0;
  
  ShowIfDebug("Position "+String(fixheading,6)+"\n");
      if (prech == '\n') {
        writeCli(String(fixheading,6)+"\n0.000000\nRPRT 0\n");
      } else {
      if (prech == '+') prech = '\n';
      writeCli("get_pos:"+String(prech)+"Azimuth: "+String(fixheading,6)+String(prech)+"Elevation: 0.000000"+String(prech)+"RPRT 0\n"); 
      }
ok = true;
}
//-------------------------------------------------------
void set_pos(int away){
  longTOshort(away);
  String rep = "RPRT -1\n";
 Az,El ="";
// we must have 2 parameters 
for (i = 0; i < RigCmd.length(); i++) {
  if (RigCmd.substring(i, i+1) == " ") {
    Az = RigCmd.substring(0, i);
    El = RigCmd.substring(i+1);
    break;
  }
}
ShowIfDebug("Pos split parameters: "+Az+" & "+El);


if ( ! El.equals("") ) {   // not used, but required for syntax

/*
  count abs dir from Az backwards decide from that the turning direction need
  
  heading = (dir-MCCW) * digideg;
  if (heading > 360) heading = heading - 360;
  if (heading < 0) heading = 360 + heading;
  fixheading = heading + truefix;
  if (fixheading > 360) fixheading = fixheading - 360;
  if (fixheading < 0) fixheading = 360 + fixheading;
*/  

  // from needed true heading to relative heading
  float MYheading = Az.toInt();
  if (negdeg) {
  MYheading = MYheading + 180.0;
  ShowIfDebug("0..360 conversion: "+String(MYheading));
  }
  MYheading = (MYheading - truefix);
  if (MYheading > 360) MYheading = MYheading - 360;
  if (MYheading < 0) MYheading = 360 + MYheading;
  
  // from relative heading to Poti ADCreading
  int MYdir = (MYheading / digideg ) + MCCW;
  ShowIfDebug("FixHead:"+String(fixheading)+" RelHead:"+String(heading)+" fix:"+String(truefix)+" digideg:"+String(digideg)+" Poti:"+String(dir)+" MCCW:"+String(MCCW));
  ShowIfDebug("Needed:"+Az+" RelHead:"+String(MYheading)+" Head2ADC:"+String(MYheading / digideg)+" Poti:"+String(MYdir));
  
  if (MYdir < dir) {
    needCCW = true;
    turnTo = Az.toInt();
  }
  if (MYdir > dir) {
    needCW = true;
    turnTo = Az.toInt();
  }
  rep = "RPRT 0\n";
}
     if (prech == '\n') writeCli(rep);
     else {
      if (prech == '+') prech = '\n';
      writeCli("set pos: "+Az+" "+El+String(prech)+rep);
     } 
ok = true;
}
//-------------------------------------------------------
void set_move(int away){;
// 8 CCW, 16 CW
 longTOshort(away); //remove command, leave parameters
 ShowIfDebug("Move parameters:"+RigCmd);
 String rep = "RPRT -1\n";
 Az,El ="";
 
for (i = 0; i < RigCmd.length(); i++) {
  if (RigCmd.substring(i, i+1) == " ") {
    Az = RigCmd.substring(0, i);
    El = RigCmd.substring(i+1);
    break;
  }
}
 ShowIfDebug("Move split parameters:"+Az+" & "+El);

if ( ! El.equals("") ) {   // not used, but required for syntax
  if (Az.equals("16")) {
       rep = "RPRT 0\n";
       needCW = true;
       ShowIfDebug("Turn CW");
       }
  if (Az.equals("8")) {
       rep = "RPRT 0\n";
       needCCW = true;
       ShowIfDebug("Turn CCW");
       }
}
     if (prech == '\n') writeCli(rep);
     else {
      if (prech == '+') prech = '\n';
      writeCli("move: "+Az+" "+El+String(prech)+rep);
     }
ok = true;
}
//-------------------------------------------------------
void set_conf(int away){
  longTOshort(away); //remove command, leave parameters
  String rep = "RPRT -1\n";
  int Mdir = dir;
  // set parameter to be as current rotator position or give value 
  if (RigCmd.indexOf("MCW",0) == 0){
      if (RigCmd.length() > 4) { //there may be parameter
      longTOshort(3);
      if (RigCmd.toInt() > 0) Mdir = int(RigCmd.toInt()); //you can't set value 0!
      RigCmd = "MCW "+RigCmd; //for long reporting
      }
      MCW = Mdir;
      chklimits();
      writeparms();
      rep = "RPRT 0\n";
    }
  // set parameter to be as current rotator position or give value   
  if (RigCmd.indexOf("MCCW",0) == 0){
      if (RigCmd.length() > 5) { //there may be parameter
      longTOshort(4);
      if (RigCmd.toInt() > 0) Mdir = int(RigCmd.toInt()); //you can't set value 0!
      RigCmd = "MCCW "+RigCmd; //for long reporting 
      }
      MCCW = Mdir;
      chklimits();
      writeparms();
      rep = "RPRT 0\n";
    }
  if (RigCmd.indexOf("LCW",0) == 0){ //parameter M sets same as max , else current position
     if (RigCmd.indexOf("M",0) > 3){
       ShowIfDebug("LCW = MCW");
       LCW =MCW;
     } else {
       LCW = Mdir;
       ShowIfDebug("LCW = "+String(LCW));
     }
      chklimits();
      writeparms();
      rep = "RPRT 0\n";
    }
  if (RigCmd.indexOf("LCCW",0) == 0){ //parameter M sets same as max , else current position
     if (RigCmd.indexOf("M",0) > 4){
       ShowIfDebug("LCCW = MCCW");
       LCCW =MCCW;
     } else {
       LCCW = Mdir;
       ShowIfDebug("LCCW = "+String(LCCW));
     }
      chklimits();
     writeparms();
      rep = "RPRT 0\n";
    }
    
  if (RigCmd.indexOf("SOUTH",0) == 0){
      if (RigCmd.length() > 6) { //there may be parameter
      longTOshort(5);
      truefix = int(RigCmd.toInt()); 
      } else {
       truefix = 180 - heading; //from current position
      }
     writeparms();
     rep = "RPRT 0\n";
    }

    
  if (RigCmd.indexOf("TRACE",0) == 0){ 
    if (RigCmd.indexOf("1",0) > 5){  // to trace poti reading
      trace = true;
      ShowIfDebug("Trace on");
      rep = "RPRT 0\n";
    } 
    if (RigCmd.indexOf("0",0) > 5){
      trace = false;
      ShowIfDebug("Trace off");
      rep = "RPRT 0\n";
    }
  }
  if (RigCmd.indexOf("DEBUG",0) == 0){ 
    if (RigCmd.indexOf("1",0) > 5){  // to get debug msgs to serial port
      debug = true;
      DbgCliNr= 99;
      ShowIfDebug("Debug on");
      rep = "RPRT 0\n";
    } 
    if (RigCmd.indexOf("0",0) > 5){
      ShowIfDebug("Debug off");
      debug = false;
      DbgCliNr= 99;
      rep = "RPRT 0\n";
    }
  } 
  
  if (RigCmd.indexOf("NEGDEG",0) == 0){ 
    if (RigCmd.indexOf("1",0) > 6){  // values either 0..360 or -180..180
      negdeg = true;
      writeparms();
      ShowIfDebug("-180/180 on");
      rep = "RPRT 0\n";
    } 
    if (RigCmd.indexOf("0",0) > 6){
      ShowIfDebug("-180/180 off");
      negdeg = false;
      writeparms();
      rep = "RPRT 0\n";
    }
  }    
      if (prech == '\n') writeCli(rep);
     else {
      if (prech == '+') prech = '\n';
      writeCli("set_conf: "+RigCmd+String(prech)+rep);
     } 
ok = true;
}
//-------------------------------------------------------
void set_park(int away){
  longTOshort(away); //remove command, leave parameters
  // stop by setting both relays off
      digitalWrite(CW,LOW);
      digitalWrite(CCW,LOW);
      tcw= false;
      tccw= false;
      ShowIfDebug("Stopped turning");
     if (prech == '\n') writeCli("RPRT 0\n");
     else {
      if (prech == '+') prech = '\n';
      writeCli("park:"+String(prech)+"RPRT 0\n");
     }
ok = true;
}
//-------------------------------------------------------
void set_stop(int away){
  longTOshort(away); //remove command, leave parameters
  // stop by setting both relays off
      digitalWrite(CW,LOW);
      digitalWrite(CCW,LOW);
     tcw= false;
     tccw= false;
     ShowIfDebug("Stopped turning");
     if (prech == '\n') writeCli("RPRT 0\n");
     else {
      if (prech == '+') prech = '\n';
      writeCli("stop:"+String(prech)+"RPRT 0\n");
     }
ok = true;
}
//-------------------------------------------------------
void set_reset(int away){
    String rep;
    longTOshort(away); //remove command, leave parameters
    digitalWrite(CW,LOW);
    digitalWrite(CCW,LOW);
    tcw= false;
    tccw= false;
    ShowIfDebug("Stopped turning");
    if (RigCmd.charAt(0)=='1') {
      MCW =1023; //clockwise reading max
      MCCW =0;   //counterclockwise reading max
      LCW = MCW;
      LCCW = MCCW;
      truefix = 0;
      negdeg = 0;
      writeparms();
  } else {
    ShowIfDebug("Restarting");
  }
   rep = "RPRT 0\n";
     if (prech == '\n') writeCli(rep);
     else {
      if (prech == '+') prech = '\n';
      writeCli("reset: "+String(prech)+rep);
     } 
    delay(2000);
    ResetEsp(ResEsp);
}
//-------------------------------------------------------
void get_info(){
  // print rotatator name and all settings
     if (prech == '\n')
     writeCli("OH1KH rot "+vers+"\nPoti :"+String(dir)+" Reldir:"+String(heading)+" Fixdir:"+String(fixheading)+
             "\n MCW:"+String(MCW)+"  LCW:"+String(LCW)+
             "\nMCCW:"+String(MCCW)+" LCCW:"+String(LCCW)+
             "\nfix:"+String(truefix)+"\n");
     else {
      if (prech == '+') prech = '\n';
      writeCli("get_info:"+String(prech)+"Info: OH1KH rot "+String(prech)+"Poti :"+String(dir)+" Reldir:"+String(heading)+" Fixdir:"+String(fixheading)+String(prech)+
      " MCW:"+String(MCW)+"  LCW:"+String(LCW)+
      "MCCW:"+String(MCCW)+" LCCW:"+String(LCCW)+String(prech)+
      "fix:"+String(truefix)+String(prech)+"RPRT 0\n");
     }    
     helpCee();
ok = true;
}  
//-------------------------------------------------------
void prefix() {
    prech = cmd;
    if (debug) { Serial.print("Command limiter is:");Serial.println(prech);}
    RigCmd.setCharAt(0,32); //set it to space:
    RigCmd.trim(); // trim away
    cmd = RigCmd.charAt(0); // new first        
}
//-------------------------------------------------------
void ParseCommand(){
  cmd = RigCmd.charAt(0);
  if (debug) {Serial.print("Command parser. First chr is:");Serial.println(cmd);}
  
   switch (cmd) { // prefixes +,;|
     case 43 : prefix(); break;
     case 124 : prefix(); break;
     case 44 : prefix(); break;
     case 59 : prefix(); break;
    default: {
        prech = '\n';
        if (debug) Serial.print("No prefixes +,;|\n");
        break;
       }
    }//switch prefixes
    
    yield();
    
 //supported rigctld short commands
 switch (cmd) {

   // backslash
   case 92 : 
    { 
     RigCmd.setCharAt(0,32); //set it to space:
     RigCmd.trim(); // trim away
     ShowIfDebug("Is a Long command");
     ok = false;    // ok = command found and executed 
     if (RigCmd.indexOf("dump_state",0) == 0 ) dump_state();
     if (RigCmd.indexOf("get_pos",0) == 0 ) get_pos();
     if (RigCmd.indexOf("get_info",0) == 0 ) get_info();
    
     if (RigCmd.indexOf("set_pos",0) == 0 ) set_pos(7);
     if (RigCmd.indexOf("set_move",0) == 0 ) set_move(8);
     if (RigCmd.indexOf("set_conf",0) == 0 ) set_conf(8);
     if (RigCmd.indexOf("stop",0) == 0 ) set_stop(4);
     if (RigCmd.indexOf("park",0) == 0 ) set_park(4);
     if (RigCmd.indexOf("reset",0) == 0 )  set_reset(5);  
    } 
    break;  //longmode command
  
  
  case '?': dump_state();
   break; 
  
  case 'p': get_pos();
   break; 
  
  case 'P': set_pos(1);
   break; 

  case 'Q': disClient();
   break;    

  case 'q': disClient();
   break;    
  
  case 'M': set_move(1);
   break;    
   
  case 'C': set_conf(1);
   break; 
   
  case 'c': helpCee();  //unstandard hamlib command prints help
   break; 
   
  case 'K': set_park(1);
   break;  

  case 'S': set_stop(1);
   break;  
  
  case 'R': set_reset(1);
   break; 
   
  case '_': get_info();
   break; 
  
  default : 
   if (RigCmd.length()>1) {
    if (prech == '\n') writeCli("RPRT -11\n");
     else {
      if (prech == '+') prech = '\n';
      writeCli(RigCmd+":"+String(prech)+"RPRT -11\n");
     }
    } 
   break; 
  }//switch cmd end
  
  if (!ok) { // happens if long mode command is not found. Unknown shorts handled by switch/default
     if (prech == '\n') writeCli("RPRT -11\n");
     else {
      if (prech == '+') prech = '\n';
      writeCli(RigCmd+":"+String(prech)+"RPRT -11\n");
     }
  }
  
  yield();
}
//------------------------------------------------------- 
