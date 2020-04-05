//-------------------------------------------------------
boolean eecheck() {
  byte check = 0;
// byte 0 xored checksum of whole area
 for (int address = 1;address <= EETOP; ++address){ check = EEPROM.read(address) ^ check;};
  if (EEPROM.read(0) != check) { return false; } else { return true; };
}
//-------------------------------------------------------
void eesum() {
 //write checksum to 0
  byte check = 0;
  for (int address = 1;address <= EETOP; ++address){ check = EEPROM.read(address) ^ check;};
  EEPROM.write(0,check); 
  EEPROM.commit(); //note here the commit!
}
//-------------------------------------------------------
boolean eestart() {
  boolean ret = true;
  ShowIfDebug("Reserve EEPROM");
  EEPROM.begin(EETOP); 
  ShowIfDebug("check EEPROM ");
  if (!eecheck()) {
      ShowIfDebug("Is NOT ok. Writing 0s to EEPROM");
      for (int address = 0;address <= EETOP; ++address){ EEPROM.write(address,0); };
      EEPROM.commit(); //note here the commit!
      ret = false;
     } else {
     ShowIfDebug("Reading EEPROM is ok.");
   }
 return ret;  
}
//-------------------------------------------------------
int eewriteW(int pos, int t) {  //write word (startpos,word)

 byte tl = t % 256;
 byte th = t / 256;
 EEPROM.write(pos,tl);
 EEPROM.write(pos+1,th);
 EEPROM.commit(); //note here the commit!

}             
//-------------------------------------------------------
int eewrite() {  //write CW and CCW analog max values
 
 eewriteW(1,MCW);
 ShowIfDebug("MaxCW "+String(MCW)+ " saved");
 eewriteW(3,MCCW);
 ShowIfDebug("MaxCCW "+String(MCCW)+ " saved");
 eewriteW(5,truefix);
 ShowIfDebug("Truefix "+String(truefix)+ " saved");
 eewriteW(7,LCW);
 ShowIfDebug("LimitCW "+String(LCW)+ " saved");
 eewriteW(9,LCCW);
 ShowIfDebug("LimitCCW "+String(LCCW)+ " saved");
 eewriteW(11,negdeg);
 ShowIfDebug("NegDeg "+String(negdeg)+ " saved");
 eesum();
 
}
//-------------------------------------------------------
int eeread() { //read CW and CCW analog max values
  if (eecheck) {
      MCW = EEPROM.read(1) + 256 * EEPROM.read(2);
      ShowIfDebug("MaxCW "+String(MCW)+ " read");
      MCCW = EEPROM.read(3) + 256 * EEPROM.read(4);
      ShowIfDebug("MaxCCW "+String(MCCW)+ " read");
      truefix = EEPROM.read(5) + 256 * EEPROM.read(6);
      ShowIfDebug("Truefix "+String(truefix)+ " read");
      LCW = EEPROM.read(7) + 256 * EEPROM.read(8);
      ShowIfDebug("LimitCW "+String(LCW)+ " read");
      LCCW = EEPROM.read(9) + 256 * EEPROM.read(10);
      ShowIfDebug("LimitCCW "+String(LCCW)+ " read");
      negdeg = EEPROM.read(11) + 256 * EEPROM.read(12);
      ShowIfDebug("NegDeg "+String(negdeg)+ " read");
    } else {
      MCW =1023; //clockwise reading max
      MCCW =0; //counterclockwise reading max
      ShowIfDebug("EE-error CW & CCW max set as 1023 & 0 "); 
    }
    
    chklimits();
    
} 
