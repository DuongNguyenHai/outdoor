
#ifndef EEPROM_H
#define EEPROM_H

#include "config.h"
#include "EEPROM-AT24C256.h"

class SystemMemory : public EPROM_AT24C256 {

public:
   SystemMemory();
   ~SystemMemory();
   bool writeInforWifi(const char *ssid, const char *pass);
   bool writeInforServer(const char *host, const int port);
   bool readInforWifi(char *ssid, char *pass);
   bool readInforServer(char *host, int &port);
};

extern SystemMemory Memory;


#endif
