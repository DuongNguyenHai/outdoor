// Nguyen Hai Duong
// Jan 5 2017

#include "system.h"

unsigned long time_sys = 0;
unsigned int addr = SYS_OFFSET;

void setup()
{
   // System.startUp();
   Serial.begin(115200);

   char ssid[30] = "Song He";  // network name
   char password[30] = "nuocchaydamdam"; // password
   char host[15] = "192.168.0.101"; // ip of Root
   int port = 8888;
   Memory.begin();
   Memory.clear(0,100);
   Memory.writeInforWifi(ssid,pass);
   Memory.writeInforServer(host,port);

}

void loop()
{
   char s[64];
   char ssid[30];
   char pass[30];
   char host[15];
   int port;
   unsigned int n = Memory.readString(WIFI_OFFSET,s,64);
   Log.printfs("%s",s);
   n = Memory.readString(SERVER_OFFSET,s,64);
   Log.printfs("%s",s);
   Memory.readInforWifi(ssid,pass);
   Memory.readInforServer(host, port);
   Log.printfs("ssid: %s - pass %s", ssid, pass);
   Log.printfs("host: %s - port %d", host, port);
   delay (1000); // Wait one second before repeating :)
}
