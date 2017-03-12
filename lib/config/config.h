// Nguyen Hai Duong

// There are 2 version of esp8266 board now
// Notice to the first line define board version to choose right board

#ifndef CONFIG_H
#define CONFIG_H

#define ESP8266_BOARD_1          // main board ver 1
// #define ESP8266_BOARD_2       // main board ver 2

// three types of return
#define RT_MISS -1
#define RT_SUSS 0
#define RT_FAIL 1

// extern char ssid[30];  // network name
// extern char password[30]; // password
// extern char host[30]; // ip of host.

// loge_level = 0 : show nothing
//              1 : show error
//              2 : show error and warning
//              3 : show error, warning and log
//              4 : show error, warning, log, verbose
extern char log_level;
extern bool log_serial;  // log via serial
extern bool log_network; // log via network

#define BAUD_RATE 115200

#define SCHEDULE_MEASURE_MAX 8        // max times of alarm per day. it use for alarm 1
#define SCHEDULE_LAMP_MAX 2            // max times of alarm per day. it use for alarm 2
#define BUFFER_SIZE 128

extern unsigned char schedule_measure[SCHEDULE_MEASURE_MAX];
extern unsigned char schedule_lamp[SCHEDULE_LAMP_MAX];

// map data in eeprom
#define SYS_OFFSET 0
#define WIFI_OFFSET 0
#define SERVER_OFFSET 64
#define DATA_OFFSET 100
#define TOTAL_OFFSET 123

// I2C pin
#define ESP8266_SDA 2         // esp8266 ver 7, ver 12
#define ESP8266_SCL 14        // esp8266 ver 7, ver 12

#define I2C_LCD_ADDR 0x27     // use i2c to lcd module
// #define I2C_ARDUINO_LCD_ADDR 0x13 // use arduino to print lcd

#define ALARM_LAMP ALARM_1
#define ALARM_MEASURE ALARM_2

#define ON 1
#define OFF 0
#define ONLINE true
#define OFFLINE false

// define pin for ver 1.0
#ifdef ESP8266_BOARD_1

#define PIN_LAMP 4         // connect to relay pin. Its used for turn on/off lamps
#define PIN_TEMPT_TANK 5  // pin connect to sensor to measure tempt of tempt tank. the pin 4 and 5 in ESP ver 7 was exchanged in ESP ver 12
#define PIN_ALARM 13     // pin connect to alarm pin (INTCN) of RTC. This pin actives low
#define LATCH 12        // Pin connected to latch pin (ST_CP or RCLK (texas instrument)) of 74HC595
#define CLK 16          // Pin connected to clock pin (SH_CP or SRCLK (texas instrument)) of 74HC595
#define DATA 0          // Pin connected to Data in (DS or SER (texas instrument)) of 74HC595

// define pin for ver 2.0
#elif ESP8266_BOARD_2

#define PIN_TEMPT_TANK 5  // pin connect to sensor to measure tempt of tempt tank. the pin 4 and 5 in ESP ver 7 was exchanged in ESP ver 12
#define PIN_ALARM 4     // pin connect to alarm pin (INTCN) of RTC. This pin actives low
#define RCLK 16        // Pin connected to latch pin (ST_CP or RCLK (texas instrument)) of 74HC595
#define SRCLK 12          // Pin connected to clock pin (SH_CP or SRCLK (texas instrument)) of 74HC595
#define SER 13          // Pin connected to Data in (DS or SER (texas instrument)) of 74HC595

#endif

#endif
