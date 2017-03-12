#include "realtime.h"

#define DEFAULT_TIME 1483228800           // 00:00:00 jan 1 2017

SystemTime::SystemTime() {
   pinMode(PIN_ALARM, INPUT);
   track_measure_ = 0;
   track_lamp_ = 0;
}

SystemTime::~SystemTime() {}

int SystemTime::set(time_t t) {
   adjust(DateTime(t));
   return RT_SUSS;
}

int SystemTime::setDefault() {
   return set(DEFAULT_TIME);
}

int SystemTime::setOffline() {
   if(lostPower())
      return set(DEFAULT_TIME);
   else
      return RT_SUSS;
}

int SystemTime::setOnline() {

   time_t timeOnl = getTimeOnline();
   time_t timeSys = getTime();

   if(timeSys==RCT_WRONG)  {               // error reading time from RTC
      Log.errors("read RTC !");
      return RT_FAIL;
   }
   if(timeOnl==0 && timeSys==0) {         // cant got time from internet and system time has not set before
      Log.logs("RTC date-time will be set to default.");
      return setDefault();
   }
   else if(timeOnl==0 && timeSys!=0) return 0; // cant got time from internet
   else if ( timeSys==0 || timeOnl<=timeSys-TIME_DIFF || timeOnl>=timeSys+TIME_DIFF) {
      Log.logs("RTC date-time will be updated online.");
      // Setting time and date
      return set(timeOnl);
   }
   Log.logs("RTC date-time dont need to be adjusted.");
   return RT_MISS;
}

time_t SystemTime::get() {
   return now().unixtime();
}

time_t SystemTime::getTime() {
   if(lostPower())
      return 0;
   time_t tnow = get();
   if(tnow==RCT_WRONG)
      Log.errors("RTC device read error !");

   return tnow;
}

unsigned char SystemTime::getTimeHour() {
   return DateTime(getTime()).hour();
}

void SystemTime::print() {
   char tm[9];
   getTimeString(tm);
   Log.printfs(tm);
}

void SystemTime::printDate() {
   char tm[10];
   getDateString(tm);
   Log.printfs(tm);
}

void SystemTime::printDateTime() {
   char tm[22];
   getDateTimeString(tm);
   Log.printfs(tm);
}

time_t SystemTime::getTimeOnline() {

   WiFiClient client;
   // trying to connect google.con in 13 times
   for (size_t i = 0; i < 13; i++) {
      if (client.connect("google.com", 80))
         break;
      if(i==13-1) {
         Log.errors("Got time from google failed");
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
                          // Log.logs(dateTime);
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

bool SystemTime::getHourMinuteString(char *s) {
   DateTime tm = now();
   if(tm.unixtime()==RCT_WRONG)
      return false;
   snprintf (s, 10, "%s%dh%s%d", ((tm.hour() > 9) ? "": " "), tm.hour(), ((tm.minute() > 9) ? "": "0"), tm.minute());
   return true;
}

bool SystemTime::isAlarmOn() {
   if(! digitalRead(PIN_ALARM))
      return true;
   else
      return false;
}

bool SystemTime::whichAlarmOn(unsigned char alarm) {
   if(! digitalRead(PIN_ALARM)) {
      if(isAlarmRinging(alarm)) {
         clearAlarm(alarm);
         return true;
      }
      return false;
   }
   return false;
}

bool SystemTime::initAlarm(unsigned char alarm) {

   // set first time for alarm 2
   if(alarm==ALARM_MEASURE) {
      for (unsigned char i = 0; i < SCHEDULE_MEASURE_MAX; i++) {
         if(schedule_measure[i]!=0) {
            if(schedule_measure[i]>getTimeHour()) {
               track_measure_ = i;
               break;
            }
         }
      }
      Log.verboses("Set time for alarm %d: %uh", alarm, schedule_measure[track_measure_]);
      // Because we use alarm 2 for repeating in minute. so dont use the code below
      // return setAlarmHour(alarm, schedule_measure[track_measure_]);
      return true;

   }
   // set first time for alarm 1
   else {
      for (unsigned char i = 0; i < SCHEDULE_LAMP_MAX; i++) {
         if(schedule_lamp[i]!=0) {
            if(schedule_lamp[i]>getTimeHour()) {
               track_lamp_ = i;
               break;
            }
         }
      }
      Log.verboses("Set time for alarm %d: %uh", alarm, schedule_lamp[track_lamp_]);
      return setAlarmHour(alarm, schedule_lamp[track_lamp_]);
   }

}

bool SystemTime::setNextSchedule(unsigned char alarm) {

   // set next time for alarm 2
   if(alarm==ALARM_MEASURE) {
      track_measure_ = (track_measure_ + 1) % SCHEDULE_MEASURE_MAX;
      Log.verboses("Set time for alarm %d: %uh", alarm, schedule_measure[track_measure_]);
      // Because we use alarm 2 for repeating in minute. so dont use the code below
      // return setAlarmHour(alarm, schedule_measure[track_measure_]);
      return true;
   }
   // set next time for alarm 1
   else {
      track_lamp_ = (track_lamp_ + 1) % SCHEDULE_LAMP_MAX;
      Log.verboses("Set time for alarm %d: %uh", alarm, schedule_lamp[track_lamp_]);
      return setAlarmHour(alarm, schedule_lamp[track_lamp_]);
   }

}

unsigned char SystemTime::trackCurrent(unsigned int track) {
   if(track==TRACK_LAMP)
      return track_lamp_;
   else
      return track_measure_;
}
// Create a global Time
SystemTime Time;
