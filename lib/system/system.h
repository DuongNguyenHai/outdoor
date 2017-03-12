
#ifndef SYSTEM_H
#define SYSTEM_H

#include <Arduino.h>
#include "log.h"
#include "config.h"
#include "realtime.h"
#include "network.h"
#include "temperature.h"
#include "display.h"
#include "lamp.h"
#include "server.h"

class SystemControl : public SysHost {

public:
   SystemControl();
   ~SystemControl();
   bool startUp();
   int config();
   int setupNetwork();
   bool setupSystemTime(bool online);
   bool setupDisplay();
   int serverConfig();
   int handleAlarm();

   int displayInfor();
   void displayInfor2LCD();
   void showHostFile();
private:

};


extern SystemControl System;

#endif
