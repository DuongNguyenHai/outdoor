#ifndef LAMP_H
#define LAMP_H

#include "config.h"
#include "display.h"
#include "realtime.h"

class SysLamps {

public:
   SysLamps();
   ~SysLamps();
   static void setupLamps();
   static void switchLamps();
   static void switchLamps(bool state);
   static bool lamp_state;                            // state of lamp
   static bool user_handle;
private:

};

#endif
