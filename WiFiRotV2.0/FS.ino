void PrintFile(){
  char a; 
  Serial.println(F("Opening file for check read"));
   // Open file for reading
  File file = SPIFFS.open(filename,"r");
  if (!file) {
    Serial.println(F("Failed to read file"));
    return;
  }

  // Extract each characters by one by one
  while (file.available()) {
   a = (char)file.read();
   Serial.print(a);
   if (a == ',') {
    Serial.println();
   }
  }
  Serial.println();

  // Close the file
  file.close();
}

void PrintConfig(){
  
Serial.println(F("Printing current configuration from memory"));
Serial.print(F("CW reading "));Serial.println( config.MCW);
Serial.print(F("CW limit max "));Serial.println( config.LCW);
Serial.print(F("CCW reading ")); Serial.println( config.MCCW);
Serial.print(F("CCW limit max "));Serial.println( config.LCCW);
Serial.print(F("degrees of turn between true south and current heading "));Serial.println( config.truefix);
Serial.print(F("neg Az ok "));       Serial.println( config.negdeg);

}

void ReadSettings() {
 
  //read configuration from FS json
 
    if (SPIFFS.exists(filename)) {
      //file exists, reading and loading
      Serial.println(F("reading config file"));
      File file = SPIFFS.open(filename,"r");

     // Allocate a temporary JsonDocument
     // Don't forget to change the capacity to match your requirements.
     // Use arduinojson.org/v6/assistant to compute the capacity.
     StaticJsonDocument<512> doc;

     // Deserialize the JSON document
     DeserializationError error = deserializeJson(doc, file);
  if (error) {
      Serial.println(F("Failed to read file, using default configuration"));
      }
     else {
        Serial.println(F("Loading settings"));
        // Copy values from the JsonDocument to the Config
        config.MCW          = (int) doc["MCW"];
        config.LCW          = (int) doc["LCW"];
        config.MCCW         = (int) doc["MCCW"];
        config.LCCW         = (int) doc["LCCW"];
        config.truefix      = (int) doc["truefix"];
        config.negdeg       = (byte)doc["negdeg"];
     }

     // Close the file (Curiously, File's destructor doesn't close the file)
     file.close();
    } else { 
      Serial.println(F("Failed to find file, using default configuration"));
    }
  //end read
}



void SaveSettings() {
  Serial.println(F("saving config"));
  // Delete existing file, otherwise the configuration is appended to the file
  SPIFFS.remove(filename);

  // Open file for writing
  File file = SPIFFS.open(filename, "w");
  if (!file) {
    Serial.println(F("Failed to create file"));
    return;
  }

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<512> doc;

  // Set the values in the document
  doc["MCW"]      =        config.MCW;
  doc["LCW"]      =        config.LCW;
  doc["MCCW"]     =        config.MCCW;
  doc["LCCW"]     =        config.LCCW;
  doc["truefix"]  =        config.truefix;
  doc["negdeg"]   =        config.negdeg;

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  // Close the file
  file.close();
 
}
  
