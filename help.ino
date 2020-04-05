void helpCee(){
 writeCli("\n");
 writeCli("Parameters for command C\n");
 writeCli("MCW  [val] set parameter to be as current rotator position or give value {eeprom}\n");
 writeCli("MCCW [val] set parameter to be as current rotator position or give value {eeprom}\n"); 
 writeCli("LCW    M   parameter sets same as max , else current position {eeprom}\n");
 writeCli("LCCW   M   parameter sets same as max , else current position {eeprom}\n");
 writeCli("SOUTH  0|1 heading|180 - heading {eeprom}\n");
 writeCli("TRACE  0|1 to trace poti reading in serial port. Needs also DEBUG\n"); 
 writeCli("DEBUG  0|1 to print debug messages to serial port \n");
 writeCli("NEGDEG 0|1 values either 0..360 or -180..180  {eeprom}\n");
 writeCli("\n");
 ok = true;
}
