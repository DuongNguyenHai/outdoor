#ifndef SERVER_H
#define SERVER_H

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#include "ArduinoJson.h"
#include "handle-file.h"
#include "lamp.h"
#include "log.h"
#include "temperature.h"

#define HOST_NAME "aquaos"
// use three define below to check what action
#define SET_TIME "date"
#define SET_LAMP "lightOnFrom"
#define SET_MEASURE "temptSchedule"
#define SWITCH_LAMP "switchLamp"
#define SWITCH_BUMPER "switchBumper"
#define SWITCH_HEATER "switchHeater"
#define JSON "json"
#define NETWORK_FILE "/network.txt"
#define SYSTEM_FILE "/system.txt"
#define TEMPT_FILE "/tempt.txt"

extern ESP8266WebServer server;
extern File fsUploadFile;

// bool setupHost();
// bool handleClient();

class SysHost : public SysLamps {

public:
   SysHost();
   ~SysHost();
   bool setupHost();
   bool handleClient();

};

bool convertStringToArry(String str, unsigned char *arry, char delim, unsigned char max);

#endif
