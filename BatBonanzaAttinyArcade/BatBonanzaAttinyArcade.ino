
/*  2018 Nov - ATtiny Joypad version
 *   
 *  Modified by Billy Cheung to ajust pins  for OLED and control buttons to suit the Tiny Joypad
 *  original source code and credits to the original author below
 *  
 * 2015 / 2016 /2017
 * Pong game by Andy Jackson - Twitter @andyhighnumber
 * Inspired by http://webboggles.com/ and includes some code from the #AttinyArcade games on that site
 * The code that does not fall under the licenses of sources listed below can be used non commercially with attribution.
 * 
 * When the game is running :
 * LEFT BUTTON - Controls the player's bat
 * RIGHT BUTTON - Press and relese to change mode (difficulty) - Press and hold to toggle sound
 *  
 * Also, from standby....
 *  Press and hold left button to reset the system
 * 
 * If you have problems uploading this sketch, this is probably due to sketch size - you need to update ld.exe in arduino\hardware\tools\avr\avr\bin
 * https://github.com/TCWORLD/ATTinyCore/tree/master/PCREL%20Patch%20for%20GCC
 *
 * This sketch is using the screen control and font functions written by Neven Boyanov for the http://tinusaur.wordpress.com/ project
 * Source code and font files available at: https://bitbucket.org/tinusaur/ssd1306xled
 * **Note that this highly size-optimised version requires modified library functions (which are in this source code file) 
 * and a modified font header
 * 
 * Sleep code is based on this blog post by Matthew Little:
 * http://www.re-innovation.co.uk/web12/index.php/en/blog-75/306-sleep-modes-on-attiny85
 * Comments from Billy Cheung
  I/O for the Tiny Arcad board that this program is originally written for
  B0 - L button 
  B1 - Buzzer
  B2 - R button  
  B3 - OLED SCL
  B4 - OLED SDA
  B5 - Reset

  
  I/O for Tiny Joypad that this version has been modiifed for
  
  B0      OLED SDA
  B1      Right button
  B2      OLED SCL
  B3/A3   up (500-750)  down(750-950) — Landscape mode, down = left button
  B4      Buzzer
  B5/A0   right(500-750) left(750-950) - Landscape mode

 *  Controls:
 *  for Tiny Joypad
 *  At Standby screen 
 *   - Press and hold fire button to reset system  
 *   - Press and hold down button to toogle between single button mode (down),  dual button mode (up & down) to control the bat
 *   - and the single button 2 player mode (player 1 use down button, player 2 use fire button).
 *   
 *  During the game
 *  -  Press and hold left or right button to change speed of the game normal/fast/tough/expert 
 *  In single button mode
 *   -  Fire BUTTON pressed - move player's bat up quickly, 
 *   -  Fire BUTTON released - move player's bat down slowly, 
 *  In dual button mode
 *  -  Up BUTTON - move player's bat up
 *  -  Down BUTTON - move player's bat down
 *  
 *  
 *  RIGHT (Fire) BUTTON - Press and relese to change mode (difficulty) - Press and hold to toggle sound
 *  
 *  Also, from standby....
 *  Press and hold down button to reset the system
*/


// Defines for OLED output
#define SSD1306XLED_H
#define SSD1306_SCL   PORTB2  // SCL=PB2 for Tiny Joypad
#define SSD1306_SDA   PORTB0  // SDA=PB0 for Tiny Joypad
#define SSD1306_SA    0x78  // Slave address
#define soundPin      4
#define upDownPin     A3
#define leftRightPin  A0
#define firePin       1

#include <EEPROM.h>
#include "font6x8AJ.h"
#include <avr/sleep.h>
#include <avr/interrupt.h> // needed for the additional interrupt

#define DIGITAL_WRITE_HIGH(PORT) PORTB |= (1 << PORT)
#define DIGITAL_WRITE_LOW(PORT) PORTB &= ~(1 << PORT)

// Routines to set and clear bits (used in the sleep code)
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))



#define WINSCORE 7

// Function prototypes
void startGame(void);
void drawPlatform(void);
void drawPlatform2(void);
void sendBlock(int);
void playPong(void);
void beep(int,int);
void drawBall(int x, int y);
void blankBall(int x, int y);

void doDrawLS(long, byte);
void doDrawRS(long, byte);
void doNumber (int x, int y, int value);

void ssd1306_init(void);
void ssd1306_xfer_start(void);
void ssd1306_xfer_stop(void);
void ssd1306_send_byte(uint8_t byte);
void ssd1306_send_command(uint8_t command);
void ssd1306_send_data_start(void);
void ssd1306_send_data_stop(void);
void ssd1306_setpos(uint8_t x, uint8_t y);
void ssd1306_fillscreen(uint8_t fill_Data);
void ssd1306_char_f6x8(uint8_t x, uint8_t y, const char ch[]);
void ssd1306_draw_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t bitmap[]);

int player; //0 to 128-platformWidth  - this is the position of the player
int player2; //0 to 128-platformWidth  - this is the position of the player
int lastPlayer;
int lastPlayer2;
int platformWidth = 16; 
boolean stopAnimate = 0; // this is set to 1 when a collision is detected
boolean mute = 0;
boolean newHigh = 0;
int score = 0; // score - this affects the difficulty of the game
int score2 = 0; // score - this affects the difficulty of the game

int ballx = 62*8; // coordinate of the ball
int bally = 50*4; // coordinate of the ball
int vdir = -4; // vertical direction and step  distance
int hdir = -8; // horizontal direction and step distance

int difficulty = 0; 
int gameMode = 0;  // start with single button mode


int perturbation = 0;
int pFactor = 12;

int leftRightReading;
int upDownReading;

// Interrupt handlers
ISR(PCINT0_vect){ // PB0 pin button interrupt           
}

void playerIncPong(){ // PB2 pin button interrupt
}

// Arduino stuff - setup
void setup() {
// for Tiny Joypad
  DDRB = 0b00010000;    // set PB4 "5th bit from the right" as output (for the speaker)
  PCMSK = 0b0000010; // pin change mask: listen to portb b1 Fire button
  GIMSK |= 0b00100000;  // enable PCINT interrupt }       // enable all interrupts
}

// Arduino stuff - loop
void loop() { 
  ssd1306_init();
  ssd1306_fillscreen(0x00);
  
  // The lower case character set is seriously compromised because I've had to truncate the ASCII table
  // to release space for executable code - hence lower case y and w are remapped to h and / respectively.
  // There is no z in the table (or h!) as these aren't used anywhere in the text here and most of the 
  // symbols are also missing for the same reason (see my hacked version of font6x8.h - font6x8AJ.h for more detail)
  ssd1306_char_f6x8(0, 1, "   ---------------  ");
  ssd1306_char_f6x8(0, 2, "        B A T       ");
  ssd1306_char_f6x8(0, 4, "    B O N A N Z A   ");
  ssd1306_char_f6x8(0, 5, "   ---------------  ");
  ssd1306_char_f6x8(0, 7, "   bh andh jackson  "); // see comments above !

    long startT = millis();
    long nowT =0;
    boolean sChange = 0;

   // read systems settings from eeporm
    mute = EEPROM.read(0);
    difficulty = EEPROM.read(1);
    gameMode = EEPROM.read(2);

    if (mute != 0 && mute != 1) {
      mute = 0;
      EEPROM.write(0,0);
    }


    if (difficulty < 1 || difficulty > 4) {
      difficulty = 1;
      EEPROM.write(1,1);
    }

    if (gameMode < 0 || gameMode > 2) {
     gameMode = 0;
     EEPROM.write(2,gameMode);
    }

    
    
    // hold fire Button at startup to reset systems settings
    while(digitalRead(firePin) == LOW) {
      nowT = millis();
      if (nowT - startT > 2000) {
        sChange = 1;     
        EEPROM.write(0,0);
        EEPROM.write(1,1);
        EEPROM.write(2,0);
        ssd1306_char_f6x8(16, 0, "- SYSTEM RESET -");  
        break;
      }
      if (sChange == 1) break;
    }  
    while(digitalRead(firePin) == LOW);

    // hold down or down button to toggle Single or Dual button mode
    while(analogRead(upDownPin) < 950) {
      nowT = millis();
      if (nowT - startT > 2000) {
        sChange = 1;  
        gameMode = (gameMode +1) % 3; 
        EEPROM.write(2,gameMode);
        if (gameMode == 0) 
          ssd1306_char_f6x8(16, 0, "- ONE BUTTON  -");
        else if (gameMode == 1) 
          ssd1306_char_f6x8(16, 0, "- DUAL BUTTON -"); 
        else if (gameMode == 2) 
          ssd1306_char_f6x8(16, 0, "- 2 PLAYERS   -"); 
        break;
      }
     if (sChange == 1) break;
     }  
    while(analogRead(upDownPin) < 950);

        if (gameMode == 0) 
          ssd1306_char_f6x8(16, 0, "- ONE BUTTON  -");
        else if (gameMode == 1) 
          ssd1306_char_f6x8(16, 0, "- DUAL BUTTON -"); 
        else if (gameMode == 2) 
          ssd1306_char_f6x8(16, 0, "- 2 PLAYERS   -"); 

 
    if (sChange != 1) {
    delay(3000);
    ssd1306_init();
    ssd1306_fillscreen(0x00);
    stopAnimate = 0;
    score = 0;
    score2 = 0;

    playPong(); 
    }
    
    delay(3500);
 
  system_sleep();
}

void doNumber (int x, int y, int value) {
    char temp[10] = {0,0,0,0,0,0,0,0,0,0};
    itoa(value,temp,10);
    ssd1306_char_f6x8(x, y, temp);
}

void ssd1306_init(void){
  DDRB |= (1 << SSD1306_SDA); // Set port as output
  DDRB |= (1 << SSD1306_SCL); // Set port as output

  ssd1306_send_command(0xAE); // display off
  ssd1306_send_command(0x00); // Set Memory Addressing Mode
  ssd1306_send_command(0x10); // 00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
  ssd1306_send_command(0x40); // Set Page Start Address for Page Addressing Mode,0-7
  ssd1306_send_command(0x81); // Set COM Output Scan Direction
  ssd1306_send_command(0xCF); // ---set low column address
  ssd1306_send_command(0xA1); // ---set high column address
  ssd1306_send_command(0xC8); // --set start line address
  ssd1306_send_command(0xA6); // --set contrast control register
  ssd1306_send_command(0xA8);
  ssd1306_send_command(0x3F); // --set segment re-map 0 to 127
  ssd1306_send_command(0xD3); // --set normal display
  ssd1306_send_command(0x00); // --set multiplex ratio(1 to 64)
  ssd1306_send_command(0xD5); // 
  ssd1306_send_command(0x80); // 0xa4,Output follows RAM content;0xa5,Output ignores RAM content
  ssd1306_send_command(0xD9); // -set display offset
  ssd1306_send_command(0xF1); // -not offset
  ssd1306_send_command(0xDA); // --set display clock divide ratio/oscillator frequency
  ssd1306_send_command(0x12); // --set divide ratio
  ssd1306_send_command(0xDB); // --set pre-charge period
  ssd1306_send_command(0x40); // 
  ssd1306_send_command(0x20); // --set com pins hardware configuration
  ssd1306_send_command(0x02);
  ssd1306_send_command(0x8D); // --set vcomh
  ssd1306_send_command(0x14); // 0x20,0.77xVcc
  ssd1306_send_command(0xA4); // --set DC-DC enable
  ssd1306_send_command(0xA6); // 
  ssd1306_send_command(0xAF); // --turn on oled panel 
}

void ssd1306_xfer_start(void){
  DIGITAL_WRITE_HIGH(SSD1306_SCL);  // Set to HIGH
  DIGITAL_WRITE_HIGH(SSD1306_SDA);  // Set to HIGH
  DIGITAL_WRITE_LOW(SSD1306_SDA);   // Set to LOW
  DIGITAL_WRITE_LOW(SSD1306_SCL);   // Set to LOW
}

void ssd1306_xfer_stop(void){
  DIGITAL_WRITE_LOW(SSD1306_SCL);   // Set to LOW
  DIGITAL_WRITE_LOW(SSD1306_SDA);   // Set to LOW
  DIGITAL_WRITE_HIGH(SSD1306_SCL);  // Set to HIGH
  DIGITAL_WRITE_HIGH(SSD1306_SDA);  // Set to HIGH
}

void ssd1306_send_byte(uint8_t byte){
  uint8_t i;
  for(i=0; i<8; i++)
  {
    if((byte << i) & 0x80)
      DIGITAL_WRITE_HIGH(SSD1306_SDA);
    else
      DIGITAL_WRITE_LOW(SSD1306_SDA);
    
    DIGITAL_WRITE_HIGH(SSD1306_SCL);
    DIGITAL_WRITE_LOW(SSD1306_SCL);
  }
  DIGITAL_WRITE_HIGH(SSD1306_SDA);
  DIGITAL_WRITE_HIGH(SSD1306_SCL);
  DIGITAL_WRITE_LOW(SSD1306_SCL);
}

void ssd1306_send_command(uint8_t command){
  ssd1306_xfer_start();
  ssd1306_send_byte(SSD1306_SA);  // Slave address, SA0=0
  ssd1306_send_byte(0x00);  // write command
  ssd1306_send_byte(command);
  ssd1306_xfer_stop();
}

void ssd1306_send_data_start(void){
  ssd1306_xfer_start();
  ssd1306_send_byte(SSD1306_SA);
  ssd1306_send_byte(0x40);  //write data
}

void ssd1306_send_data_stop(void){
  ssd1306_xfer_stop();
}

void ssd1306_setpos(uint8_t x, uint8_t y)
{
  if (y>7) return;
  ssd1306_xfer_start();
  ssd1306_send_byte(SSD1306_SA);  //Slave address,SA0=0
  ssd1306_send_byte(0x00);  //write command

  ssd1306_send_byte(0xb0+y);
  ssd1306_send_byte(((x&0xf0)>>4)|0x10); // |0x10
  ssd1306_send_byte((x&0x0f)|0x01); // |0x01

  ssd1306_xfer_stop();
}

void ssd1306_fillscreen(uint8_t fill_Data){
  uint8_t m,n;
  for(m=0;m<8;m++)
  {
    ssd1306_send_command(0xb0+m); //page0-page1
    ssd1306_send_command(0x00);   //low column start address
    ssd1306_send_command(0x10);   //high column start address
    ssd1306_send_data_start();
    for(n=0;n<128;n++)
    {
      ssd1306_send_byte(fill_Data);
    }
    ssd1306_send_data_stop();
  }
}

void ssd1306_char_f6x8(uint8_t x, uint8_t y, const char ch[]){
  uint8_t c,i,j=0;
  while(ch[j] != '\0')
  {
    c = ch[j] - 32;
    if (c >0) c = c - 12;
    if (c >15) c = c - 6;
    if (c>40) c=c-6;
    if(x>126)
    {
      x=0;
      y++;
    }
    ssd1306_setpos(x,y);
    ssd1306_send_data_start();
    for(i=0;i<6;i++)
    {
      ssd1306_send_byte(pgm_read_byte(&ssd1306xled_font6x8[c*6+i]));
    }
    ssd1306_send_data_stop();
    x += 6;
    j++;
  }
}

void system_sleep() {
  ssd1306_fillscreen(0x00);
  ssd1306_send_command(0xAE);
  cbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter OFF
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  sleep_enable();
  sleep_mode();                        // System actually sleeps here
  sleep_disable();                     // System continues execution here when watchdog timed out 
  sbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter ON  
  ssd1306_send_command(0xAF);
}

void beep(int bCount,int bDelay){
  if (mute) return;
  for (int i = 0; i<=bCount; i++){digitalWrite(soundPin,HIGH);for(int i2=0; i2<bDelay; i2++){__asm__("nop\n\t");}digitalWrite(soundPin,LOW);for(int i2=0; i2<bDelay; i2++){__asm__("nop\n\t");}}
}



/* ------------------------
 *  Pong Code
 */
void playPong() {
  ballx = 64*8;
  bally = 32*4;
  hdir = -8;
  vdir = -4;
  int actualy, actualx;
  int factor = 0;
  int waitCount = 0;

  int lastx=64*8, lasty=32*4;

  player=64;
  player2=64;
  lastPlayer = 64;
  lastPlayer2 = 64;
  score = 0; // obvious
  score2 = 0; // obvious
  perturbation = 0;
    
  startGame();
  while (stopAnimate == 0) {
    while(1) {
    waitCount++;

    if(analogRead(leftRightPin) < 950){
      boolean sChange = 0;
      long startT = millis();
      long nowT =0;
      while(analogRead(leftRightPin) < 950){
        nowT = millis();
        if (nowT - startT > 1500) { // long press to toggle sound on/off
          sChange = 1;
          if (mute == 0) { mute = 1; ssd1306_char_f6x8(32, 0, "-- MUTE --"); } else { mute = 0; ssd1306_char_f6x8(23, 0, "-- SOUND ON --"); }
          break;
        }
      }
      while(analogRead(leftRightPin) < 950);

      if (sChange == 1) { // long press to toggle sound on/off
           //short press to set level of  difficulty level
          } else if (difficulty == 1) {  
            difficulty = 2; 
            pFactor = 12;
            ssd1306_char_f6x8(20, 0, "-- FAST   --");  
          } else if (difficulty == 2) { 
            difficulty = 3; 
            pFactor = 11;
            ssd1306_char_f6x8(20, 0, "-- TOUGH  --");  
          } else if (difficulty == 3) { 
            difficulty = 4; 
            pFactor = 10;
            ssd1306_char_f6x8(16, 0, "-- EXPERT --");  
          } else if (difficulty == 4) { 
            difficulty = 1; 
            pFactor = 12;
            ssd1306_char_f6x8(32, 0, "-- NORMAL --"); 
          }
      
      if (sChange == 0) delay(1000);
      ssd1306_fillscreen(0x00);        
      EEPROM.write(0,mute);
      EEPROM.write(1,difficulty);
    }
    
  upDownReading = analogRead(upDownPin);
  
  if (gameMode == 0) { // one button mode
    //use only fire button  to control bat 
    if (digitalRead(firePin)==LOW)
          player-=2;     
    else  player+=1;     
  }
  else if (gameMode == 1) { // dual button mode

    //Up button 
    if (upDownReading> 500 && upDownReading <750) 
      player-=2;
    
    //Down button 
    if (upDownReading> 750 && upDownReading <950) 
      player+= 2;
   
  }
   else if (gameMode == 2) { // two player mode
    // down button for player 1
    if (upDownReading> 750 && upDownReading <950) 
         player-=2;
    else player+=1;
   }
 

    if (player > 48) player = 48;
    if (player <0) player = 0;
    
    // move player2 bat
    if (gameMode == 2) { // 2 player mode, fire button for player 2
       if (digitalRead(firePin)==LOW)
             player2 -=2;     
       else  player2 +=1;     
    }
    else { // single player mode, computer controls palyer 2
      if(waitCount >= 3) {
        waitCount = 0;
        perturbation = perturbation - 2 + random(0,5);
        if (perturbation > pFactor) perturbation = pFactor - 2;
        if (perturbation < pFactor*-1) perturbation = (pFactor*-1)+2;
      }
      player2 = (bally/4 -8)+perturbation;
    }
      
    if (player2 > 48) player2 = 48;
    if (player2 <0) player2 = 0;

    actualy = floor(bally/4);
    actualx = floor(ballx/8);
    
    // bounce off the sides of the screen
    if ((actualy+vdir<63&&vdir>01) || (actualy- vdir>6&&vdir<0)){
      bally+=vdir;
    }else {
        vdir = vdir*-1;
    }
    ballx+=hdir;

    actualy = floor(bally/4);
    actualx = floor(ballx/8);
    
    // check it hits the left pad and deal with bounces and misses
    if (actualx <= 4) {
      if(actualy<player-1||actualy>player+platformWidth+1){ 
        score2++;
    
        ballx = 5*8;
        bally = player*4;

        hdir = 13;
        if (vdir > 0) {
          vdir = 2;
        } else vdir = -2;
        
        ssd1306_fillscreen(0x00);        
        doNumber(46,4,score);
        doNumber(78,4,score2);        
        if (score2 < WINSCORE) {
          for (int i = 0; i<1000; i = i+ 100){
            beep(50,i);
          }
          for (int incr=0;incr<3;incr++) {
              ssd1306_send_data_stop();
              ssd1306_setpos(78,4);
              ssd1306_send_data_start();
              sendBlock(0);
              sendBlock(0);
              ssd1306_send_data_stop();
              delay(350);
              doNumber(78,4,score2);
              delay(350);
            }
            startGame();
        }
        perturbation = 0;
        break;
      }else if (actualy<player+1){        
        vdir = -6;
        hdir = 7;
      }else if (actualy<player+4){        
        vdir = -4;
        hdir = 10;
      }else if (actualy<player+7){        
        vdir = -2;
        hdir = 13;
      }else if (actualy<player+9){        
        vdir = 0;
        hdir = 14;
      }else if (actualy<player+12){        
        vdir = 2;
        hdir = 13;
      }else if (actualy<player+15){        
        vdir = 4;
        hdir = 10;
      }else {   
        vdir = 6;
        hdir = 7;
      }
      beep(20,600);
    }  
    
    // check it hits the right pad and deal with bounces
    if(actualx >= 122) {
      if(actualy<player2-1||actualy>player2+platformWidth+1){
        score++;
  
        ballx = 120*8;
        bally = player2*4;

        hdir = -13;
        if (vdir > 0) {
          vdir = 2;
        } else vdir = -2;

        ssd1306_fillscreen(0x00);        
        doNumber(46,4,score);
        doNumber(78,4,score2);
        if (score < WINSCORE) {
        for (int i = 0; i<1000; i = i+ 100){
            beep(50,i);
          }                
  
          for (int incr=0;incr<3;incr++) {
              ssd1306_setpos(46,4);
              ssd1306_send_data_start();
              sendBlock(0);
              sendBlock(0);
              ssd1306_send_data_stop();
              delay(350);
              doNumber(46,4,score);
              delay(350);
            }
            perturbation = 0;
            startGame();
        }        
        break;
      }else if (actualy<player2+1){        
        vdir = -6;
        hdir = -7;
      }else if (actualy<player2+4){        
        vdir = -4;
        hdir = -10;
      }else if (actualy<player2+7){        
        vdir = -2;
        hdir = -13;
      }else if (actualy<player2+9){        
        vdir = 0;
        hdir = -14;
      }else if (actualy<player2+12){        
        vdir = 2;
        hdir = -13;
      }else if (actualy<player2+15){        
        vdir = 4;
        hdir = -10;
      }else {   
        vdir = 6;
        hdir = -7;
      }
      beep(20,300);
    }      

    
    if ( difficulty == 3 || difficulty == 4) {
      factor = 10-floor((score-score2)/2); // expert modes
      if (factor < 2) factor = 2;
    } 
    else if (difficulty == 2) {
      factor = 20-floor((score-score2)/2); // normal modes
      if (factor < 10) factor = 10;
    }
    else { // difficulty == 1
      factor = 30-floor((score-score2)/2); // normal modes
      if (factor < 15) factor = 15;
    }
    
    delay(factor);
    
    // draw ball
    blankBall(floor(lastx/8),floor(lasty/4));  
    drawPlatform();
    drawPlatform2();
    drawBall(floor(ballx/8),floor(bally/4));
    lastx = ballx;
    lasty = bally;
    
    doNumber(28,0,score);
    doNumber(92,0,score2);
    if (score == WINSCORE || score2 == WINSCORE) {
      stopAnimate = 1;
      break;
    }
    }
 }

blankBall(floor(lastx/8),floor(lasty/4));  
blankBall(floor(ballx/8),floor(bally/4));  

if (score > score2) {
  ssd1306_char_f6x8(27, 3, "P L A Y E R 1"); 
} else {
  ssd1306_char_f6x8(27, 3, "P L A Y E R 2"); 
}
ssd1306_char_f6x8(27, 4, "             ");
ssd1306_char_f6x8(27, 5, "   W I N S   ");

for (int i = 0; i<1000; i = i+ 50){
  beep(50,i);
}


for (int incr=0;incr<6;incr++) {
    ssd1306_setpos(28,0);
    ssd1306_send_data_start();
    sendBlock(0);
    sendBlock(0);
    ssd1306_send_data_stop();
    ssd1306_setpos(92,0);
    ssd1306_send_data_start();
    sendBlock(0);
    sendBlock(0);
    ssd1306_send_data_stop();
    delay(350);
    doNumber(28,0,score);
    doNumber(92,0,score2);
    delay(350);
  }
}

void drawPlatform() {
  if (player != lastPlayer) {
    ssd1306_setpos(0,lastPlayer/8);
    ssd1306_send_data_start();
    ssd1306_send_byte(B00000000);
    ssd1306_send_data_stop();
    ssd1306_setpos(0,lastPlayer/8+1);
    ssd1306_send_data_start();
    ssd1306_send_byte(B00000000);
    ssd1306_send_data_stop(); 
    ssd1306_setpos(0,lastPlayer/8+2);
    ssd1306_send_data_start();
    ssd1306_send_byte(B00000000);
    ssd1306_send_data_stop(); 
  }  

  if (player%8!=0){
    ssd1306_setpos(0,player/8);
    ssd1306_send_data_start();
    ssd1306_send_byte((B11111111)<<player%8);
    ssd1306_send_data_stop();
    ssd1306_setpos(0,player/8+1);
    ssd1306_send_data_start();
    ssd1306_send_byte(B11111111);
    ssd1306_send_data_stop();    
    ssd1306_setpos(0,player/8+2);
    ssd1306_send_data_start();
    ssd1306_send_byte((B01111110)>>8-player%8);
    ssd1306_send_data_stop();
  } else {
    ssd1306_setpos(0,player/8);
    ssd1306_send_data_start();
    ssd1306_send_byte(B11111111);
    ssd1306_send_data_stop();
    ssd1306_setpos(0,player/8+1);
    ssd1306_send_data_start();
    ssd1306_send_byte(B11111111);
    ssd1306_send_data_stop();

  }
  lastPlayer = player;
}

void drawPlatform2() {

  if (player2 != lastPlayer2) {
    ssd1306_setpos(127,lastPlayer2/8);
    ssd1306_send_data_start();
    ssd1306_send_byte(B00000000);
    ssd1306_send_data_stop();
    ssd1306_setpos(127,lastPlayer2/8+1);
    ssd1306_send_data_start();
    ssd1306_send_byte(B00000000);
    ssd1306_send_data_stop(); 
    ssd1306_setpos(127,lastPlayer2/8+2);
    ssd1306_send_data_start();
    ssd1306_send_byte(B00000000);
    ssd1306_send_data_stop(); 
  }
  
  if (player2%8!=0){
    ssd1306_setpos(127,player2/8);
    ssd1306_send_data_start();
    ssd1306_send_byte((B11111111)<<player2%8);
    ssd1306_send_data_stop();
    ssd1306_setpos(127,player2/8+1);
    ssd1306_send_data_start();
    ssd1306_send_byte(B11111111);
    ssd1306_send_data_stop();        
    ssd1306_setpos(127,player2/8+2);
    ssd1306_send_data_start();
    ssd1306_send_byte((B01111110)>>8-player2%8);
    ssd1306_send_data_stop();
  } else {
    ssd1306_setpos(127,player2/8);
    ssd1306_send_data_start();
    ssd1306_send_byte((B11111111)<<0);
    ssd1306_send_data_stop();
    ssd1306_setpos(127,player2/8+1);
    ssd1306_send_data_start();
    ssd1306_send_byte((B11111111)<<0);
    ssd1306_send_data_stop();
  }
  lastPlayer2 = player2;
}

void sendBlock(int fill){
 ssd1306_send_byte(B00000000);
 ssd1306_send_byte(B00000000);
 ssd1306_send_byte(B00000000);
 ssd1306_send_byte(B00000000);
 ssd1306_send_byte(B00000000);
 ssd1306_send_byte(B00000000);
 ssd1306_send_byte(B00000000);
 ssd1306_send_byte(B00000000);
}

void blankBall(int x, int y) {
  if (y%8!=0){
    ssd1306_setpos(x,y/8);
    ssd1306_send_data_start();
    ssd1306_send_byte(B00000000);
    ssd1306_send_byte(B00000000);
    ssd1306_send_data_stop();
    
    ssd1306_setpos(x,y/8+1);
    ssd1306_send_data_start();
    ssd1306_send_byte(B00000000);
    ssd1306_send_byte(B00000000);
    ssd1306_send_data_stop();
  } else {
    ssd1306_setpos(x,y/8);
    ssd1306_send_data_start();
    ssd1306_send_byte(B00000000);
    ssd1306_send_byte(B00000000);
    ssd1306_send_data_stop();
  }
}


void drawBall(int x, int y) {
  if (y%8!=0){
    ssd1306_setpos(x,y/8);
    ssd1306_send_data_start();
    doDrawLS(0,y%8);
    ssd1306_send_data_stop();
    
    ssd1306_setpos(x,y/8+1);
    ssd1306_send_data_start();
    doDrawRS(0,8-y%8);
    ssd1306_send_data_stop();
  } else {
    ssd1306_setpos(x,y/8);
    ssd1306_send_data_start();
    doDrawLS(0,0);
    ssd1306_send_data_stop();
  }
}

void doDrawRS(long P1, byte P2) {
  ssd1306_send_byte((B00000011 | P1)>>P2);
  ssd1306_send_byte((B00000011 | P1)>>P2);
}
void doDrawLS(long P1, byte P2) {
  ssd1306_send_byte((B00000011 | P1)<<P2);
  ssd1306_send_byte((B00000011 | P1)<<P2);
}

void startGame(void) {
  
    ssd1306_fillscreen(0x00);

    ssd1306_char_f6x8(16, 3, "-- GET READY --");
    doNumber(60,5,3);
    delay(1000);
    doNumber(60,5,2);
    delay(1000);
    doNumber(60,5,1);
    delay(1000);
    ssd1306_fillscreen(0x00);

    for (int i = 800; i>200; i = i - 200){
    beep(30,i);
    }

}
