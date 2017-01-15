
#ifndef DISPLAY_H
#define DISPLAY_H

#include "led-7seg.h"

class SystemDisplay : public Led7seg {
public :
   SystemDisplay(const unsigned char latch, const unsigned char clk, const unsigned char data) : Led7seg(latch, clk, data) {};
   ~SystemDisplay();
   bool begin();
   bool numberIndividual(unsigned char led, unsigned char n);
   bool number(int n);
   bool character(unsigned char led, char c);
   bool string(const char *s);

};


extern SystemDisplay Display;

#endif
