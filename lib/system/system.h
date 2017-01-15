
#ifndef SYSTEM_H
#define SYSTEM_H

#include <Arduino.h>
#include "log.h"
#include "config.h"
#include "realtime.h"
#include "network.h"
#include "temperature.h"
#include "eeprom.h"
#include "display.h"

class SystemControl {

public:
   SystemControl();
   ~SystemControl();
   bool startUp();
   int setUpNetwork();
   int setUpSystemTime(bool online);
   int serverConfig();
   int readInfor();
   int handleAlarm();
private:

};

extern SystemControl System;

#endif
