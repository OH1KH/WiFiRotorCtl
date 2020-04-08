void helpCee(){
 writeCli("\n");
 writeCli("Parameters for command C\n");
 writeCli("MCW  [val] set parameter to be as current rotator position or give value {saved}\n");
 writeCli("MCCW [val] set parameter to be as current rotator position or give value {saved}\n"); 
 writeCli("LCW    M   parameter sets same as max , else current position {saved}\n");
 writeCli("LCCW   M   parameter sets same as max , else current position {saved}\n");
 writeCli("SOUTH [val] set south to be as current rotator position or give value {saved}\n");
 writeCli("TRACE  0|1 to trace poti reading in serial port. Needs also DEBUG\n"); 
 writeCli("DEBUG  0|1 to print debug messages to serial port \n");
 writeCli("NEGDEG 0|1 values either 0..360 or -180..180  {saved}\n");
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
