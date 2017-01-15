

#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "OneWire.h"
#include "config.h"

#define NOT_REAL -100 // the value will not never exist

class Temperature : public OneWire {

public:
   Temperature() : OneWire(PIN_TEMPT_TANK) {};        // connect to pin 5/13/12/14, except pin 16;
   ~Temperature();
   float read();

private :
   byte addr[8];

};

extern Temperature TemptTank;

#endif
