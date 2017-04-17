#include "display.h"

SystemDisplay7segs::~SystemDisplay7segs() {}

bool SystemDisplay7segs::numberIndividual(unsigned char led, unsigned char n) {
   return display(led, n);
}

bool SystemDisplay7segs::number(int n) {
   return displayDual(n);
}

bool SystemDisplay7segs::character(unsigned char led, char c) {
   return displayCharacter(led, c);
}

bool SystemDisplay7segs::string(const char *s) {
   return displayDualCharacter(s);
}

SystemDisplayI2C_Arduino_LCD::~SystemDisplayI2C_Arduino_LCD() {}

void SystemDisplayI2C_Arduino_LCD::begin() {
   #if defined(ESP8266)
   Wire.begin(ESP8266_SDA, ESP8266_SCL);
   #else
   Wire.begin();
   #endif
}

void SystemDisplayI2C_Arduino_LCD::print(const char *s, char x, char y) {
   Wire.beginTransmission(addr_);
   Wire.write(x);
   Wire.write(y);
   Wire.write(s, strlen(s));
   Wire.write('*');
   Wire.endTransmission();
   delay(50);                 // wait for lcd receive data and print
}

void SystemDisplayI2C_Arduino_LCD::print(int num, char x, char y) {
   char str[15];
   sprintf(str, "%d", num);
   print(str, x, y);
}

SystemDisplayI2C_LCD::~SystemDisplayI2C_LCD() {}

void SystemDisplayI2C_LCD::print(const char *s, char x, char y) {
   LiquidCrystal_I2C::setCursor(x, y);
   LiquidCrystal_I2C::print(s);
}

void SystemDisplayI2C_LCD::print(int num, char x, char y) {
   LiquidCrystal_I2C::setCursor(x, y);
   LiquidCrystal_I2C::print(num);
}

SystemDisplay::~SystemDisplay() {};

void SystemDisplay::displayTime() {
   char tm[22];
   Time.getHourMinuteString(tm);
   DisplayLCD.print(tm, 11, 1);
   // Log.verboses("Alarm 2 is on %s", tm);
   // Network.send("Alarm 2 is on %s", tm);
   // Time.setNextSchedule(ALARM_2);
}

void SystemDisplay::displayOnlyLamptStatus(bool state) {
   char str[15];
   if(state==ON) {
      DisplayLCD.print("LAMP ON ",0,0);
   } else {
      DisplayLCD.print("LAMP OFF",0,0);
   }
}

void SystemDisplay::displayLamptStatus(bool state) {
   char str[15];
   if(state==ON) {
      sprintf(str, "LAMP ON  %dh-%dh", schedule_lamp[0], schedule_lamp[1]);
   } else {
      sprintf(str, "LAMP OFF %dh-%dh", schedule_lamp[1], schedule_lamp[0]);
   }
   DisplayLCD.print(str,0,0);
}

void SystemDisplay::displayTempt(int tempt) {
   char str[7];
   sprintf(str, "%d oC", tempt);
   DisplayLCD.print(str,0,1);
   Log.verboses("Tempt tank %d (C)", tempt);
   // Network.send("Tempt tank %d (C)", tempt);
}

// SystemDisplay7segs DisplaySegs(LATCH, CLK, DATA);
// SystemDisplayI2C_Arduino_LCD DisplayLCD(I2C_ARDUINO_LCD_ADDR);
SystemDisplay DisplayLCD(I2C_LCD_ADDR);
