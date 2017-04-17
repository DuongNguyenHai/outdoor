#include "system.h"

NetworkBase WirelessLocal;
NetworkUDP Network;

SystemControl::SystemControl() {}
SystemControl::~SystemControl() {}

bool SystemControl::startUp() {

   Serial.begin(BAUD_RATE);                  // begin serial
   Wire.begin(ESP8266_SDA, ESP8266_SCL);     // begin I2C

   // Step 2: setup display monitor
   setupDisplay();
   DisplayLCD.print("System start",0,0);

   // Step 3: read config from system.txt file
   showHostFile();     // show all files in system
   config();

   // Step 4: setup network. Trying to connect to wifi. If has internet connection will get time from google
   // and if not, it will set system time offline
   int ret = setupNetwork();
   bool online = (ret != RT_FAIL) ? ONLINE : OFFLINE;

   setupSystemTime(online);

   // System.setupSystemTime(ONLINE);
   // Time.set(DateTime(2017, 03, 10, 22, 24, 40).unixtime()); // setup time

   // Step 5: setup lamps.
   setupLamps();

   // Step 6: setup server: access the web page at http://esp8266fs.local
   setupHost();

   // save data in file
   log_file = true;

   // Show information to display monitor
   char tm[22];
   Time.getDateTimeString(tm);
   Log.logs("System has took %lu ms for start up, %s", millis(), tm);

   displayInfor2LCD();
}

int SystemControl::config() {
   SPIFFS.begin();
   File fileSys = SPIFFS.open(SYSTEM_FILE, "r+");
   if (!fileSys) {
      Log.errors("network file open failed");
      return RT_FAIL;
   }
   String json = fileSys.readStringUntil('\n');
   StaticJsonBuffer<200>jsonBuffer;
   JsonObject& root = jsonBuffer.parseObject((char *)json.c_str());
   String schedule = root["measure_tempt_fishtank"]["temptSchedule"];
   convertStringToArry(schedule, schedule_measure, ',', SCHEDULE_MEASURE_MAX);

   int lightOnFrom = root["lamp"]["lightOnFrom"];
   int lightOnTo = root["lamp"]["lightOnTo"];

   schedule_lamp[0] = lightOnFrom;
   schedule_lamp[1] = lightOnTo;

   Serial.printf("lightOnFrom: %d, lightOnTo: %d", schedule_lamp[0], schedule_lamp[1]);
}

int SystemControl::setupNetwork() {

   File fileSys = SPIFFS.open(NETWORK_FILE, "r+");
   if (!fileSys) {
      Log.errors("network file open failed");
      return RT_FAIL;
   }
   String json = fileSys.readStringUntil('\n');
   fileSys.close();
   StaticJsonBuffer<100> jsonBuffer;
   JsonObject& root = jsonBuffer.parseObject((char *)json.c_str());
   String ssid = root["network"]["ssid"];
   String pass = root["network"]["pass"];
   String host = root["host"];

   // Log.verboses("ssid: %s, pass: %s, host: %s", ssid.c_str(), pass.c_str(), host.c_str());

   int rt_wf = WirelessLocal.begin(ssid.c_str(), pass.c_str(), 3);
   int rt_net = Network.begin(host.c_str());

   if(rt_wf==RT_SUSS)
      Log.logs("Internet connected !");
   else
      Log.warnings("Cant connect to internet !");

   if(rt_wf==RT_SUSS && rt_net==RT_SUSS)           // connecting to wifi and host success
      return RT_SUSS;
   else if(rt_wf==RT_SUSS && rt_net==RT_FAIL)      // connecting to wifi success but host fail
      return RT_MISS;
   else                                            // connecting wifi and host fail
      return RT_FAIL;
}

bool SystemControl::setupSystemTime(bool online) {

   if(online) {
      Time.setOnline();  // getting tim from internet and set system time
   } else {
      Log.warnings("Time will be set offline.");
      Time.setOffline();   // if RTC lost time, system time will be set to default value 0:0:0 jan 1 2017
   }

   Time.initAlarm(ALARM_LAMP);            // setting track_lamp for alarm 1, and set alarm time.
   Time.initAlarm(ALARM_MEASURE);         //  setting track_measure for alarm 2
   Time.setAlarmRepeat(ALARM_MEASURE);    // repeating alarm 2 in minute

   return true;
}

bool SystemControl::setupDisplay() {
   DisplayLCD.begin();
   return true;
}

int SystemControl::serverConfig() {
   return RT_SUSS;
}

int SystemControl::displayInfor() {
   int tempt = (int)(TemptTank.read()+0.5);        // round float -> int
   if(tempt!=NOT_REAL) {
      Log.verboses("Tempt tank %d (C)", tempt);
      // Network.send("Tempt tank %d (C)", tempt);
      // DisplayLCD.number(tempt);
   }
   else {
      Log.errors("Reading sensor failed !");
   }
}

int SystemControl::handleAlarm() {
   if(Time.isAlarmOn()) {
      if(Time.whichAlarmOn(ALARM_LAMP)) {
         char tm[22];
         Time.getTimeString(tm);
         Log.logs("Alarm lamp is on %ss ", tm);
         // Network.send("Alarm 1 is on %s: %d (C)", tm, tempt);
         Time.setNextSchedule(ALARM_LAMP);
         switchLamps();
         DisplayLCD.displayLamptStatus(lamp_state);
      } else if(Time.whichAlarmOn(ALARM_MEASURE)) {
         DisplayLCD.displayTime();  // display time in minute
         // check schedule measure.
         if(Time.getTimeHour() == schedule_measure[Time.trackCurrent(TRACK_MEASURE)]) {
            Time.setNextSchedule(ALARM_MEASURE);
            int tempt = (int)(TemptTank.read()+0.5);        // round float -> int
            if(tempt!=NOT_REAL) {
               DisplayLCD.displayTempt(tempt);
            }
            else {
               Log.errors("Reading sensor failed !");
            }
            if(log_file) {
               File fileSys = SPIFFS.open(TEMPT_FILE, "a+");
               fileSys.print(",");
               fileSys.print(tempt);
               fileSys.close();
            }
         }
      }
   }
   return RT_SUSS;
}

void SystemControl::displayInfor2LCD() {
   // DisplayLCD.print("system start", 0, 0);
   DisplayLCD.displayLamptStatus(lamp_state);
   DisplayLCD.print("Time", 6, 1);
   DisplayLCD.displayTime();
   int tempt = (int)(TemptTank.read()+0.5);        // round float -> int
   // float tempt = TemptTank.read();
   if(tempt!=NOT_REAL) {
      DisplayLCD.displayTempt(tempt);
   }
   else {
      Log.errors("Reading sensor failed !");
   }
}
void SystemControl::showHostFile() {

   Dir dir = SPIFFS.openDir("/");
   while (dir.next()) {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      Log.printfs("FS File: %s, size: %s", fileName.c_str(), formatBytes(fileSize).c_str());
   }
   FSInfo fs_info;
   SPIFFS.info(fs_info);
   Log.printfs("totalBytes: %d, usedBytes: %d, blockSize %d, pageSize %d, maxOpenFiles %d, maxPathLength %d\n",
                fs_info.totalBytes, fs_info.usedBytes, fs_info.blockSize, fs_info.pageSize, fs_info.maxOpenFiles , fs_info.maxPathLength);

}

SystemControl System;
