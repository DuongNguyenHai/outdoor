#include "realtime.h"

#define DEFAULT_TIME 1483228800           // 00:00:00 jan 1 2017
#define RCT_WRONG 947979985            // it will be used to indicate that reading from RTC has a problem

SystemTime::SystemTime() {
   if (!begin()) {
     Log.error("Couldn't find RTC");
   }
   pinMode(ALARM_INTER, INPUT);
   track_measure = 0;
   track_lamp = 0;
}

SystemTime::~SystemTime() {}

int SystemTime::setRTC(time_t t) {
   adjust(DateTime(t));
   time_t t2 = getTime();
   if(t2<=t+TIME_DIFF)
      return RT_SUSS;
   else
      return RT_FAIL;
}

int SystemTime::setDefault() {
   return setRTC(DEFAULT_TIME);
}

int SystemTime::setOnline() {

   time_t timeOnl = getTimeOnline();
   time_t timeSys = getTime();
   if(timeOnl==0 && timeSys==0) {         // cant got time from internet and system time has not setted before
      Log.error("Set default time and date\n");
      return setDefault();
   }
   else if(timeOnl==0 && timeSys!=0) return 0; // cant got time from internet
   else if ( timeSys==0 || timeOnl<=timeSys-TIME_DIFF || timeOnl>=timeSys+TIME_DIFF) {
      Log.log("Time and date will be updated online\n");
      // Setting time and date
      return setRTC(timeOnl);
   }

   return RT_SUSS;
}

time_t SystemTime::getTime() {
   if(lostPower())
      return 0;
   time_t t = now().unixtime();
   if (t==RCT_WRONG) {      // RTC device read error
      Log.error("RTC device read error !\n");
      return 0;
   }
   return t;
}

void SystemTime::print() {
   DateTime tnow = now();
   if (tnow.unixtime()!=RCT_WRONG)
      printDateTime(tnow);
   else
      Log.error("RTC device read error !\n");
}

time_t SystemTime::getTimeOnline() {

   WiFiClient client;
   // trying to connect google.con in 13 times
   for (size_t i = 0; i < 13; i++) {
      if (!client.connect("google.com", 80)) {
         Log.log("connection to google.com failed, retrying...\n");
      } else break;
      if(i==13-1) {
         Log.error("Cant got time from google\n");
         return 0;
      }
   }

   client.print(String("GET /") + " HTTP/1.1\r\n" +
              "Connection: close\r\n" +
              "\r\n");

   while (client.connected()) {
      if (client.available()) {
         if (client.read() == '\n') {
            if (client.read() == 'D') {
               if (client.read() == 'a') {
                  if (client.read() == 't') {
                     if (client.read() == 'e') {
                        if (client.read() == ':') {
                          client.read();
                          String dateTime = client.readStringUntil('\r');
                          client.stop();
                          // Log.log(dateTime);
                          return convertToDateTime(dateTime);
                        }
                     }
                  }
               }
            }
         }
      }
   }

   return 0;

}

time_t SystemTime::convertToDateTime(String str) {

   tmElements_t tm;
   String dw("SunMonTueWedThuFriSat");
   String mt("JanFebMarAprMayJunJulAugSepOctNovDec");

   String sub = str.substring(0,3); // day of week
   tm.Wday = dw.indexOf(sub.c_str())/3;

   sub = str.substring(5,7);        // day
   tm.Day = sub.toInt();

   sub = str.substring(8,11);       // month (short string)
   tm.Month = mt.indexOf(sub.c_str())/3;

   sub = str.substring(12,16);      // year
   tm.Year = CalendarYrToTm(sub.toInt());

   sub = str.substring(17,19);      // hour
   tm.Hour = sub.toInt();

   sub = str.substring(20,22);      // minute
   tm.Minute = sub.toInt();

   sub = str.substring(23,25);      // second
   tm.Second = sub.toInt();

   // printDateTime(tm);                           // print UTC time

   return makeTime(tm) + GMT_VIETNAM*3600 + 1;  // return GMT time, plus 1 to compensate elapsed calculation time

}

void SystemTime::printDateTime(tmElements_t tm) {
   Log.printf("Time = %d:%d:%d, Date (D/M/Y) = %d/%d/%d \n", tm.Hour, tm.Minute, tm.Second, tm.Day, tm.Month, tmYearToCalendar(tm.Year));
}

void SystemTime::printDateTime(DateTime tm) {
   Log.printf("Time = %d:%d:%d, Date (D/M/Y) = %d/%d/%d \n", tm.hour(), tm.minute(), tm.second(), tm.day(), tm.month(), tm.year());
}

bool SystemTime::isAlarmOn() {
   if(! digitalRead(ALARM_INTER))
      return true;
   else
      return false;
}

bool SystemTime::WhatsAlarmOn(unsigned char alarm) {
   if(! digitalRead(ALARM_INTER)) {
      if(isFlagSet(alarm)) {
         clearFlag(alarm);
         return true;
      }
      return false;
   }
   return false;
}

int SystemTime::setNextSchedule(unsigned char alarm) {

   int ret;
   // set next time for alarm 1
   if(alarm==ALARM_1) {
      ret = setAlarmHour(alarm, schedule_measure[track_measure]);
      track_measure = (track_measure + 1) % SCHEDULE_MEASURE_MAX;
   }
   // set next time for alarm 2
   else {
      ret = setAlarmHour(alarm, schedule_lamp[track_lamp]);
      track_lamp = (track_lamp + 1) % SCHEDULE_LAMP_MAX;
   }

   if(ret)
      return RT_SUSS;
   else
      return RT_FAIL;
}

// create a global Time
SystemTime Time;
