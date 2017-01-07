

#ifndef MAIN_H
#define MAIN_H

#include <log.h>

#define ALARM_INTER 5     // pin  active low
#define RT_MISS -1
#define RT_SUSS 0
#define RT_FAIL 1

extern char *ssid;  // network name
extern char *password; // password

extern char log_level;
extern bool log_serial;
extern bool log_online;

#define SCHEDULE_MEASURE_MAX 2
#define SCHEDULE_LAMP_MAX 2

extern unsigned char schedule_measure[SCHEDULE_MEASURE_MAX];
extern unsigned char schedule_lamp[SCHEDULE_LAMP_MAX];

#endif
