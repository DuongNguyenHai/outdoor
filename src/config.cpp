#include "config.h"

// loge_level = 0 : show nothing
//              1 : show error
//              2 : show error and log
//              3 : show error, log and verbose
char log_level = 2;
bool log_serial = 1;       // log via serial
bool log_online = 1;       // log via internet

char *ssid = (char *)"Song He";  // network name
char *password = (char *)"nuocchaydamdam"; // password

unsigned char schedule_measure[SCHEDULE_MEASURE_MAX] = {13,13};
unsigned char schedule_lamp[SCHEDULE_LAMP_MAX] = {13,13};
