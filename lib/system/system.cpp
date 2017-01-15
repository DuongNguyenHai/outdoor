#include "system.h"

SystemControl::SystemControl() {}
SystemControl::~SystemControl() {}

NetworkBase WirelessLocal;
NetworkUDP Network;

bool SystemControl::startUp() {

   Serial.begin(BAUD_RATE);                  // begin serial
   Wire.begin(ESP8266_SDA, ESP8266_SCL);     // begin I2C
   // Time.begin();     // begin I2C
   // eeprom.begin();    // begin I2C

   int rt_net = setUpNetwork();
   bool online = (rt_net != RT_FAIL) ? true : false;
   setUpSystemTime(online);
   readInfor();
   return true;
}

int SystemControl::setUpNetwork() {

   char ssid[30];
   char pass[30];
   char host[15];
   int port;

   Memory.readInforWifi(ssid,pass);
   Memory.readInforServer(host, port);

   Log.verboses("ssid: %s, pass: %s", ssid, pass);
   Log.verboses("host: %s, port: %d", host, port);

   int rt_wf = WirelessLocal.begin(ssid, pass, 10);
   int rt_net = Network.begin(host);

   if(rt_wf==RT_SUSS && rt_net==RT_SUSS)           // connecting to wifi and host success
      return RT_SUSS;
   else if(rt_wf==RT_SUSS && rt_net==RT_FAIL)      // connecting to wifi success but host fail
      return RT_MISS;
   else                                            // connecting wifi and host fail
      return RT_FAIL;
}

int SystemControl::setUpSystemTime(bool online) {

   if(online) {
      Time.setOnline();  // getting tim from internet and set system time
   } else {
      Log.warnings("Time will be set to default.");
      Time.setDefault();   // system time will be set to default value 0:0:0 jan 1 2017
   }

   Time.set(DateTime(2017,9,1,9,59,50).unixtime()); // 13:59:50 2017/1/8
   Time.initAlarm(ALARM_1);
   Time.initAlarm(ALARM_2);
}

int SystemControl::serverConfig() {
   return RT_SUSS;
}

int SystemControl::readInfor() {
   int tempt = (int)(TemptTank.read()+0.5);        // round float -> int
   if(tempt!=NOT_REAL) {
      Log.verboses("Tempt tank %d (C)", tempt);
      // Network.send("Tempt tank %d (C)", tempt);
      Display.number(tempt);
   }
   else {
      Log.errors("Reading sensor failed !");
   }
}

int SystemControl::handleAlarm() {
   if(Time.isAlarmOn()) {
      if(Time.WhichsAlarmOn(ALARM_1)) {
         int tempt = (int)(TemptTank.read()+0.5);        // round float -> int
         if(tempt!=NOT_REAL) {
            char tm[22];
            Time.getTimeString(tm);
            Log.logs("Alarm 1 is on %s: %d (C)", tm, tempt);
            // Network.send("Alarm 1 is on %s: %d (C)", tm, tempt);
            Display.number(tempt);
            Time.setNextSchedule(ALARM_1);
         }
         else {
            Log.errors("Reading sensor failed !");
         }
      } else if(Time.WhichsAlarmOn(ALARM_2)) {
            char tm[22];
            Time.getTimeString(tm);
            Log.logs("Alarm 2 is on %s", tm);
            // Network.send("Alarm 2 is on %s", tm);
            Time.setNextSchedule(ALARM_2);
      }
   }
   return RT_SUSS;
}

SystemControl System;
