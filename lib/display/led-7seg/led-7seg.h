#ifndef LED_7_SEG_H
#define LED_7_SEG_H

// MCU -> 74HC595N -> dual LED 7segs
// References:
// https://www.arduino.cc/en/tutorial/ShiftOut
// https://en.wikipedia.org/wiki/Seven-segment_display
// http://www.ti.com/lit/ds/symlink/sn74hc595.pdf

#include <Arduino.h>
#include "config.h"
//This is the hex value of each number stored in an array by index num
// value display :   0     1     2     3     4     5     6     7     8     9     A  b     C    d    E    F
const unsigned char digit[16]= {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};

class Led7seg {

public:
   Led7seg(const unsigned char latch, const unsigned char clk, const unsigned char data);
   ~Led7seg();
   bool display(unsigned char led, unsigned char n);
   bool displayDual(int n);
   bool displayCharacter(unsigned char led, char c);
   bool displayDualCharacter(const char *s);
private:
   unsigned char latch_;
   unsigned char clk_;
   unsigned char data_;
};

#endif
