
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
      f.println(String(MCW));
      ShowIfDebug("MaxCW "+String(MCW)+ " saved");
      f.println(String(MCCW));
      ShowIfDebug("MaxCCW "+String(MCCW)+ " saved");
      f.println(String(truefix));
      ShowIfDebug("Truefix "+String(truefix)+ " saved");
      f.println(String(LCW));
      ShowIfDebug("LimitCW "+String(LCW)+ " saved");
      f.println(String(LCCW));
      ShowIfDebug("LimitCCW "+String(LCCW)+ " saved");
      f.println(String(negdeg));
      ShowIfDebug("NegDeg "+String(negdeg)+ " saved");
      f.close();  //Close file
  }
 
}
//----------------------------------------
void readparms(){
String s,r;
int i,p;  
int t[6]={0,0,0,0,0,0};

  //Read File data
  f = SPIFFS.open(filename, "r");
  
  if (!f) {
    ShowIfDebug("file open failed");
  }
  else
  {
      ShowIfDebug("Reading Data from File:");
      //Data from file
          s= f.readString();
          ShowIfDebug(s);

          r=""; p=0;
          for (i=0;i<s.length();i++) {

           if (s[i] != '\n') {
                    r= r+s[i];
           } else {
            t[p] = r.toInt();
            p++;
            r="";
           }
          }

          MCW = t[0];
          ShowIfDebug("MaxCW "+String(MCW)+ " read");
          MCCW = t[1];
          ShowIfDebug("MaxCCW "+String(MCCW)+ " read");
          truefix = t[2];
          ShowIfDebug("Truefix "+String(truefix)+ " read");
          LCW = t[3];
          ShowIfDebug("LimitCW "+String(LCW)+ " read");
          LCCW = t[4];
          ShowIfDebug("LimitCCW "+String(LCCW)+ " read");
          negdeg = t[5];
          ShowIfDebug("NegDeg "+String(negdeg)+ " read");
      f.close();  //Close file
      ShowIfDebug("File Closed");
  }  
}
//----------------------------------------#
