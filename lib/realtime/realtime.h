// Modifier : Nguyen Hai Duong
// Jan 5 2017

#ifndef REALTIME_H
#define REALTIME_H

#include <Time.h>
#include <RTClib.h>
#include <WiFiClient.h>
#include <log.h>

#define TIME_DIFF 30
#define GMT_VIETNAM 7

class SystemTime : public RTC {

public:
   SystemTime();
   ~SystemTime();
   int setRTC(time_t t);            // set time to RTC
   int setOnline();                 // get time from google and set that time to RTC
   int setDefault();                // set default time 00:00:00 jan 1 2017
   void print();                    // get time and print it
   bool isAlarmOn();                // check alarm on ? by alarm pin (INTCN). When alarm 1/2 on, RTC will active low INTCN
   bool WhatsAlarmOn(unsigned char alarm);  // whats alarm is ringing ?
   int setNextSchedule(unsigned char alarm); // set next time in schedule for alarm
private:
   time_t getTimeOnline();                   // get time from google
   time_t getTime();                         // get time from RTC
   time_t convertToDateTime(String str);     // convert time (string) to UNIX time
   void printDateTime(tmElements_t tm);      // print time & date
   void printDateTime(DateTime tm);          // print time & date
   unsigned char track_measure;              // use for tracking time in measure schedule
   unsigned char track_lamp;                 // use for tracking time in lamp measure
};

extern SystemTime Time;

#endif // REALTIME_H
