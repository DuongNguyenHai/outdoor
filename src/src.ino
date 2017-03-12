// Nguyen Hai Duong
// Jan 5 2017

#include "system.h"

void setup()
{

   // System.startUp();
   // Folowing all of steps below or use the only line code above
   // Step 1: start serial and wire
   Serial.begin(BAUD_RATE);                  // begin serial
   Wire.begin(ESP8266_SDA, ESP8266_SCL);     // begin I2C

   // Step 2: setup display monitor
   System.setupDisplay();
   DisplayLCD.print("System start",0,0);

   // Step 3: read config from system.txt file
   System.showHostFile();     // show all files in system
   System.config();

   // Step 4: setup network. Trying to connect to wifi. If has internet connection will get time from google
   // and if not, it will set system time offline
   int ret = System.setupNetwork();
   bool online = (ret != RT_FAIL) ? ONLINE : OFFLINE;
   System.setupSystemTime(online);

   // System.setupSystemTime(ONLINE);
   // Time.set(DateTime(2017, 03, 10, 22, 24, 40).unixtime()); // setup time

   // Step 5: setup lamps.
   System.setupLamps();

   // Step 6: setup server: access the web page at http://esp8266fs.local
   System.setupHost();

   // Show information to display monitor
   char tm[20];
   Time.getDateTimeString(tm);
   Log.logs("System has took %lu ms for start up, %s", millis(), tm);

   System.displayInfor2LCD();

   // Network.send("__System start__ %s", tm);
}

void loop()
{
   // Time.print();
   System.handleClient();
   System.handleAlarm();
   // delay(1000); // Wait one second before repeating :)
}
