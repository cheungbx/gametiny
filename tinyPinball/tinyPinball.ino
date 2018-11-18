//          >>>>>  T-I-N-Y  P-I-N-B-A-L-L for ATTINY85  GPL v3 <<<<
//                  Programmer: Daniel Champagne 2018
//                 Contact EMAIL: phoenixbozo@gmail.com
//           https://sites.google.com/view/arduino-collection

//  Tiny PINBALL is free software: you can redistribute it and/or modify
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
#include <math.h>

//<<<<<<<<<<<<<<<<<<<VARIABLE PUBLIQUE>>>>>>>>>>>>>>>>>
#define SSD64x32 0 //1 pour 64 x 32 : 0 pour 128 x 64
#define SPRINGLONG 72
uint8_t SCANLINE=0;
uint8_t totalBall=4;
uint8_t totalpush=0;
uint8_t FIRSTTIME=0;
uint16_t bouncePush=0; 
uint8_t frameCount=0; 
uint8_t SpringBar=SPRINGLONG;
uint8_t MemExa[3][3]={{0}};
int8_t trigFlipG=0;
int8_t trigFlipD=0;
typedef struct BALL{
float x=0;
float y=0;
float Speedx=0;
float Speedy=0;
float SIMx=0;
float SIMy=0;
float SIMSpeedx=0;
float SIMSpeedy=0;
float DecelX=0;
float DecelY=0;
uint8_t resetBall=0;
uint8_t grid[2][3]={{0}};
}BALL;
//<<<<<<<<<<<<<<<<<<<FIN VARIABLE PUBLIQUE>>>>>>>>>>>>>>>>>

//<<<<<<<<<<<<<<<<<<<SETUP>>>>>>>>>>>>>>>>>
      void setup() {
_delay_ms(40);
SSD1306.ssd1306_init();
_delay_ms(40);
      pinMode(A3,INPUT);
      pinMode(1,INPUT);//flipdroite
 //     pinMode(3,INPUT);//flipgauche
      pinMode(4,OUTPUT);//sound
      SSD1306.ssd1306_fillscreen(0x00);
      }
//<<<<<<<<<<<<<<<<<<<end setup>>>>>>>>>>>>>>>>>


//<<<<<<<<<<<<<<<<<<<MAIN>>>>>>>>>>>>>>>>>
void loop() {
digitalWrite(4,LOW);
NEWGAME:
totalpush=0;
totalBall=5;
SSD1306.ssd1306_fillscreen(0xff);
delay(500);
if (SSD64x32==0) {SSD1306.ssd1306_draw_bmp(31, 2, 96, 6, intro);}else{
SSD1306.ssd1306_draw_bmp(31, 4, 96, 8, intro);}
for (uint8_t t=255;t>0;t--){
Sound(t,2);}
for (uint8_t t=0;t<255;t++){
Sound(t,2);}
delay(1000);
while(1){
random(255);
if (((analogRead(A3)>=750)&&(analogRead(A3)<850))||((digitalRead(1)==LOW))||((analogRead(A3)>500)&&(analogRead(A3)<750))) {
SSD1306.ssd1306_fillscreen(0x00);
if (SSD64x32==0) {SSD1306.ssd1306_draw_bmp(27+32, 3, 37+32, 5, READY);}else{
SSD1306.ssd1306_draw_bmp(27+32, 5, 37+32, 7, READY);
}
FIRSTTIME=1;
delay(1000);
break;}}
start:
if (totalBall!=0) {totalBall--;}else{
delay(300);
if (SSD64x32==0) {SSD1306.ssd1306_draw_bmp(22+32, 2, 41+32, 6, GameOver);}else{
SSD1306.ssd1306_draw_bmp(22+32, 4, 41+32, 8, GameOver);}

 for(uint8_t t=0;t<5;t++){
  Sound (100,100);
  Sound (1,100);}
delay(1000);goto NEWGAME;}
#define XSTART 46.0
#define YSTART 30.0;
#define XSTARTSPEED 0
#define YSTARTSPEED 0
BALL ball;
ball.resetBall=0;
ball.x=XSTART;
ball.y=YSTART;
ball.Speedx=XSTARTSPEED ;
ball.Speedy=YSTARTSPEED;
ball.SIMx=XSTART;
ball.SIMy=YSTART;
ball.SIMSpeedx=XSTARTSPEED ;
ball.SIMSpeedy=YSTARTSPEED ;

while(1){
BallupDate(&ball);
if (SSD64x32) {Tiny_Flip(&ball);}else{Tiny_Flip2(&ball);delay(3);if (SCANLINE<1) {SCANLINE++;}else{SCANLINE=0;}}
if (ball.x<.5) {falseBall();FIRSTTIME=1;goto start;}
if ((ball.y>=29)&&(ball.x>=18)) {if (((analogRead(A3)>=750)&&(analogRead(A3)<850))||(digitalRead(1)==LOW)) {if (SpringBar>54) {SpringBar=SpringBar-2;}}else{if ((SpringBar>SPRINGLONG-10)&&(SpringBar<SPRINGLONG)) {SpringBar=SPRINGLONG;} if (SpringBar<SPRINGLONG) {SpringBar=SpringBar+8;}}}else{SpringBar=SPRINGLONG;}
if ((analogRead(A3)>500)&&(analogRead(A3)<750)) {if (trigFlipG<3) {trigFlipG++;}}else{if (trigFlipG>0) {trigFlipG--;}}
if (((analogRead(A3)>=750)&&(analogRead(A3)<850))||(digitalRead(1)==LOW)) {if (trigFlipD<3) {trigFlipD++;}}else{if (trigFlipD>0) {trigFlipD--;}}
}}
//<<<<<<<<<<<<<<<<<<<MAIN FIN>>>>>>>>>>>>>>>

void falseBall(void){
uint8_t t;
for (t=50;t>0;t--){
Sound(t,6);}}

void Sound(uint8_t freq,uint8_t dur){
for (uint8_t t=0;t<dur;t++){
digitalWrite(4,HIGH); 
for (uint8_t t=0;t<(255-freq);t++){
_delay_us(1);}
digitalWrite(4,LOW);
for (uint8_t t=0;t<(255-freq);t++){
_delay_us(1); }
}}

void BallupDate(BALL *B){
SimulateMove(B);
if (ColisionCheck(B->SIMx,B->SIMy,B)) {
CheckColisionType(B);
if ((B->y<7)||(B->y>24)||(B->x<31)||(B->x>48)){
WriteMove(B);
if (B->y<29) {Sound(1,6);}
}else{
WriteMoveBounce(B);
bouncePush=256;
if (totalpush<7) {totalpush++;}else{
 if (totalBall<4) {totalBall++;}
 totalpush=0;
for(uint8_t ttt=60;ttt<240;ttt=ttt+20){Sound(ttt,6);}
}}
if ((B->SIMSpeedy>=0.15)) {B->SIMSpeedy=(B->SIMSpeedy-.1);} 
if ((B->SIMSpeedy<=-0.15)) {B->SIMSpeedy=(B->SIMSpeedy+.1);}
}else{WriteMove(B);}
if ((B->y>29)&&(B->x>22)) {B->y=30;}
if ((round(B->x)>=49)&&(round(B->y)==30)) {
B->x=50; 
B->y=29; 
B->Speedx=0.8-(random(10.0)/100.0);  
B->Speedy=-1.0+(random(30.0)/100.0);  
}}

uint8_t SelectByte(uint8_t ByteSelect,uint8_t FFx0){
uint8_t ByteRecup=0;
switch (ByteSelect){
  case(0):ByteRecup=0b10000000;break;
  case(1):ByteRecup=0b01000000;break;
  case(2):ByteRecup=0b00100000;break;
  case(3):ByteRecup=0b00010000;break;
  case(4):ByteRecup=0b00001000;break;
  case(5):ByteRecup=0b00000100;break;
  case(6):ByteRecup=0b00000010;break;
  case(7):ByteRecup=0b00000001;break;
  default:ByteRecup=0;break;
}
if ((ByteRecup&FFx0)!=0b00000000) {return 1;}else{return 0;}
}


void TrimXY(BALL *B){
if (B->Speedx>1) B->Speedx=1;
if (B->Speedx<-1) B->Speedx=-1;
if (B->Speedy>1) B->Speedy=1;
if (B->Speedy<-1) B->Speedy=-1;}

void SimulateRebounce(uint8_t Sim,BALL *B){
B->SIMx=B->x;
B->SIMy=B->y;
switch(Sim){
  case (0):B->SIMSpeedx=B->Speedx;B->SIMSpeedy=B->Speedy;break;
  case (1):B->SIMSpeedx=-B->Speedx;B->SIMSpeedy=B->Speedy;break;
  case (2):B->SIMSpeedx=B->Speedx;B->SIMSpeedy=-B->Speedy;break;
  case (3):B->SIMSpeedx=-B->Speedx;B->SIMSpeedy=-B->Speedy;break;
  case (4):B->SIMSpeedx=-B->Speedy;B->SIMSpeedy=-B->Speedx;break;
  case (5):B->SIMx=B->x+1;B->SIMy=B->y;B->SIMSpeedx=-0.2;B->SIMSpeedy=0.2;break;
  case (6):B->SIMx=B->x+1;B->SIMy=B->y;B->SIMSpeedx=-0.2;B->SIMSpeedy=-0.2;break;
  default:break;}}

uint8_t CheckColisionType(BALL *B){
TrimXY(B);
B->SIMx=B->x;
B->SIMy=B->y;
B->SIMSpeedx=B->Speedx;
B->SIMSpeedy=B->Speedy;
if ((B->SIMy==30)&&(B->SIMx<=(79-32))){ 
if (!ColisionCheck(B->SIMx,B->SIMy,B)){
SimulateMove(B);
return 0;}else{
TrimBallOnSpring(B);
if (B->SIMSpeedx<=0) {B->SIMSpeedx=-B->SIMSpeedx;}
SimulateMove(B);
return 0;}}
uint8_t sim=0;
while(1){                       
SimulateRebounce(sim,B);          
SimulateMove(B);
if (!ColisionCheck(B->SIMx,B->SIMy,B)) {return 0;}
sim++;if (sim==7) {sim=0;}
}}

void TrimBallOnSpring(BALL *B){
uint8_t counter=0;
if ((B->SIMx+32)<(SpringBar-1)) {
DAS:
if ((B->SIMx+32)<(SpringBar-1)) { B->SIMx++;counter++;goto DAS;}
if (counter>4) {
B->SIMSpeedx=2;
}}}

void WriteMove(BALL *B){ 
B->x=B->SIMx;
B->y=B->SIMy;
B->Speedx=B->SIMSpeedx;
B->Speedy=B->SIMSpeedy;}

void WriteMoveBounce(BALL *B){ 
B->x=B->SIMx;
B->y=B->SIMy;
B->SIMSpeedx=B->SIMSpeedx*8;
B->SIMSpeedy=B->SIMSpeedy*8;
if (B->SIMSpeedx>1.4) B->SIMSpeedx=1.4;
if (B->SIMSpeedx<-1.4) B->SIMSpeedx=-1.4;
if (B->SIMSpeedy>1.4) B->SIMSpeedy=1.4;
if (B->SIMSpeedy<-1.4) B->SIMSpeedy=-1.4;
B->Speedx=B->SIMSpeedx;
B->Speedy=B->SIMSpeedy;}

void SimulateMove(BALL *B){
TrimXY(B);
if (B->SIMx>=0) {
if ((B->SIMSpeedx>-1)) {if (B->SIMSpeedx-0.05>-1) {B->SIMSpeedx=B->SIMSpeedx-0.05;}}
B->SIMx=B->SIMx+B->SIMSpeedx;}else{B->resetBall=1;}
B->SIMy=B->SIMy+B->SIMSpeedy;}

uint8_t ColisionCheck(float x,float y,BALL *B){
TrimXY(B);
uint16_t serialcount=0;
uint8_t X=round(x);
uint8_t Y=round(y);
uint8_t verticalStrip=((Y)/8); //0 to 3
serialcount=((verticalStrip*64)+X);
#define BACK (pgm_read_byte(&start[serialcount])&PixelAsign(Y))!=0b00000000)
if ((X>=4)&&(X<=9)&&(Y>=11)&&(Y<=14)){
if (trigFlipG==0) {
if ((((pgm_read_byte(&FLIPGAUCHE[(X-4)+(trigFlipG*6)]))&(PixelAsign(Y)))!=0b00000000)||(BACK) {return 1;}else{return 0;} 
}else if ((((pgm_read_byte(&FLIPDETGAUCHE[(X-4)]))&(PixelAsign(Y)))!=0b00000000)||(BACK){if (trigFlipG!=3) {B->SIMSpeedx=2;
Sound(20,12);
}else{B->SIMSpeedx=1;Sound(1,6);}return 0;}
} else if ((X>=4)&&(X<=9)&&(Y>=17)&&(Y<=20)){
if (trigFlipD==0) {
if ((((pgm_read_byte(&FLIPDROITE[(X-4)+(trigFlipD*6)]))&(PixelAsign(Y)))!=0b00000000)||(BACK) {return 1;}else{return 0;} 
}else if ((((pgm_read_byte(&FLIPDETDROIT[(X-4)]))&(PixelAsign(Y)))!=0b00000000)||(BACK){if (trigFlipD!=3) {B->SIMSpeedx=2;
Sound(20,12);
}else{B->SIMSpeedx=1;Sound(1,6);}return 0;}
}else{
if (((X+32)<(SpringBar))&&(B->y==30)) {return 1;}
if ((pgm_read_byte(&start[serialcount])&PixelAsign(Y))!=0b00000000) {return 1;}
return 0;}return 0x00;}

uint8_t RecupeScreen(uint8_t nn,uint8_t mm){
uint8_t ScreenCount=nn-90;  
switch(mm){
case(4): return pgm_read_byte(&ScreenBallA[ScreenCount+(totalBall*6)]);break;
case(5): return pgm_read_byte(&ScreenBallB[ScreenCount+(totalBall*6)]);break;
case(6): return pgm_read_byte(&PusherA[ScreenCount+(totalpush*6)]);break;
case(7): return pgm_read_byte(&PusherB[ScreenCount+(totalpush*6)]);break;
}return 0x00;}

////////////////////////////////SCREEN FLIP PROCEDURE/////////////////////////////////////////////
void Tiny_Flip(BALL *B){
uint8_t m,n;
  for (m = 0; m < 8; m++)
  {
    SSD1306.ssd1306_send_command(0xb0 + m); // page0 - page1
    SSD1306.ssd1306_send_command(0x00);   // low column start address
    SSD1306.ssd1306_send_command(0x10);   // high column start address
    SSD1306.ssd1306_send_data_start();
    for (n = 0; n < 128; n++)
    {     
    if ((n>31)&&(n<97)&&(m>3)) {
      if (n<90) {
      SSD1306.ssd1306_send_byte(addBin(PixelConvert(n,m,B),RecupeByte(n,m),RecupeFlip(n,m),RecupeSpring(n,m)));
        }else{
       SSD1306.ssd1306_send_byte(RecupeScreen(n,m));
      }
      }else{SSD1306.ssd1306_send_byte(0x00);}
    
    }
    SSD1306.ssd1306_send_data_stop();
  }
  if (bouncePush==256) {
    frameCount++;
  if (frameCount>1) {
    Sound(1,20);
Sound(20,20);
Sound(1,20);
bouncePush=0;frameCount=0;}
}}

uint8_t addBin(uint8_t a,uint8_t b,uint8_t c,uint8_t d){
return a|b|c|d;}

uint8_t RecupeByte(uint8_t x,uint8_t y){
return pgm_read_byte(&start [((x-32)+(((y)-4)*64))+bouncePush]);}

uint8_t RecupeFlip(uint8_t x,uint8_t y){
uint8_t TRIGG=0;
uint8_t TRIDD=0;
if (trigFlipG>2) {TRIGG=2;}else{TRIGG=trigFlipG;}
if (trigFlipD>2) {TRIDD=2;}else{TRIDD=trigFlipD;}
if ((y==5)&&(x>35&&x<42)){
return pgm_read_byte(&FLIPGAUCHE[(x-36)+(TRIGG*6)]);
}else if ((y==6)&&(x>35&&x<42)) {
return pgm_read_byte(&FLIPDROITE[(x-36)+(TRIDD*6)]);}else{
return 0x00;}}

uint8_t RecupeSpring(uint8_t x,uint8_t y){
if (((y==7)&&(x>53&&x<SpringBar))){
return 0b01000000;}else{
return 0x00;}
}

uint8_t PixelAsign(uint8_t Value){
while(Value>=8){
Value=Value-8;}
switch(Value){
case  (0):return 0b00000001;break;
case  (1):return 0b00000010;break;
case  (2):return 0b00000100;break;
case  (3):return 0b00001000;break;
case  (4):return 0b00010000;break;
case  (5):return 0b00100000;break;
case  (6):return 0b01000000;break;
case  (7):return 0b10000000;break;
default:return 0x00;break;
}}

uint8_t PixelConvert(uint8_t horiz,uint8_t verti,BALL *B){
uint8_t ballx (round(B->x)+32);
uint8_t bally (round(B->y)+32);
if ((horiz>31)&&(horiz<97)&&(verti>3)) {
if ((trunc((bally)/8)==verti)&&(ballx==horiz)) {return PixelAsign(bally);}else{return 0x00;}
}else{return 0x00;}}

uint8_t SliceByte(uint8_t Vertical,uint8_t Byte){
uint8_t toto=0;
 if ((Vertical%2)!=0)  {
 if ((Byte&0b10000000)!=0) {toto=toto|0b11000000;}
 if ((Byte&0b01000000)!=0) {toto=toto|0b00110000;}
 if ((Byte&0b00100000)!=0) {toto=toto|0b00001100;}
 if ((Byte&0b00010000)!=0) {toto=toto|0b00000011;}
 } else {
 if ((Byte&0b00001000)!=0) {toto=toto|0b11000000;}
 if ((Byte&0b00000100)!=0) {toto=toto|0b00110000;}
 if ((Byte&0b00000010)!=0) {toto=toto|0b00001100;}
 if ((Byte&0b00000001)!=0) {toto=toto|0b00000011;}
  }return toto;}

void Tiny_Flip2(BALL *B){
uint8_t m,x,y,n,mem,Min,Max;
if ((B->y)<10.66) {Min=0;Max=3;}
else if ((B->y)>21.66) {Min=4;Max=7;}
else {Min=2;Max=5;}
  if (FIRSTTIME==1) {Min=0;Max=7;}
  for (y = 0; y <= 7; y++)
  {  
    SSD1306.ssd1306_send_command(0xb0 + y);
    SSD1306.ssd1306_send_command(0x00);  
    SSD1306.ssd1306_send_command(0x10); 
    SSD1306.ssd1306_send_data_start();
    m=(trunc(y/2))+4;
    for (x = 0; x < 64; x++){
if (((y>=Min)&&(y<=Max))||(x<12)) {
      n=(x)+32;   
      if ((n<90)) {
       mem=SliceByte(y,addBin(PixelConvert(n,m,B),RecupeByte(n,m),RecupeFlip(n,m),RecupeSpring(n,m)));
       SSD1306.ssd1306_send_byte(mem);
       SSD1306.ssd1306_send_byte(mem);
}else{    
 mem=SliceByte(y,RecupeScreen(n,m));
 SSD1306.ssd1306_send_byte(mem);
 SSD1306.ssd1306_send_byte(mem);}}}
SSD1306.ssd1306_send_data_stop();}
if (bouncePush==256) {
frameCount++;
if (frameCount>1) {
Sound(1,20);Sound(20,20);Sound(1,20);
bouncePush=0;frameCount=0;}}
FIRSTTIME=0;}
