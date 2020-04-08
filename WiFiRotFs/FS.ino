//-----------------------------------------
 void fWwrite(int t) {

 byte tl = t % 256;
 byte th = t / 256;
 f.print(tl);
 f.print(th);
}
//-----------------------------------------
 int fWread() {

 byte tl = f.read();
 byte th = f.read();
 int result  = 256*th+tl;
 return result;
}
//----------------------------------------
void writeparms(){

   //Create New File And Write Data to It
  //w=Write Open file for writing
  f = SPIFFS.open(filename, "w");
  
  if (!f) {
    ShowIfDebug("file open failed");
  }
  else
  {
      //Write data to file
      ShowIfDebug("Writing Data to File");
      fWwrite(MCW);
      ShowIfDebug("MaxCW "+String(MCW)+ " saved");
      fWwrite(MCCW);
      ShowIfDebug("MaxCCW "+String(MCCW)+ " saved");
      fWwrite(truefix);
      ShowIfDebug("Truefix "+String(truefix)+ " saved");
      fWwrite(LCW);
      ShowIfDebug("LimitCW "+String(LCW)+ " saved");
      fWwrite(LCCW);
      ShowIfDebug("LimitCCW "+String(LCCW)+ " saved");
      f.print(negdeg);
      ShowIfDebug("NegDeg "+String(negdeg)+ " saved");
      f.close();  //Close file
  }
 
}

//----------------------------------------
void readparms(){
 int i;
 byte b;
  
  //Read File data
  f = SPIFFS.open(filename, "r");
  
  if (!f) {
    ShowIfDebug("file open failed");
  }
  else
  {
      ShowIfDebug("Reading Data from File:");
      //Data from file
          MCW = fWread();
          ShowIfDebug("MaxCW "+String(MCW)+ " saved");
          MCCW = fWread();
          ShowIfDebug("MaxCCW "+String(MCCW)+ " saved");
          truefix = fWread();
          ShowIfDebug("Truefix "+String(truefix)+ " saved");
          LCW = fWread();
          ShowIfDebug("LimitCW "+String(LCW)+ " saved");
          LCCW = fWread();
          ShowIfDebug("LimitCCW "+String(LCCW)+ " saved");
          negdeg = f.read();
          ShowIfDebug("NegDeg "+String(negdeg)+ " saved");
      f.close();  //Close file
      ShowIfDebug("File Closed");
  }  
}
//----------------------------------------#
