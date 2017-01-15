#include "temperature.h"

// Temperature::Temperature() {}
Temperature::~Temperature() {}

float Temperature::read() {

   byte data[9];
   if ( !OneWire::search(addr) ) {}

   if ( OneWire::crc8( addr, 7) != addr[7]) {
      return NOT_REAL;
   }

   OneWire::reset();
   OneWire::select(addr);
   OneWire::write(0x44,1);         // start conversion, with parasite power on at the end

   delay(1000);                     // maybe 750ms is enough, maybe not
   // we might do a ds.depower() here, but the reset will take care of it.

   OneWire::reset();
   OneWire::select(addr);
   OneWire::write(0xBE);                           // Read Scratchpad

   for (byte i = 0; i < 9; i++) {           // we need 9 bytes
      data[i] = OneWire::read();
   }
   // float celsius = (float)((data[1] << 8) | data[0]) / 16.0;
   return  (float)((data[1] << 8) | data[0]) / 16.0;
}

Temperature TemptTank;
