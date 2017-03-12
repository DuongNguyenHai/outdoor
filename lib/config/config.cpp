#include "config.h"


char log_level = 4;
bool log_serial = 1;       // log via serial
bool log_network = 0;       // log via network

// char ssid[30] = "Song He";  // network name
// char password[30] = "nuocchaydamdam"; // password
// char host[30] = "192.168.0.101"; // ip of Root

// 2 shedule below is hour shedule. all value are hours.
unsigned char schedule_measure[SCHEDULE_MEASURE_MAX] = {2,5,8,11,14,17,20,23};
unsigned char schedule_lamp[SCHEDULE_LAMP_MAX] = {6,16};
