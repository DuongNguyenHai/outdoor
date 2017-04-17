// Nguyen Hai Duong
// Jan 5 2017
// setup system in offline mode. Its mean system will run without inter
#include "system.h"

void setup()
{
   // Folowing all of steps below
   // Step 1: start serial and wire
   Serial.begin(BAUD_RATE);                  // begin serial
   Wire.begin(ESP8266_SDA, ESP8266_SCL);     // begin I2C

   // Step 2: setup display monitor
   System.setupDisplay();
   DisplayLCD.print("System start",0,0);

   // Step 3: setup time
   System.setupSystemTime(OFFLINE); // if RTC had lost power, system will set default time to RTC
   // Time.set(DateTime(2017, 03, 10, 22, 24, 40).unixtime()); // setup time

   // Step 4: setup lamps.
   System.setupLamps();

   // Show information to display monitor
   char tm[20];
   Time.getDateTimeString(tm);
   Log.logs("System has took %lu ms for start up, %s", millis(), tm);
   System.displayInfor2LCD();
}

void loop()
{
   // Time.print();
   System.handleAlarm();
   // delay(1000); // Wait one second before repeating :)
}
