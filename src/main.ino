// Nguyen Hai Duong
// Jan 5 2017

#include "config.h"
#include <ESP8266WiFi.h>
#include <realtime.h>
#include <log.h>


void setup()
{
   // Setup Serial connection
   Serial.begin(115200);
   // Log.printf("Connecting to %s ", ssid);
   // WiFi.begin(ssid, password);
   //
   // //  trying to connect to wifi (120 times = 1 minute)
   // for (size_t i = 0; i < 120; i++) {
   //    if (WiFi.status() != WL_CONNECTED) {
   //       delay(500);
   //       Log.printf(".");
   //    } else {
   //       Log.printf(" connected\n");
   //       Time.setOnline();  // getting tim from internet and set system time
   //       break;
   //    }
   //    if(i==120-1) {
   //       Log.error("Cant connect to internet. Time will be setted to default.\n");
   //       Time.setDefault();   // system time will be setted to default value 0:0:0 jan 1 2017
   //    }
   // }
   Time.setRTC(1483793990);
   Time.setNextSchedule(ALARM_1);
   Time.setNextSchedule(ALARM_2);
   Serial.println("set success!");
}

void loop()
{
   Time.print();
   if(Time.isAlarmOn()) {
      if(Time.WhatsAlarmOn(1)) {
         Serial.println("Alarm 1 is on !");
      } else if(Time.WhatsAlarmOn(2)) {
         Serial.println("Alarm 2 is on !");
      }
   }
   delay (1000); // Wait one second before repeating :)
}
