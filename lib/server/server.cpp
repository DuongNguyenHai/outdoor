#include "server.h"

ESP8266WebServer server(80);
File fsUploadFile;

SysHost::SysHost() {};
SysHost::~SysHost() {};

bool convertStringToArry(String str, unsigned char *arry, char delim, unsigned char max) {
   unsigned char count = 0, idx = 0;
   unsigned char tail = count;
   while (str[count]!='\0') {
      if(str[count]==delim) {
         arry[idx] = str.substring(tail, count).toInt();
         idx++;
         if(idx==max) break;
         count++;
         tail = count;
      }
      count++;
   }
   if(idx<max) {
      arry[idx] = str.substring(tail, count).toInt();
      idx++;
   }
}

bool SysHost::setupHost() {

   MDNS.begin(HOST_NAME);
   //SERVER INIT
   //list directory
   server.on("/list", HTTP_GET, handleFileList);
   //load editor
   server.on("/edit", HTTP_GET, [](){
     if(!handleFileRead("/edit.htm")) server.send(404, "text/plain", "FileNotFound");
   });
   //create file
   server.on("/edit", HTTP_PUT, handleFileCreate);
   //delete file
   server.on("/edit", HTTP_DELETE, handleFileDelete);
   //first callback is called after the request has ended with all parsed arguments
   //second callback handles file uploads at that location
   server.on("/edit", HTTP_POST, [](){ server.send(200, "text/plain", ""); }, handleFileUpload);

   server.on("/get", HTTP_POST, []() {
      int tempt = (int)(TemptTank.read()+0.5);
      char tm[20];
      Time.getDateTimeString(tm);
      String st;
      if(lamp_state==ON)
         st += "true";
      else
         st += "false";
      char s[30];
      snprintf(s, 30,"%s;%d;%s;",st.c_str(), tempt, tm); // ex : false;24;2017-02-12T21:52
      server.send(200, "text/plain", s);
   });

   server.on("/set", HTTP_POST, []() {

      if(server.hasArg(SWITCH_LAMP)) {
         // Serial.println("switch lamp: " + server.arg(0));
         if(server.arg(0).equals("true")) {
            SysLamps::switchLamps(ON);
            server.send(200, "text/plain", "S;Bật hệ thống chiếu sáng cho cây");
            DisplayLCD.displayOnlyLamptStatus(lamp_state);
         } else {
            SysLamps::switchLamps(OFF);
            server.send(200, "text/plain", "S;Tắt hệ thống chiếu sáng cho cây");
            DisplayLCD.displayOnlyLamptStatus(lamp_state);
         }
      }
      else if(server.hasArg(SWITCH_HEATER)){
         Serial.println("switch heater: " + server.arg(0));
         if(server.arg(0).equals("true")) {
            server.send(200, "text/plain", "S;Bật hệ thống sưởi bể cá");
         }else {
            server.send(200, "text/plain", "W;Tắt hệ thống sưởi bể cá");
         }
      }
      else if(server.hasArg(SWITCH_BUMPER)){
         Serial.println("switch bumper: " + server.arg(0));
         if(server.arg(0).equals("true")) {
            server.send(200, "text/plain", "S;Bật hệ thống bơm bể cá");
         }else {
            server.send(200, "text/plain", "W;Tắt hệ thống bơm bể cá");
         }
      }
      else if(server.hasArg(SET_TIME)) {
         // Serial.println("date: " + server.arg(0));
         // Serial.println("time: " + server.arg(1));
         Time.adjust(DateTime(server.arg(0).c_str(), server.arg(1).c_str()));
         Time.initAlarm(ALARM_LAMP);
         SysLamps::switchLamps();
         server.send(200, "text/plain", "S;Hệ thống đã cập nhật giờ");
         DisplayLCD.displayOnlyLamptStatus(lamp_state);
      }
      else if(server.hasArg(SET_LAMP)) {
         // Serial.println("set light: " + server.arg(0) + "-" + server.arg(1));
         schedule_lamp[0] = server.arg(0).toInt();
         schedule_lamp[1] = server.arg(1).toInt();
         Time.initAlarm(ALARM_LAMP);
         SysLamps::switchLamps();
         server.send(200, "text/plain", "S;Đã thiết lập khoảng thời gian bật đèn");
         DisplayLCD.displayOnlyLamptStatus(lamp_state);
      }
      else if(server.hasArg(SET_MEASURE)) {
         Serial.println("set measure schedule: " + server.arg(0));
         convertStringToArry(server.arg(0), schedule_measure, ',', SCHEDULE_MEASURE_MAX);
         server.send(200, "text/plain", "S;Đã thiết lập khung giờ đo nhiệt độ");
      } else if(server.hasArg(JSON)) {
         File fileSys = SPIFFS.open(SYSTEM_FILE, "w+");
         fileSys.println(server.arg(0));
         server.send(200, "text/plain", "S;JSON update success");
         fileSys.close();
      }
   });

   //called when the url is not defined here
   //use it to load content from SPIFFS
   server.onNotFound([](){
      if(!handleFileRead(server.uri()))
         server.send(404, "text/plain", "FileNotFound");
   });

   //get heap status, analog input value and all GPIO statuses in one json call
   server.on("/all", HTTP_GET, [](){
      String json = "{";
      json += "\"heap\":"+String(ESP.getFreeHeap());
      json += ", \"analog\":"+String(analogRead(A0));
      json += ", \"gpio\":"+String((uint32_t)(((GPI | GPO) & 0xFFFF) | ((GP16I & 0x01) << 16)));
      json += "}";
      server.send(200, "text/json", json);
      json = String();
   });

   server.begin();
   return false;
}

bool SysHost::handleClient() {
   server.handleClient();
}
