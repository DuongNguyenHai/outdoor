#include "led-7seg.h"

Led7seg::Led7seg(const unsigned char latch, const unsigned char clk, const unsigned char data) {
   latch_ = latch;
   clk_ = clk;
   data_ = data;
   pinMode(latch_, OUTPUT);
   pinMode(clk_, OUTPUT);
   pinMode(data_, OUTPUT);
}

Led7seg::~Led7seg() {}

bool Led7seg::display(unsigned char led, unsigned char n) {
   if(n<0 || n>15)
      return false;
   digitalWrite(latch_, LOW);
   if(led==1) {
      shiftOut(data_, clk_, MSBFIRST, ~digit[0]);
      shiftOut(data_, clk_, MSBFIRST, ~digit[n]); // digit on LED 1
   } else
   if(led==2) {
      shiftOut(data_, clk_, MSBFIRST, ~digit[n]);
      shiftOut(data_, clk_, MSBFIRST, ~digit[0]); // digit on LED 1
   }
   digitalWrite(latch_, HIGH);

   return true;
}


bool Led7seg::displayDual(int n) {
   if(n<0 || n>99)
      return false;
   unsigned char tens = n/10;
   unsigned char units = n%10;
   digitalWrite(latch_, LOW);
   shiftOut(data_, clk_, MSBFIRST, ~digit[units]); // digit on LED 2. if use wanna more LED, just add more shiftOut function.
   shiftOut(data_, clk_, MSBFIRST, ~digit[tens]); // digit on LED 1
   digitalWrite(latch_, HIGH);
   return true;
}

bool Led7seg::displayCharacter(unsigned char led, char c) {
   if(c>90)
      c = c -32;
   return display(led, c - 55);
}

bool Led7seg::displayDualCharacter(const char *s) {
   char c1 = s[0];
   char c2 = s[1];

   if(s[0]>90)
      c1 = c1 - 32;
   if(c2>90)
      c2 = c2 - 32;

   digitalWrite(latch_, LOW);
   shiftOut(data_, clk_, MSBFIRST, ~digit[c2-55]); // digit on LED 2. if use wanna more LED, just add more shiftOut function.
   shiftOut(data_, clk_, MSBFIRST, ~digit[c1-55]); // digit on LED 1
   digitalWrite(latch_, HIGH);
   return true;
}
