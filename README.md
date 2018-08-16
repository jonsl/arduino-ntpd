# arduino-ntpd
A stand-alone Arduino GPS time server

1. download arduino ide: https://www.arduino.cc/en/Main/Donate Press 'Just Download' at the bottom of the screen

2. run the downloaded installer (in this case arduino-1.8.5-windows.exe) and press 'I Agree'->'Next'->'Next'

3. when the installer asks to install device software click 'Install' each time

4. double click the installed arduino ide on the desktop, and check 'Private networks, such as my home or work network' and click 'Allow access'

5. go to: https://github.com/jonsl/arduino-ntpd click 'Clone or Download'->'Download Zip'

6. make a new folder in your Documents directory called 'dev', and copy the downloaded 'arduino-ntpd-master.zip' into it, and unzip it in place

7. in the 'Documents\dev\arduino-ntpd-master\arduino-ntpd' folder, doble click the file 'arduino-ntpd.ino'

8. when the ide has started, go to 'Tools'->'Arduino/Genuino Uno'-> and select 'Arduino/Genuino Uno Mega or Mega 2650'

9. make sure 'Processor' is set to 'ATmega2560 (Mega 2560)'

10. go to port and make a list of all the COM ports there - then close the whole Arduino ide

11. plug in the arduino board that you will program into the computer via USB, and start the Arduino ide again.

12. go to 'Port'-> and select the COM port that is now present, that was not present before (in the list you made in 10.)

13. go to 'Sketch'->'Include Library'->'Add .ZIP Library' select the file 'Documents\dev\arduino-ntpd\arduino-ntpd\libraries\TinyGPS.zip' and click 'Open'

14. click the green tick mark (top left) to varify the library builds, you should see something like the following:
  Sketch uses 23392 bytes (9%) of program storage space. Maximum is 253952 bytes.
  Global variables use 2901 bytes (35%) of dynamic memory, leaving 5291 bytes for local variables. Maximum is 8192 bytes.

15. click the green right arrow next to the (tick mark) to upload the library to the board.
  - If the upload fails, simply unplug the board from the USB socket, plug it back in again and then press the green right arrow again.
  
