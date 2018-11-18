# tinyboy
Collection of games playable on Tiny Boy and schematics. Tony Boy is a modified version of the Attiny Joypad originally created by Daniel C. (Electro L.I.B) 2018 GPL V3. This modification  adds a header for USBasp programmer that doubles up as a socket for the OLED. Refer to the schematics on the files section. Schematics created on easyEDA.com.

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

*** Games (modified to work on Tinyboy) and tools for testing Tinyboy in this repository ***
Test_ATtiny.ino - test screen, buttons and speaker of the Tiny boy. Useful tools if your button is not working.
BatBonanzaAttinyArcade.ino - traditional ping pong game , modified to have single button, dual button, and two players mode.
Frogger_Attiny_Arcade - push a frog to cross the street and river.
MorseAttinyArcade.ino - a great too to practice keying morse codes that listend into what you keyed in and convert that to text.
Schematic_Tinyboy - schematic of the Tinyboy in pdf and png format - This modification  adds a header for USBasp programmer that doubles up as a socket for the OLED.
SpaceAttackAttiny.ino - modified buttons logic to play on Tinyboy
Tetris_Multi_Button.ino - modified buttons logic to play on Tinyboy
Tiny_space_invaders.ino - a copy of the original game without modification.
tinyarkanoid.ino - a copy of the original game without modification.
tinybomber.ino - a copy of the original game without modification.
tinygilbert.ino - a copy of the original game without modification.
tinypacman.ino - a copy of the original game without modification.
tinyPinball.ino - a copy of the original game without modification.
UFO_Breakout_Arduino.ino - load 2 games at the same time to your Tiny boy and select which one to play. Modified for Tiny boy buttons.
UFO_Stacker_Attiny.ino - dual game set for UFO attack and Stacker . Modified for Tiny boy buttons.
WrenRollercoasterAttinyArcade.ino - roller coaster game modified for Tiny boy.
