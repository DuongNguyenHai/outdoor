#include "network.h"

// IPAddress ip_static(192,168,0,13);
// IPAddress ip_gateway(192,168,0,1);
// IPAddress ip_subnet(255,255,255,0);

NetworkBase::NetworkBase() {}
NetworkBase::~NetworkBase() {}

int NetworkBase::begin(const char *ssid_, const char *pass_, unsigned char times_out) {

   // Serial.printf("Connecting to %s ", ssid);
   // pinMode(LED, OUTPUT);
   // digitalWrite(LED, HIGH); // set the light off

   // gotIpEventHandler = WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP& event) {
   //    Serial.println("Wifi connected !");
   //    digitalWrite(LED, LOW); // set the light on
   // });
   //
   // disconnectedEventHandler = WiFi.onStationModeDisconnected([](const WiFiEventStationModeDisconnected& event) {
   //    digitalWrite(LED, HIGH); // set the light off
   //    Serial.println("WiFi disconnected !");
   // });

   WiFi.begin(ssid_, pass_);

   //  trying to connect to wifi (time = times_out*5000 ms)
   for (size_t i = 0; i < times_out; i++) {
      if (WiFi.status() != WL_CONNECTED) {
         Serial.printf("Trying to connect internet, %d\n", i);
         delay(5000);
      } else {
         break;
      }
      if(i==times_out-1) {
         // Serial.printf("Cant connect to internet.\n");
         return RT_FAIL;
      }
   }

   return RT_SUSS;
}

NetworkTCPClient::NetworkTCPClient() {}
NetworkTCPClient::~NetworkTCPClient() {}

int NetworkTCPClient::begin(const char *hostServer) {

   for (size_t i = 0; i < 13; i++) {
      if (!WiFiClient::connect(host_, PORT)) {
          delay(1000);
      } else break;
      if(i==13-1) {
         // Serial.printf("Cant connect to host: %s", host);
         return RT_FAIL;
      }
   }

   // Serial.printf("Connecting to host: %s\n", host_);
   return RT_SUSS;
}

bool NetworkTCPClient::send(char *s) {
   WiFiClient::print(s);
   return RT_SUSS;
}

void NetworkTCPClient::stop() {
   WiFiClient::stop();
}

NetworkUDP::NetworkUDP() {}
NetworkUDP::~NetworkUDP() {}

int NetworkUDP::begin(const char *hostServer) {
   strncpy(host_, hostServer, 15);
   if(WiFiUDP::begin(PORT)==1)
      return RT_SUSS;
   else
      return RT_FAIL;
}

bool NetworkUDP::getData(char *s) {
   if (packetSize = WiFiUDP::parsePacket()) {
     // Serial.printf("Received %d bytes from %s, PORT %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
     root_ip = WiFiUDP::remoteIP();  // imPORTant to save root_ip cause its will be erase in the next parsePacket()
     root_PORT = WiFiUDP::remotePort(); // imPORTant to save root_PORT
     int len = WiFiUDP::read(s, BUFFER_SIZE);
     if (len > 0) {
       s[len] = 0;
     }
   }
}

bool NetworkUDP::send(char *s) {
   WiFiUDP::beginPacket(host_, PORT);
   WiFiUDP::print(s);
   WiFiUDP::endPacket();
}

void NetworkUDP::send(const char *fmt, ... ) {
   char buf[BUFFER_SIZE];
   va_list args;
   va_start (args, fmt );
   vsnprintf(buf, BUFFER_SIZE, fmt, args);
   send(buf);
   va_end (args);
}

void NetworkUDP::stop() {
   WiFiUDP::stop();
}
