                

/*  2018 Nov - ATtiny Joypad Testing
 *   
 *  Created by  Billy Cheung 
 *  Small program to test buttons, sounds and OLED of Attiny Joypad
 *  
  
  I/O for Tiny Joypad are defined as follows
  
  B0      OLED SDA
  B1      Fire button
  B2      OLED SCL
  B3/A3   up (500-750)  down(750-950) — Landscape mode
  B4      Buzzer
  B5/A0   right(500-750) left(750-950) - Landscape mode

 
*/
#include <font6x8.h>
#include <font8x16.h>
#include <ssd1306xled.h>
#include <ssd1306xled8x16.h>

// ============================================================================

// #include "img0_128x64c1.h"
// #include "img1_128x64c1.h"

// SSD1306Device SSD1306;

void beep(int bCount,int bDelay){

  for (int i = 0; i<=bCount; i++){digitalWrite(4,HIGH);for(int i2=0; i2<bDelay; i2++){__asm__("nop\n\t");}digitalWrite(4,LOW);for(int i2=0; i2<bDelay; i2++){__asm__("nop\n\t");}}
}


void setup() {
    uint8_t p = 0xff;
  // put your setup code here, to run once:
  _delay_ms(40);
  SSD1306.ssd1306_init();

  for (uint8_t i = 0; i < 4; i++)
  {
    p = (p >> i);
    SSD1306.ssd1306_fillscreen(~p);
    _delay_ms(100);
  }
 SSD1306.ssd1306_fillscreen(0x00);
}
byte sound = LOW;
void loop() {
  // put your main code here, to run repeatedly:

char text[] = "          ";
String text1;


  SSD1306.ssd1306_setpos(0, 0);
  SSD1306.ssd1306_string_font6x8("ATtiny85 Test");

  SSD1306.ssd1306_setpos(0, 2);
  SSD1306.ssd1306_string_font6x8("PB1:");
  text1 = String (digitalRead (1));
  text1.toCharArray(text, 5);
  SSD1306.ssd1306_setpos(60,2);
  SSD1306.ssd1306_string_font6x8("     ");
  SSD1306.ssd1306_setpos(60,2);
  SSD1306.ssd1306_string_font6x8(text);
  
  SSD1306.ssd1306_setpos(0, 3);
  SSD1306.ssd1306_string_font6x8(" A0:");
  text1 = String (analogRead (A0));
  text1.toCharArray(text, 5);
  SSD1306.ssd1306_setpos(60, 3);
  SSD1306.ssd1306_string_font6x8("     ");
  SSD1306.ssd1306_setpos(60, 3);
  SSD1306.ssd1306_string_font6x8(text);
  
  SSD1306.ssd1306_setpos(0, 4);
  SSD1306.ssd1306_string_font6x8(" A3:");
  text1 = String (analogRead (A3));
  text1.toCharArray(text, 5);
  SSD1306.ssd1306_setpos(60, 4);
  SSD1306.ssd1306_string_font6x8("     ");
  SSD1306.ssd1306_setpos(60, 4);
  SSD1306.ssd1306_string_font6x8(text);


  SSD1306.ssd1306_setpos(0, 6);
  SSD1306.ssd1306_string_font6x8("Press to Beep");

  if (digitalRead(1)==LOW) { beep(30,400); beep(30,300);  beep(30,100); };

  if (analogRead(A0)<950)  { beep(30,200); beep(30,300);  beep(30,400); };

  if (analogRead(A3)<950)  { beep(30,300); beep(30,400);  beep(30,200); };
     
  _delay_ms(50);
}
