gametiny  
---------------------------------------------------------------------------
I was inspired by the mini game consoles based on the energy efficieng 3V Attiny85 MCU that can run for hours on a CR2032 3V button battery. 
Attiny Joypad  created by Daniel Champagne.. (Electro L.I.B) 2018 GPL V3 is a great platform with the 4 way directional buttons/joystick and another fire button to write games on.  The  other platform called Attiny Arcade created by webbloggles is also a  great mini game console with the simplicity of having only two buttons. Both platforms inspired me to create something taking the best from both worlds.

This game board I called gametiny is based on the Attiny Joypad created by Daniel Champagne. I modified it  to add a header for USBasp programmer that doubles up as a socket for the OLED plus adding a switch to pull PB3 to ground. This allows the interrupt codes to work for both PB1 and PB3, making it easier to convert games written for the Attiny arcade to work on Attiny joypad.

Schematics and games converted to work on gametiny can be found here
https://github.com/cheungbx/gametiny

Video link of a youtube video with the instructions on how to make this gametiny board.
https://studio.youtube.com/video/RduFxd-Zp8M/edit


Links
Original Tiny Joypad design by Daniel Champagne.
https://sites.google.com/view/arduino-collectionLinks 

Original Attiny Arcade keychain kit
http://webboggles.com/attiny85-game-kit-assembly-instructions/

I converted these games for the AttinyArcade  (originally created by Andy Jackson - Twitter: @andyhighnumber)  to work for gametiny.
https://github.com/andyhighnumber/Attiny-Arduino-Games


Arduino board driver for attiny85
https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json

schematics and source codes in  https://github.com/cheungbx/gametiny

I purchased the parts from these taobao links.
128x64 i2c mono OLED -  
https://item.taobao.com/item.htm?spm=a1z09.2.0.0.52cd2e8dEH1bEe&id=553116768996&_u=i106a08oda4a

USBasp programmer 
https://detail.tmall.com/item.htm?id=573399213944&spm=a1z09.2.0.0.67002e8dEmFVw9&_u=i106a08o8c9c

attiny 85 DIP8 chips
https://item.taobao.com/item.htm?spm=a1z09.2.0.0.2e622e8dl6mZNt&id=35831130850&_u=i106a08o4917

buttons and other misc. items - https://item.taobao.com/item.htm?spm=a1z09.2.0.0.52cd2e8dEH1bEe&id=552220368876&_u=i106a08oc0d5

Transistors from HK Shumshuipo - Ip Liu Street electronic shops.



 
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




Refer to the schematics on the files section. Schematics created on easyEDA.com.

*** Games (modified to work on gametiny) and tools for testing gametiny in this repository ***
Test_ATtiny.ino - test screen, buttons and speaker of the gametiny. Useful tools if your button is not working.
BatBonanzaAttinyArcade.ino - traditional ping pong game , modified to have single button, dual button, and two players mode.
Frogger_Attiny_Arcade - push a frog to cross the street and river.
MorseAttinyArcade.ino - a great too to practice keying morse codes that listend into what you keyed in and convert that to text.
Schematic_gametiny - schematic of the gametiny in pdf and png format - This modification  adds a header for USBasp programmer that doubles up as a socket for the OLED.
SpaceAttackAttiny.ino - modified buttons logic to play on gametiny
Tetris_Multi_Button.ino - modified buttons logic to play on gametiny
Tiny_space_invaders.ino - a copy of the original game without modification.
tinyarkanoid.ino - a copy of the original game without modification.
tinybomber.ino - a copy of the original game without modification.
tinygilbert.ino - a copy of the original game without modification.
tinypacman.ino - a copy of the original game without modification.
tinyPinball.ino - a copy of the original game without modification.
UFO_Breakout_Arduino.ino - load 2 games at the same time to your gametiny and select which one to play. Modified for gametiny buttons.
UFO_Stacker_Attiny.ino - dual game set for UFO attack and Stacker . Modified for gametiny.
WrenRollercoasterAttinyArcade.ino - roller coaster game modified for gaemtiny.
