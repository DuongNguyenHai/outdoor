
#ifndef DISPLAY_H
#define DISPLAY_H

#include <Wire.h>
#include "LiquidCrystal_I2C.h"
#include "led-7seg/led-7seg.h"
#include "realtime.h"
// I2C pin
#define ESP8266_SDA 2         // esp8266 ver 7, ver 12
#define ESP8266_SCL 14        // esp8266 ver 7, ver 12

class SystemDisplay7segs : public Led7seg {
public :
   SystemDisplay7segs(const unsigned char latch, const unsigned char clk, const unsigned char data) : Led7seg(latch, clk, data) {};
   ~SystemDisplay7segs();
   bool begin();
   bool numberIndividual(unsigned char led, unsigned char n);
   bool number(int n);
   bool character(unsigned char led, char c);
   bool string(const char *s);

};

class SystemDisplayI2C_Arduino_LCD {

public:
   SystemDisplayI2C_Arduino_LCD(uint16_t addr): addr_(addr) {};
   ~SystemDisplayI2C_Arduino_LCD();
   void begin();
   void print(const char *s, char x, char y);
   void print(int num, char x, char y);
private:
   uint16_t addr_;
};

class SystemDisplayI2C_LCD : public LiquidCrystal_I2C {

public:
   SystemDisplayI2C_LCD(uint16_t addr): LiquidCrystal_I2C(addr, 16, 2) {};
   ~SystemDisplayI2C_LCD();
   void print(const char *s, char x, char y);
   void print(int num, char x, char y);

};

class SystemDisplay : public SystemDisplayI2C_LCD {
public:
   SystemDisplay(uint16_t addr): SystemDisplayI2C_LCD(addr) {};
   ~SystemDisplay();
   void displayTime();
   void displayOnlyLamptStatus(bool state);
   void displayLamptStatus(bool state);
   void displayTempt(int tempt);
};

// extern SystemDisplay Display;
extern SystemDisplay DisplayLCD;

#endif
