
#ifndef NETWORK_H
#define NETWORK_H

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include "config.h"

#define LED 2       // has the same meaning with PIN_READY but show to user by led
#define PORT 8888                // PORT connect to log server

class NetworkBase {

public:
   NetworkBase();
   ~NetworkBase();
   int begin(const char *ssid_, const char *pass_, unsigned char times_out=120);

private:
   WiFiEventHandler gotIpEventHandler, disconnectedEventHandler;
};

class NetworkTCPClient : public WiFiClient {

public:
   NetworkTCPClient();
   ~NetworkTCPClient();
   int begin(const char *hostServer);
   bool send(char *s);
   void stop();
private:
   char *host_;
};

class NetworkUDP : public WiFiUDP {

public:
   NetworkUDP();
   ~NetworkUDP();
   int begin(const char *hostServer);
   bool send(char *s);
   void send(const char *fmt, ... );
   bool getData(char *s);
   void stop();
private:
   IPAddress root_ip;
   unsigned int root_PORT;
   unsigned int packetSize;
   char host_[15];
};

extern NetworkBase WirelessLocal;
extern NetworkUDP Network;

#endif
