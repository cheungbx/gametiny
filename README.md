# tinyboy
Collection of games playable on Tiny Boy and schematics. Tony Boy is a modified version of the Attiny Joypad originally created by Based on original version of Attiny Joypad  created by Daniel C. (Electro L.I.B) 2018 GPL V3. This modification  adds a header for USBasp programmer that doubles up as a socket for the OLED. Refer to the schematics on the files section. Schematics created on easyEDA.com.

*** WARNING ***
Inserting the battery or the OLED or the header in the wrong direction may cause permanent damage to the ATTiny85 chip and/or the OLED, and/or the USB port of your computer
Beware of different pin layout of OLED from different manfacturers that may swap the positions of VCC and GND, and SCL and SDA.
ATtiny85 boards with USB ports are not suitable for this project. The bootloader on such boards uses up almost half of the program memory space with insufficient space for game program.
Unless you know how to solder surface mount components (SMD), buy the raw/bare dual in line ATtin85 for this project. and use an IC socket. 
Programming the ATtiny85 is a bit tricky. Not every Arduino UNO, NANO board will work. Clone Nano/Uno from China may not work.
USBasp programmer is preferred. You need to create the cable to connect the Tiny Boy header to the USBasp header as shown in the schematics at the bottom  right
 
*** TIPS ***
Game buttons are all 6x6 tap-tic buttons. The up/down/left/right buttons can be replaced by a 5-direction mini joystick-like integrated button.
Use small PC speakers or  buzzers without an internal beeper circuit as sound output,
Test out the circuit on a breadboard first before soldering on a circuit board.
Use laminated (isolated) wire for connection between points. Use the small soldering tip and apply flux.
