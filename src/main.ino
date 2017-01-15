// Nguyen Hai Duong
// Jan 5 2017

#include "system.h"

unsigned long time_sys = 0;
unsigned int addr = SYS_OFFSET;

void setup()
{
   System.startUp();
   // Serial.begin(115200);
   char tm[22];
   Time.getTimeString(tm);
   Log.logs("System has took %lu ms for start up, %s", millis(), tm);
   // Network.send("__System start__ %s", tm);
}

void loop()
{

   Time.print();
   System.handleAlarm();

   delay (1000); // Wait one second before repeating :)
}
