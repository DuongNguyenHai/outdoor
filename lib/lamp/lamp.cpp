#include "lamp.h"

SysLamps::SysLamps() {}
SysLamps::~SysLamps() {}

bool SysLamps::lamp_state = OFF;

void SysLamps::setupLamps() {
   pinMode(PIN_LAMP, OUTPUT);
   switchLamps();
}

void SysLamps::switchLamps() {
   // Serial.printf("track lamp: %d", Time.trackCurrent(TRACK_LAMP));
   if(Time.trackCurrent(TRACK_LAMP)!=0)
      switchLamps(ON);
   else
      switchLamps(OFF);
}

void SysLamps::switchLamps(bool state) {
   digitalWrite(PIN_LAMP, state);
   lamp_state = state;
   if(state)
      Log.verboses("Turn on lamps !");
   else
      Log.verboses("Turn off lamps !");
}
