// Nguyen Hai Duong
// Jan 5 2017

#include "system.h"

unsigned long time_sys = 0;
unsigned int addr = SYS_OFFSET;

// SystemDisplay DisplayLCD(LATCH, CLK, DATA);
SystemDisplayI2C_LCD DisplayLCD(LCD_ADDR);

void setup()
{
   // System.startUp();
   Serial.begin(BAUD_RATE);                  // begin serial
   Wire.begin(ESP8266_SDA, ESP8266_SCL);     // begin I2C
   // Time.begin();     // begin I2C
   // eeprom.begin();    // begin I2C

   int rt_net = System.setupNetwork();
   bool online = (rt_net != RT_FAIL) ? true : false;

   // Time.set(DateTime(2017, 01, 19, 11, 59, 50).unixtime());
   System.setupSystemTime(online);
   System.setupLamps();

   char tm[22];
   Time.getTimeString(tm);
   Log.logs("System has took %lu ms for start up, %s", millis(), tm);
   Time.print();

   System.displayInfor2LCD();
   // Network.send("__System start__ %s", tm);
}

void loop()
{
   // Time.print();
   System.handleAlarm();
   delay(1000); // Wait one second before repeating :)
}
