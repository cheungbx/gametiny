//          >>>>>  T-I-N-Y  P-A-C-M-A-N for ATTINY85  GPL v3 <<<<
//                  Programmer: Daniel Champagne 2018
//                 Contact EMAIL: phoenixbozo@gmail.com
//           https://sites.google.com/view/arduino-collection

//  Tiny PACMAN is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.

//  You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

//the code work at 16MHZ internal
//and use ssd1306xled Library for SSD1306 oled display 128x64
//Program chip with arduino uno as isp at 16 mhz internal!
#include <ssd1306xled.h>
#include "spritebank.h"

// SDA - pin 0 - IC pin 5
// SCL - pin 2 - IC pin 7 

// var public
uint8_t LEVELSPEED;
uint8_t GobbingEND;
uint8_t LIVE;
uint8_t INGAME;
uint8_t Gobeactive;
uint8_t TimerGobeactive;
uint8_t add;
uint8_t dotsMem[9];
int8_t dotscount;
uint8_t Frame;
// fin var public

enum {PACMAN=0,FANTOME=1,FRUIT=2};

void ResetVar(void){
LEVELSPEED=200;
GobbingEND=0;
LIVE=3;
Gobeactive=0;
TimerGobeactive=0;
add=0;
INGAME=0;
for(uint8_t t=0;t<9;t++){
dotsMem[t]=0xff;}
dotscount=0;
Frame=0;}

void setup() {
SSD1306.ssd1306_init();
pinMode(1,INPUT);
pinMode(4,OUTPUT);
pinMode(A0,INPUT); 
pinMode(A3,INPUT);}

void StartGame(PERSONAGE *Sprite){
if (INGAME==0) {
Sprite[1].x=76;
Sprite[1].y=3;
Sprite[2].x=75;
Sprite[2].y=4;
Sprite[3].x=77;
Sprite[3].y=3;
Sprite[4].x=76;
Sprite[4].y=4;
INGAME=1;}}

void loop() {
uint8_t t;
PERSONAGE Sprite[5];
NEWGAME:
ResetVar();
LIVE=3;
goto New;
NEWLEVEL:
if (LEVELSPEED>10) {LEVELSPEED=LEVELSPEED-10;
if ((LEVELSPEED==160)||(LEVELSPEED==120)||(LEVELSPEED==80)||(LEVELSPEED==40)||(LEVELSPEED==10)){    
if (LIVE<3) {LIVE++; for(t=0;t<=4;t++){
Sound(80,100);delay(300);
}}}}
New:
GobbingEND=(LEVELSPEED/2);
for(t=0;t<9;t++){
dotsMem[t]=0xff;
}
RESTARTLEVEL:
Gobeactive=0;
Sprite[0].type=PACMAN;
Sprite[0].x=64;
Sprite[0].y=3;
Sprite[0].Decalagey=5;
Sprite[0].DirectionV=2;
Sprite[0].DirectionH=2;
Sprite[0].DirectionAnim=0;
Sprite[1].type=FANTOME;
Sprite[1].x=76;
Sprite[1].y=4;
Sprite[1].guber=0;
Sprite[2].type=FANTOME;
Sprite[2].x=75;
Sprite[2].y=5;
Sprite[2].guber=0;
Sprite[3].type=FANTOME;
Sprite[3].x=77;
Sprite[3].y=4;
Sprite[3].guber=0;
Sprite[4].type=FANTOME;
Sprite[4].x=76;
Sprite[4].y=5;
Sprite[4].guber=0;
while(1){
//joystick
if (digitalRead(1)==0) {StartGame(&Sprite[0]);}
if (INGAME) {
if ((analogRead(A0)>=750)&&(analogRead(A0)<950)) {Sprite[0].DirectionV=0;}
else if ((analogRead(A0)>500)&&(analogRead(A0)<750)) {Sprite[0].DirectionV=1;}
if ((analogRead(A3)>=750)&&(analogRead(A3)<950)) {Sprite[0].DirectionH=1;}
else if ((analogRead(A3)>500)&&(analogRead(A3)<750)) {Sprite[0].DirectionH=0;}
//fin joystick
if (TimerGobeactive>1)  {TimerGobeactive--;}else{if (TimerGobeactive==1) {TimerGobeactive=0;Gobeactive=0;}}}
if (Frame<24) {Frame++;}else{Frame=0;}
if (CollisionPac2Caracter(&Sprite[0])==0) {RefreshCaracter(&Sprite[0]);}else{
Sound(100,200);Sound(75,200);Sound(50,200);Sound(25,200);Sound(12,200);delay(400);
if (LIVE>0) {LIVE--;goto RESTARTLEVEL;}else{goto NEWGAME;}}
if (Frame%2==0) {
Tiny_Flip(0,&Sprite[0]);
if (INGAME==1){
for (uint8_t t=0;t<=139;t=t+2){
Sound(pgm_read_byte(&Music[t])-8,(pgm_read_byte(&Music[t+1])-100)); 
}INGAME=2;}
}else{
for(t=0;t<63;t++){
if (checkDotPresent(t)) {break;}else{if (t==62) { for(uint8_t r=0;r<60;r++){Sound(2+r,10);Sound(255-r,20);}delay(1000);goto NEWLEVEL;}}
}}
if ((Gobeactive)&&(Frame%2==0)) {Sound((255-TimerGobeactive),1);}
}}
////////////////////////////////// main end /////////////////////////////////

uint8_t CollisionPac2Caracter(PERSONAGE *Sprite){
uint8_t ReturnCollision=0;
#define xmax(I) (Sprite[I].x+6)
#define xmin(I) (Sprite[I].x)
#define ymax(I) ((Sprite[I].y*8)+Sprite[I].Decalagey+6)
#define ymin(I) ((Sprite[I].y*8)+Sprite[I].Decalagey)
if ((INGAME)) {    
for (uint8_t t=1;t<=4;t++){
if ((xmax(0)<xmin(t))||(xmin(0)>xmax(t))||(ymax(0)<ymin(t))||(ymin(0)>ymax(t))) {}else{ 
if (Gobeactive) {if (Sprite[t].guber!=1) {Sound(20,100);Sound(2,100);}Sprite[t].guber=1;ReturnCollision=0;}else{ if (Sprite[t].guber==1) {ReturnCollision=0;}else{ReturnCollision=1;}}
}}}return ReturnCollision;}

void RefreshCaracter(PERSONAGE *Sprite){
uint8_t memx,memy,memdecalagey;
for (uint8_t t=0;t<=4;t++){
memx=Sprite[t].x;
memy=Sprite[t].y;
memdecalagey=Sprite[t].Decalagey;
if ((Sprite[t].y>-1)&&(Sprite[t].y<8)) {
if ((Frame%2==0)||(t==0)||(LEVELSPEED<=160)) {
if (Sprite[t].DirectionV==1) {Sprite[t].x++;}
if (Sprite[t].DirectionV==0) {
if (t==0) {
if ((Sprite[0].y==3)&&(Sprite[0].x==86)){}else{Sprite[t].x--;}
}else{Sprite[t].x--;}
}}}
if (CheckCollisionWithBack(t,1,Sprite)) {
if (t!=0) {Sprite[t].DirectionV=random()%2;}else{ Sprite[t].DirectionV=2;}
Sprite[t].x=memx;
}
if ((Frame%2==0)||(t==0)||(LEVELSPEED<=160)) {
if (Sprite[t].DirectionH==1) {if (Sprite[t].Decalagey<7) {Sprite[t].Decalagey++;}else{Sprite[t].Decalagey=0;Sprite[t].y++;if (Sprite[t].y==9) {Sprite[t].y=-1;}}}
if (Sprite[t].DirectionH==0) {if (Sprite[t].Decalagey>0) {Sprite[t].Decalagey--;}else{Sprite[t].Decalagey=7;Sprite[t].y--;if (Sprite[t].y==-2) {Sprite[t].y=8;}}}
}
if (CheckCollisionWithBack(t,0,Sprite)) {
if (t!=0) {Sprite[t].DirectionH=random()%2;}else{Sprite[t].DirectionH=2;}
Sprite[t].y=memy;
Sprite[t].Decalagey=memdecalagey;
}
if (t==0) {
if (Frame%2==0) {
if (Sprite[t].DirectionH==1) {Sprite[t].DirectionAnim=0;}
if (Sprite[t].DirectionH==0) {Sprite[t].DirectionAnim=(2*3);} 
if (Sprite[t].DirectionV==1) {Sprite[t].DirectionAnim=(3*3);}
if (Sprite[t].DirectionV==0) {Sprite[t].DirectionAnim=(1*3);}
}}else{
if ((Frame==0)||(Frame==12)) {
Sprite[t].DirectionAnim=0;
if (Sprite[t].DirectionH==1) {Sprite[t].DirectionAnim=0;}
if (Sprite[t].DirectionH==0) {Sprite[t].DirectionAnim=2;}
}}
if (t==0) {
if (Frame%2==0) {
if (Sprite[0].switchanim==0) {
if (Sprite[0].anim<2) {Sprite[0].anim++;}else{Sprite[0].switchanim=1;} 
}else{
if (Sprite[0].anim>0) {Sprite[0].anim--;}else{Sprite[0].switchanim=0;}  
}}}else{
if ((Sprite[t].guber==1)&&(Sprite[t].x>=74)&&(Sprite[t].x<=76)&&(Sprite[t].y>=2)&&(Sprite[t].y<=4)) {Sprite[t].guber=0;}
if  (Frame%2==0) {
if (Sprite[t].anim<1) {Sprite[t].anim++;}else{Sprite[t].anim=0; } 
}}}}

uint8_t CheckCollisionWithBack(uint8_t SpriteCheck,uint8_t HorVcheck,PERSONAGE *Sprite){
uint8_t BacktoComp;
if (HorVcheck==1) {
BacktoComp=RecupeBacktoCompV(SpriteCheck,Sprite); 
}else{
BacktoComp=RecupeBacktoCompH(SpriteCheck,Sprite);}
if ((BacktoComp)!=0) {return 1;}else{return 0;}}

uint8_t RecupeBacktoCompV(uint8_t SpriteCheck,PERSONAGE *Sprite){
uint8_t Y1=0b00000000;
uint8_t Y2=Y1;
#define SpriteWide 6
#define MAXV (Sprite[SpriteCheck].x+SpriteWide)
#define MINV (Sprite[SpriteCheck].x)
if (Sprite[SpriteCheck].DirectionV==1) {
Y1=pgm_read_byte(&back[((Sprite[SpriteCheck].y)*128)+(MAXV)]);
Y2=pgm_read_byte(&back[((Sprite[SpriteCheck].y+1)*128)+(MAXV)]);
}else if (Sprite[SpriteCheck].DirectionV==0) {
Y1=pgm_read_byte(&back[((Sprite[SpriteCheck].y)*128)+(MINV)]);
Y2=pgm_read_byte(&back[((Sprite[SpriteCheck].y+1)*128)+(MINV)]);
}else{Y1=0;Y2=0;}
//decortique
Y1=Trim(0,Y1,Sprite[SpriteCheck].Decalagey);
Y2=Trim(1,Y2,Sprite[SpriteCheck].Decalagey);
if (((Y1)!=0b00000000)||((Y2)!=0b00000000) ) {return 1;}else{return 0;}
}

uint8_t Trim(uint8_t Y1orY2,uint8_t TrimValue,uint8_t Decalage){
uint8_t Comp;
if (Y1orY2==0) {
Comp=0b01111111<<Decalage;
return (TrimValue&Comp);
}else{
Comp=(0b01111111>>(8-Decalage));
return (TrimValue&Comp);
}}

uint8_t ScanHRecupe(uint8_t UporDown,uint8_t Decalage){
if (UporDown==0){
return 0b01111111<<Decalage;}
else{
return 0b01111111>>(8-Decalage);
}}

uint8_t RecupeBacktoCompH(uint8_t SpriteCheck,PERSONAGE *Sprite){
uint8_t TempPGMByte;
if (Sprite[SpriteCheck].DirectionH==0) {
uint8_t RECUPE=(ScanHRecupe(0,Sprite[SpriteCheck].Decalagey));
for(uint8_t t=0;t<=6;t++){
if ((((Sprite[SpriteCheck].y)*128)+(Sprite[SpriteCheck].x+t)>1023)||(((Sprite[SpriteCheck].y)*128)+(Sprite[SpriteCheck].x+t)<0)) {TempPGMByte=0x00;}else{
 TempPGMByte=(pgm_read_byte(&back[((Sprite[SpriteCheck].y)*128)+(Sprite[SpriteCheck].x+t)])); 
}
#define CHECKCOLLISION ((RECUPE)&(TempPGMByte))
if  (CHECKCOLLISION!=0) {return 1;}
}}else if (Sprite[SpriteCheck].DirectionH==1) {
uint8_t tadd=0;
if (Sprite[SpriteCheck].Decalagey>2) { tadd=1;}else{tadd=0;}
uint8_t RECUPE=(ScanHRecupe(tadd,Sprite[SpriteCheck].Decalagey));
for(uint8_t t=0;t<=6;t++){
if (((((Sprite[SpriteCheck].y+tadd)*128)+(Sprite[SpriteCheck].x+t))>1023)||((((Sprite[SpriteCheck].y+tadd)*128)+(Sprite[SpriteCheck].x+t))<0)) {TempPGMByte=0x00;}else{
TempPGMByte=pgm_read_byte(&back[((Sprite[SpriteCheck].y+tadd)*128)+(Sprite[SpriteCheck].x+t)]);
}
#define CHECKCOLLISION2 ((RECUPE)&(TempPGMByte))
if  (CHECKCOLLISION2!=0) {return 1;}
}}return 0;}

void Tiny_Flip(uint8_t render0_picture1,PERSONAGE *Sprite){
uint8_t y,x; 
dotscount=-1;
for (y = 0; y < 8; y++){ 
    SSD1306.ssd1306_send_command(0xb0 + y); // page0 - page1
    SSD1306.ssd1306_send_command(0x00);   // low column start address
    SSD1306.ssd1306_send_command(0x10);   // high column start address
    SSD1306.ssd1306_send_data_start();
for (x = 0; x < 128; x++){
if (render0_picture1==0) {
if (INGAME) {SSD1306.ssd1306_send_byte(background(x,y)|SpriteWrite(x,y,Sprite)|DotsWrite(x,y,Sprite)|LiveWrite(x,y)|FruitWrite(x,y));}else{
SSD1306.ssd1306_send_byte(0xff-(background(x,y)|SpriteWrite(x,y,Sprite)));
}}else if (render0_picture1==1){
SSD1306.ssd1306_send_byte(pgm_read_byte(&back[x+(y*128)]));}}
SSD1306.ssd1306_send_data_stop();
}}

uint8_t FruitWrite(uint8_t x,uint8_t y){
switch(y){
  case 7:if (x<=7) {return pgm_read_byte(&fruits[x]);}break;
  case 6:if ((LEVELSPEED<=190)&&(x<=7)) {return pgm_read_byte(&fruits[x+8]);}break;
  case 5:if ((LEVELSPEED<=180)&&(x<=7)) {return pgm_read_byte(&fruits[x+16]);}break;
  case 4:if ((LEVELSPEED<=170)&&(x<=7)) {return pgm_read_byte(&fruits[x+24]);}break;
}return 0;}

uint8_t LiveWrite(uint8_t x,uint8_t y){
if (y<LIVE) {if (x<=7) {return pgm_read_byte(&caracters[x+(1*8)]);}else{return 0;}
}return 0x00;}

uint8_t DotsWrite(uint8_t x,uint8_t y,PERSONAGE *Sprite){
uint8_t Menreturn=0;
uint8_t mem1=pgm_read_byte(&dots[x+(128*y)]);
if (mem1!=0b00000000) {
dotscount++;
 switch(dotscount){
  case 0:  
  case 1: 
  case 12: 
  case 13: 
  case 50:
  case 51:
  case 62:
  case 63:Menreturn=1;break;
  default:Menreturn=0;break;
}
if (checkDotPresent(dotscount)==0b00000000) {mem1=0x00;}else{
if ((Sprite[0].type==PACMAN)&&((Sprite[0].x<x)&&(Sprite[0].x>x-6))&&(((Sprite[0].y==y)&&(Sprite[0].Decalagey<6))||((Sprite[0].y==y-1)&&(Sprite[0].Decalagey>5)))) {DotsDestroy(dotscount);if (Menreturn==1) {TimerGobeactive=LEVELSPEED;Gobeactive=1;}else{Sound(10,10);Sound(50,10);}}
}}
if (Menreturn==1) {
if (((Frame>=6)&&(Frame<=12))||((Frame>=18)&&(Frame<=24))) {
return 0x00;
}else{return mem1;}
}else{return mem1;}}

uint8_t checkDotPresent(uint8_t  DotsNumber){
uint8_t REST=DotsNumber;
uint8_t DOTBOOLPOSITION=0;
DECREASE:
if (REST>=8) {REST=REST-8;DOTBOOLPOSITION++;goto DECREASE;}
return ((dotsMem[DOTBOOLPOSITION])&(0b10000000>>REST));
}

void DotsDestroy(uint8_t DotsNumber){
uint8_t REST=DotsNumber;
uint8_t DOTBOOLPOSITION=0;
uint8_t SOUSTRAIRE;
DECREASE:
if (REST>=8) {REST=REST-8;DOTBOOLPOSITION=DOTBOOLPOSITION+1;goto DECREASE;}
switch(REST){
  case (0):SOUSTRAIRE=0b01111111;break;
  case (1):SOUSTRAIRE=0b10111111;break;
  case (2):SOUSTRAIRE=0b11011111;break;
  case (3):SOUSTRAIRE=0b11101111;break;
  case (4):SOUSTRAIRE=0b11110111;break;
  case (5):SOUSTRAIRE=0b11111011;break;
  case (6):SOUSTRAIRE=0b11111101;break;
  case (7):SOUSTRAIRE=0b11111110;break;
}
dotsMem[DOTBOOLPOSITION]=dotsMem[DOTBOOLPOSITION]&SOUSTRAIRE;
}

uint8_t SplitSpriteDecalageY(uint8_t decalage,uint8_t Input,uint8_t UPorDOWN){
if (UPorDOWN) {
return Input<<decalage;
}else{
return Input>>(8-decalage); 
}}

uint8_t SpriteWrite(uint8_t x,uint8_t y,PERSONAGE  *Sprite){
uint8_t var1=0;
uint8_t AddBin=0b00000000;
while(1){ 
if (Sprite[var1].y==y) {
AddBin=AddBin|SplitSpriteDecalageY(Sprite[var1].Decalagey,return_if_sprite_present(x,Sprite,var1),1);
}else if (((Sprite[var1].y+1)==y)&&(Sprite[var1].Decalagey!=0)) {
AddBin=AddBin|SplitSpriteDecalageY(Sprite[var1].Decalagey,return_if_sprite_present(x,Sprite,var1),0);
}
var1++;
if (var1==5) {break;}
}return AddBin;}
  
uint8_t return_if_sprite_present(uint8_t x,PERSONAGE  *Sprite,uint8_t SpriteNumber){
uint8_t ADDgobActive;
uint8_t ADDGober;
if  ((x>=Sprite[SpriteNumber].x)&&(x<(Sprite[SpriteNumber].x+8))) { 
if (SpriteNumber!=0) { 
if (Sprite[SpriteNumber].guber==1) {
ADDgobActive=1*(4*8);
ADDGober=Sprite[SpriteNumber].guber*(4*8);
}else{
if ((((Frame>=6)&&(Frame<=12))||((Frame>=18)&&(Frame<=24)))||(TimerGobeactive>GobbingEND)) {ADDgobActive=Gobeactive*(4*8);}else{ADDgobActive=0;}
ADDGober=0;
}}else{
ADDGober=0;
ADDgobActive=0;
}
if ((INGAME==0)&&(SpriteNumber==0)) {  return 0;}     
return pgm_read_byte(&caracters[((x-Sprite[SpriteNumber].x)+(8*(Sprite[SpriteNumber].type*12)))+(Sprite[SpriteNumber].anim*8)+(Sprite[SpriteNumber].DirectionAnim*8)+(ADDgobActive)+(ADDGober)]);
}return 0;}

uint8_t background(uint8_t x,uint8_t y){
return pgm_read_byte(&BackBlitz[((y)*128)+((x))]);
}

void Sound(uint8_t freq,uint8_t dur){
for (uint8_t t=0;t<dur;t++){
if (freq!=0)digitalWrite(4,HIGH); 
for (uint8_t t=0;t<(255-freq);t++){
_delay_us(1);}
digitalWrite(4,LOW);
for (uint8_t t=0;t<(255-freq);t++){
_delay_us(1); }}}
