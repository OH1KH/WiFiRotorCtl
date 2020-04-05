# WiFiRotorCtl
Simple ESP12e driven rotor controller

Had old rotator, but no control box ....
Had ESP12e and junkbox ....


WiFiRot can be accessed via telnet (port 4533) or via serial connection (same as used for ESP12e programming)
This rotor controller is last tested with cqrlog 2.4.0(115)

On demand WiFi setup can be accessed at start when ESP's internal led is blinking by pressing Program button.
Setup uses WiFimanager that starts WiFi access point called ROTCTL.
Connecting to this accespoint with PC, tbalet or phone and issuing any http address to web browser opens
config page.
WiFiRot scans available networks and prompts for SSID and password pair. 
After successfull connect you will find WiFiRot from your network with address your networks DHCP did give to it
and then "telnet that.address 4533" for cmmanding rotator.

If you do not have any networks you can reset WiFiRot again and wait that it starts, but not pressing Program button while led blinks. WiFiRot staus then
on it's own accesspont and connecting your PC tho that network you are able to "telnet 192.168.4.1 4533" and command
rotator.

Or you can control rotator via serial interface with USB adapter (same as used for programming).


    - HamLib rotctld compatible short and long commands

		  - only for Azimuth rotator

		  - C , \set_conf specific extensions (one prameter per C command):

			MCW [value]     set clockwise (max) ADC reading of rotator poti (0 .. 1023)
					from [value] or current rotator position
			MCCW [value]    set counter clockwise (max) ADC reading of rotator poti (0 .. 1023)
					from [value] or current rotator position

				**NOTE!** these values corresponds rotator turn of 360 degrees

			LCW		set clockwise turn limit (before max reading MCW) if needed.
					Set to current rotator position.(as ADC reading of rotator poti 0 .. 1023)
			LCCW		set counter clockwise turn limit (before max reading MCCW) if needed.
					Set to current rotator position.(as ADC reading of rotator poti 0 .. 1023)
			
				**NOTE!** 	If there is something that prevents rotator to turn full 360 degrees
					these values should be used. Otherwise they are set same as max values by program.

			SOUTH		Set true south (180deg) to current rotator position.
			NEGDEG [0 | 1] 	response Az positions -180..180 or 0..360
			TRACE  [0 | 1]	Continious reading of ADC value (+ converted to degrees) on/off
			DEBUG  [0 | 1] 	Add some debugging messaged to serial connection

			MCW, MCCW, LCW, LCCW, SOUTH, NEGDEG are saved to EEprom to keep values over power restets.

When you have succesfully connected via telnet or serial line the first thing to do is run rotator to clockwise maximum
position with button (or by command).
**NOTE** that rotator usually turns a bit more than 360 degrees so you can set your clockwise maximum a little bit less
than the physical limit of rotator.
At that point set trace on with "C TRACE 1" to see ADC values read from rotator poti.
Adjust trimmer so that Poti reading is less than 1023. Around 970 or so.
Give command "C MCW" to save this position as maximum clocwise position.

Then turn to counter clockwise 360 degrees, no more or less. ADC Poti should read now something more than 0.
If not find another maximum position, adjust trimmer and save MCW.
If the counter clockwise reading is more than 0 give command "C MCCW" to save this as maximum postion of CCW.

Now program counts how many ADC digits corrsponds to one degraa physical turn. Rotator is calibrated by
relative degrees.
Next thing is to turn antenna pont to true south and then give command "C SOUTH" to save the fix in degrees between
relative position and true position.

After that you should be able to command your rotator with P, \set_pos command.

Values are saved to EEprom, but it is wise to write down MCW and MCCW just in case you have to set them again
(you can give them as values after MCW and MCCW, no need to turn rotator if mechanics are not changed).

[id]: https://github.com/Hamlib/Hamlib
Supported rotctld commans are (following text is copied from man pages of  [HamLib][id]/rotctld):

    COMMANDS
       Commands can be sent over the TCP socket either as a single char, or as
       a long command name plus the value(s) space separated on one '\n'  ter‐
       minated line. See PROTOCOL.

       Since  most  of  the  Hamlib operations have a set and a get method, an
       upper case letter will be used for set methods whereas the  correspond‐
       ing  lower  case  letter refers to the get method.  Each operation also
       has a long name; prepend a backslash to send a long command name.

       Example (Perl):  `print  $socket  "\\dump_caps\n";'  to  see  what  the
       rotor's  backend  can  do (NOTE: In Perl and many other languages a '\'
       will need to be escaped with a preceding '\' so that  even  though  two
       backslash  characters  appear  in  the code, only one will be passed to
       rotctld.  This is a possible bug, beware!).

       Please note that the backend for the rotator to be controlled,  or  the
       rotator  itself may not support some commands. In that case, the opera‐
       tion will fail with a Hamlib error message.

       Here is a summary of the supported commands (In the case of "set"  com‐
       mands  the  quoted  string is replaced by the value in the description.
       In the case of "get" commands the quoted string is the key name of  the
       value returned.):

       P, set_pos 'Azimuth' 'Elevation'
              Set position: Azimuth and Elevation as double precision floating
              point values.

       p, get_pos
              Get position: 'Azimuth'  and  'Elevation'  as  double  precision
              floating point values.

       M, move 'Direction' 'Speed'
              Move the rotator in a specific direction at the given rate.

              Values  are  integers  where Direction is defined as 2 = Up, 4 =
              Down, 8 = Left, and 16 = Right.  Speed is an integer  between  1
              and  100.   Not all backends that implement the move command use
              the Speed value.  At this time  only  the  gs232a  utilizes  the
              Speed parameter.

       S, stop
              Stop the rotator.

       K, park
              Park the antenna.

       C, set_conf 'Token' 'Value'
              Set Token to Value.
              Backend dependent.  Needs testing.

       R, reset 'Reset'
              Reset the rotator.

              Integer value of '1' for Reset All. (erases EEprom values!!)

       _, get_info
              Get misc information about the rotator.


 
  Extended Response Protocol

       An EXPERIMENTAL Extended Response protocol  has  been  introduced  into
       rotctld  as  of February 10, 2010.  This protocol adds several rules to
       the strings returned by rotctld and adds a rule for the command syntax.

       1. The command received by rotctld is echoed with its long command name
       followed  by  the value(s) (if any) received from the client terminated
       by the  specified  response  separator  as  the  first  record  of  the
       response.

       2.  The  last  record of each block is the string "RPRT x\n" where x is
       the numeric return value of the Hamlib backend function that was called
       by the command.

       3.  Any records consisting of data values returned by the rotor backend
       are prepended by a string immediately followed by a colon then a  space
       and then the value terminated by the response separator. e.g. "Azimuth:
       90.000000\n" when the command was prepended by '+'.

       4. All commands received will be acknowledged by rotctld  with  records
       from  rules  1  and 2.  Records from rule 3 are only returned when data
       values must be returned to the client.

       An example response to a +P command  command sent from the shell prompt
       (note the prepended '+'):

       $ echo "+P 90 45" | nc -w 1 localhost 4533
       set_pos: 90 45
       RPRT 0

       In this case the long command name and values are returned on the first
       line and the second line contains the  end  of  block  marker  and  the
       numeric rotor backend return value indicating success.

       In this case, as no value is passed to rotctld, the first line consists
       only of the long command name.  The final line shows that  the  command
       was processed successfully by the rotor backend.

       Invoking  the  Extended Response protocol requires prepending a command
       with a punctuation character.  As shown in the examples above, prepend‐
       ing a '+' character to the command results in the responses being sepa‐
       rated by a newline character ('\n').  Any other  punctuation  character
       recognized  by  the  C  ispunct() function except '\', '?', or '_' will
       cause that character to become the response separator  and  the  entire
       response will be on one line.

       Separator character summary:

       '+'
              Each record of the response is appended with a newline ('\n').

       ';', '|', or ','
              Each  record  of the response is appended by the given character
              resulting in entire response on one line.

              Common record separators for text representations of spreadsheet
              data, etc.

       '?'
              Reserved for 'help' in rotctl short command

       '_'
              Reserved for \get_info short command



       For example, invoking a ;\get_pos query with a leading ';' returns:

       get_pos:;Azimuth: 90.000000;Elevation: 45.000000;RPRT 0

       Or, using the pipe character '|' returns:

       get_pos:|Azimuth: 90.000000|Elevation: 45.000000|RPRT 0

       And a \set_pos command prepended with a '|' returns:

       set_pos: 135 22.5|RPRT 0


       An example response to a +\get_pos query:

       $ echo "+\get_pos" | nc -w 1 localhost 4533
       get_pos:
       Azimuth: 90.000000
       Elevation: 45.000000
       RPRT 0


		
 

