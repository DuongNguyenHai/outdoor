#include "eeprom.h"

SystemMemory::SystemMemory() {}
SystemMemory::~SystemMemory() {}

bool SystemMemory::writeInforWifi(const char *ssid, const char *pass) {
   unsigned int addr = printSpecial(WIFI_OFFSET, ssid, '*');
   addr = printSpecial(addr+1, pass, '*');
   return true;
}

bool SystemMemory::writeInforServer(const char *host, const int port) {
   unsigned int addr = printSpecial(SERVER_OFFSET, host, '*');
   addr = writeNumber(addr+1, port);
   addr = write(addr+1,'*');
   return true;
}

bool SystemMemory::readInforWifi(char *ssid, char *pass) {
   unsigned int addr = readStringUntil(WIFI_OFFSET,ssid,'*');
   addr = readStringUntil(addr+1,pass,'*');
   return true;
}

bool SystemMemory::readInforServer(char *host, int &port) {
   unsigned int addr = readStringUntil(SERVER_OFFSET,host,'*');
   addr = readNumber(78, port);
}

SystemMemory Memory;
