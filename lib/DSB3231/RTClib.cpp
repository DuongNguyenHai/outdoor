// Code by JeeLabs http://news.jeelabs.org/code/
// Released to the public domain! Enjoy!

#include "RTClib.h"
#ifdef __AVR__
 #include <avr/pgmspace.h>
#elif defined(ESP8266)
 #include <pgmspace.h>
#elif defined(ARDUINO_ARCH_SAMD)
// nothing special needed
#elif defined(ARDUINO_SAM_DUE)
 #define PROGMEM
 #define pgm_read_byte(addr) (*(const uint8_t *)(addr))
 #define Wire Wire1
#endif



#if (ARDUINO >= 100)
 #include <Arduino.h> // capital A so it is error prone on case-sensitive filesystems
 // Macro to deal with the difference in I2C write functions from old and new Arduino versions.
 #define _I2C_WRITE write
 #define _I2C_READ  read
#else
 #include <WProgram.h>
 #define _I2C_WRITE send
 #define _I2C_READ  receive
#endif


static uint8_t read_i2c_register(uint8_t addr, uint8_t reg) {
  Wire.beginTransmission(addr);
  Wire._I2C_WRITE((byte)reg);
  Wire.endTransmission();

  Wire.requestFrom(addr, (byte)1);
  return Wire._I2C_READ();
}

static void write_i2c_register(uint8_t addr, uint8_t reg, uint8_t val) {
  Wire.beginTransmission(addr);
  Wire._I2C_WRITE((byte)reg);
  Wire._I2C_WRITE((byte)val);
  Wire.endTransmission();
}

static uint8_t bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }
static uint8_t bin2bcd (uint8_t val) { return val + 6 * (val / 10); }

////////////////////////////////////////////////////////////////////////////////
// utility code, some of this could be exposed in the DateTime API if needed

const uint8_t daysInMonth [] PROGMEM = { 31,28,31,30,31,30,31,31,30,31,30,31 };

// number of days since 2000/01/01, valid for 2001..2099
static uint16_t date2days(uint16_t y, uint8_t m, uint8_t d) {
    if (y >= 2000)
        y -= 2000;
    uint16_t days = d;
    for (uint8_t i = 1; i < m; ++i)
        days += pgm_read_byte(daysInMonth + i - 1);
    if (m > 2 && y % 4 == 0)
        ++days;
    return days + 365 * y + (y + 3) / 4 - 1;
}

static long time2long(uint16_t days, uint8_t h, uint8_t m, uint8_t s) {
    return ((days * 24L + h) * 60 + m) * 60 + s;
}

////////////////////////////////////////////////////////////////////////////////
// DateTime implementation - ignores time zones and DST changes
// NOTE: also ignores leap seconds, see http://en.wikipedia.org/wiki/Leap_second

DateTime::DateTime (uint32_t t) {
   t -= SECONDS_FROM_1970_TO_2000;    // bring to 2000 timestamp from 1970

   ss = t % 60;
   t /= 60;
   mm = t % 60;
   t /= 60;
   hh = t % 24;
   uint16_t days = t / 24;
   uint8_t leap;
   for (yOff = 0; ; ++yOff) {
     leap = yOff % 4 == 0;
     if (days < 365 + leap)
         break;
     days -= 365 + leap;
   }
   for (m = 1; ; ++m) {
     uint8_t daysPerMonth = pgm_read_byte(daysInMonth + m - 1);
     if (leap && m == 2)
         ++daysPerMonth;
     if (days < daysPerMonth)
         break;
     days -= daysPerMonth;
   }
   d = days + 1;
}

DateTime::DateTime (uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec) {
    if (year >= 2000)
        year -= 2000;
    yOff = year;
    m = month;
    d = day;
    hh = hour;
    mm = min;
    ss = sec;
}

DateTime::DateTime (const DateTime& copy):
  yOff(copy.yOff),
  m(copy.m),
  d(copy.d),
  hh(copy.hh),
  mm(copy.mm),
  ss(copy.ss)
{}

static uint8_t conv2d(const char* p) {
    uint8_t v = 0;
    if ('0' <= *p && *p <= '9')
        v = *p - '0';
    return 10 * v + *++p - '0';
}

// A convenient constructor for using "the compiler's time":
//   DateTime now (__DATE__, __TIME__);
// NOTE: using F() would further reduce the RAM footprint, see below.
DateTime::DateTime (const char* date, const char* time) {
    // sample input: date = "Dec 26 2009", time = "12:34:56"
    yOff = conv2d(date + 9);
    // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
    switch (date[0]) {
        case 'J': m = date[1] == 'a' ? 1 : m = date[2] == 'n' ? 6 : 7; break;
        case 'F': m = 2; break;
        case 'A': m = date[2] == 'r' ? 4 : 8; break;
        case 'M': m = date[2] == 'r' ? 3 : 5; break;
        case 'S': m = 9; break;
        case 'O': m = 10; break;
        case 'N': m = 11; break;
        case 'D': m = 12; break;
    }
    d = conv2d(date + 4);
    hh = conv2d(time);
    mm = conv2d(time + 3);
    ss = conv2d(time + 6);
}

// A convenient constructor for using "the compiler's time":
// This version will save RAM by using PROGMEM to store it by using the F macro.
//   DateTime now (F(__DATE__), F(__TIME__));
DateTime::DateTime (const __FlashStringHelper* date, const __FlashStringHelper* time) {
    // sample input: date = "Dec 26 2009", time = "12:34:56"
    char buff[11];
    memcpy_P(buff, date, 11);
    yOff = conv2d(buff + 9);
    // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
    switch (buff[0]) {
        case 'J': m = buff[1] == 'a' ? 1 : m = buff[2] == 'n' ? 6 : 7; break;
        case 'F': m = 2; break;
        case 'A': m = buff[2] == 'r' ? 4 : 8; break;
        case 'M': m = buff[2] == 'r' ? 3 : 5; break;
        case 'S': m = 9; break;
        case 'O': m = 10; break;
        case 'N': m = 11; break;
        case 'D': m = 12; break;
    }
    d = conv2d(buff + 4);
    memcpy_P(buff, time, 8);
    hh = conv2d(buff);
    mm = conv2d(buff + 3);
    ss = conv2d(buff + 6);
}

uint8_t DateTime::dayOfTheWeek() const {
    uint16_t day = date2days(yOff, m, d);
    return (day + 6) % 7; // Jan 1, 2000 is a Saturday, i.e. returns 6
}

uint32_t DateTime::unixtime(void) const {
  uint32_t t;
  uint16_t days = date2days(yOff, m, d);
  t = time2long(days, hh, mm, ss);
  t += SECONDS_FROM_1970_TO_2000;  // seconds from 1970 to 2000

  return t;
}

long DateTime::secondstime(void) const {
  long t;
  uint16_t days = date2days(yOff, m, d);
  t = time2long(days, hh, mm, ss);
  return t;
}

DateTime DateTime::operator+(const TimeSpan& span) {
  return DateTime(unixtime()+span.totalseconds());
}

DateTime DateTime::operator-(const TimeSpan& span) {
  return DateTime(unixtime()-span.totalseconds());
}

TimeSpan DateTime::operator-(const DateTime& right) {
  return TimeSpan(unixtime()-right.unixtime());
}

////////////////////////////////////////////////////////////////////////////////
// TimeSpan implementation

TimeSpan::TimeSpan (int32_t seconds):
  _seconds(seconds)
{}

TimeSpan::TimeSpan (int16_t days, int8_t hours, int8_t minutes, int8_t seconds):
  _seconds((int32_t)days*86400L + (int32_t)hours*3600 + (int32_t)minutes*60 + seconds)
{}

TimeSpan::TimeSpan (const TimeSpan& copy):
  _seconds(copy._seconds)
{}

TimeSpan TimeSpan::operator+(const TimeSpan& right) {
  return TimeSpan(_seconds+right._seconds);
}

TimeSpan TimeSpan::operator-(const TimeSpan& right) {
  return TimeSpan(_seconds-right._seconds);
}

////////////////////////////////////////////////////////////////////////////////
// RTC_Millis implementation

long RTC_Millis::offset = 0;

void RTC_Millis::adjust(const DateTime& dt) {
    offset = dt.unixtime() - millis() / 1000;
}

DateTime RTC_Millis::now() {
  return (uint32_t)(offset + millis() / 1000);
}

////////////////////////////////////////////////////////////////////////////////
// RTC_DS3231 implementation

boolean RTC_DS3231::begin(void) {
   #if defined(ESP8266)
   Wire.begin(ESP8266_SDA, ESP8266_SCL);
   #else
   Wire.begin();
   #endif
   return true;
}

bool RTC_DS3231::lostPower(void) {
  return (read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG) >> 7);
}

void RTC_DS3231::adjust(const DateTime& dt) {
  Wire.beginTransmission(DS3231_ADDRESS);
  Wire._I2C_WRITE((byte)0); // start at location 0
  Wire._I2C_WRITE(bin2bcd(dt.second()));
  Wire._I2C_WRITE(bin2bcd(dt.minute()));
  Wire._I2C_WRITE(bin2bcd(dt.hour()));
  Wire._I2C_WRITE(bin2bcd(0));
  Wire._I2C_WRITE(bin2bcd(dt.day()));
  Wire._I2C_WRITE(bin2bcd(dt.month()));
  Wire._I2C_WRITE(bin2bcd(dt.year() - 2000));
  Wire.endTransmission();

  uint8_t statreg = read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG);
  statreg &= ~0x80; // flip OSF bit
  write_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, statreg);
}

DateTime RTC_DS3231::now() {
  Wire.beginTransmission(DS3231_ADDRESS);
  Wire._I2C_WRITE((byte)0);
  Wire.endTransmission();

  Wire.requestFrom(DS3231_ADDRESS, 7);
  uint8_t ss = bcd2bin(Wire._I2C_READ() & 0x7F);
  uint8_t mm = bcd2bin(Wire._I2C_READ());
  uint8_t hh = bcd2bin(Wire._I2C_READ());
  Wire._I2C_READ();
  uint8_t d = bcd2bin(Wire._I2C_READ());
  uint8_t m = bcd2bin(Wire._I2C_READ());
  uint16_t y = bcd2bin(Wire._I2C_READ()) + 2000;

  return DateTime (y, m, d, hh, mm, ss);
}

Ds3231SqwPinMode RTC_DS3231::readSqwPinMode() {
  int mode;

  Wire.beginTransmission(DS3231_ADDRESS);
  Wire._I2C_WRITE(DS3231_CONTROL);
  Wire.endTransmission();

  Wire.requestFrom((uint8_t)DS3231_ADDRESS, (uint8_t)1);
  mode = Wire._I2C_READ();
  mode &= 0x93;
  return static_cast<Ds3231SqwPinMode>(mode);
}

void RTC_DS3231::writeSqwPinMode(Ds3231SqwPinMode mode) {
  uint8_t ctrl;
  ctrl = read_i2c_register(DS3231_ADDRESS, DS3231_CONTROL);

  ctrl &= ~0x04; // turn off INTCON
  ctrl &= ~0x18; // set freq bits to 0

  if (mode == DS3231_OFF) {
    ctrl |= 0x04; // turn on INTCN
  } else {
    ctrl |= mode;
  }
  write_i2c_register(DS3231_ADDRESS, DS3231_CONTROL, ctrl);
}

uint8_t RTC_DS3231::readRegControl() {
   return read_i2c_register(DS3231_ADDRESS, DS3231_CONTROL);
}

bool RTC_DS3231::setAlarm(uint8_t alarm) {
   uint8_t ctrl = read_i2c_register(DS3231_ADDRESS, DS3231_CONTROL);
   ctrl |= 0x04;  // turn on INTCN
   ctrl |= alarm;  // set bit alarm
   write_i2c_register(DS3231_ADDRESS, DS3231_CONTROL, ctrl);
   return true;
}

bool RTC_DS3231::isAlarmEnable() {
   return  (read_i2c_register(DS3231_ADDRESS, DS3231_CONTROL) & 0x04) ? true : false;
}

bool RTC_DS3231::switchAlarm(bool state) {
   uint8_t ctrl = read_i2c_register(DS3231_ADDRESS, DS3231_CONTROL);
   if(state)
      ctrl |= 0x04;  // turn on INTCN
   else
      ctrl &= ~0x04; // turn off INTCN
   write_i2c_register(DS3231_ADDRESS, DS3231_CONTROL, ctrl);
   return true;
}

bool RTC_DS3231::setAlarmHour(uint8_t alarm, uint8_t hh) {

   setAlarm(alarm);
   Wire.beginTransmission(DS3231_ADDRESS);
   if(alarm==DS3231_ALARM_1) {
      Wire._I2C_WRITE((byte)DS3231_ALARM_1_ADDR);     // start at location of alarm 1 register
      Wire._I2C_WRITE(0);                             // second = 0 and A*M1 = 0
   }
   else {
      Wire._I2C_WRITE((byte)DS3231_ALARM_2_ADDR);     // start at location of alarm 2 register
   }
   Wire._I2C_WRITE(0);                                // minute = 0 and A*M2 = 0
   Wire._I2C_WRITE(bin2bcd(hh) | 0x80);               // hour = hh and A*M3 = 0
   Wire._I2C_WRITE(0x80);                             // A*M4 = 1;
   Wire.endTransmission();

   // check set alarm has successed or not
   Wire.beginTransmission(DS3231_ADDRESS);
   if(alarm==DS3231_ALARM_1)
      Wire._I2C_WRITE((byte)DS3231_ALARM_1_ADDR);
   else
      Wire._I2C_WRITE((byte)DS3231_ALARM_2_ADDR);
   Wire.endTransmission();

   if(alarm==DS3231_ALARM_1) {
      Wire.requestFrom(DS3231_ADDRESS, 4);
      if( Wire._I2C_READ()!=0 )
         return false;

   }
   else
      Wire.requestFrom(DS3231_ADDRESS, 3);

   if( Wire._I2C_READ()!=0 || Wire._I2C_READ() != (bin2bcd(hh) | 0x80) || Wire._I2C_READ() != 0x80 )
      return false;

   return true;
}

int RTC_DS3231::setAlarm(uint8_t alarm, const DateTime& dt, uint8_t mode) {

   int mm = -1, hh = -1, date = -1, ss = -1;
   setAlarm(alarm);

   if(mode>0)
      ss = dt.second();
   if(mode>1)
      mm = dt.minute();
   if(mode>2)
      hh = dt.hour();
   if(mode>3)
      date = dt.day();

   // Serial.printf("(mm:hh:date:ss) %d:%d:%d:%d\n", mm, hh, date, ss);
   if(alarm==DS3231_ALARM_1)
      setAlarmReg(DS3231_ALARM_1_ADDR, mm, hh, date, ss);
   else
      setAlarmReg(DS3231_ALARM_2_ADDR, mm, hh, date, ss);

   return 0;
}

int RTC_DS3231::setAlarmReg(uint8_t addr, int mm, int hh, int date, int ss) {

   Wire.beginTransmission(DS3231_ADDRESS);
   Wire._I2C_WRITE((byte)addr);                      // start at location addr

   if(addr==DS3231_ALARM_1_ADDR)
      if(ss!=-1)
         Wire._I2C_WRITE(bin2bcd(ss) & ~0x80 );    // bit A*M2 = 0
      else
         Wire._I2C_WRITE(0x80);                    // bit A*M2 = 1
   if(mm!=-1)
      Wire._I2C_WRITE(bin2bcd(mm) & ~0x80 );       // bit A*M2 = 0
   else
      Wire._I2C_WRITE(0x80);                       // bit A*M3 = 1
   if(hh!=-1)
      Wire._I2C_WRITE(bin2bcd(hh) & ~0x80 );       // bit A*M3 = 0
   else
      Wire._I2C_WRITE(0x80);                       // bit A*M3 = 1
   if(date!=-1)
      Wire._I2C_WRITE(bin2bcd(date) & ~0xC0 );     // bit A*M4 = 0, bit DY/DT = 0
   else
      Wire._I2C_WRITE(0x80);                       // bit A*M4 = 1

   Wire.endTransmission();
   return 0;
}

uint8_t RTC_DS3231::readRegStatus() {
   return read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG);
}

uint8_t RTC_DS3231::readRegister(uint8_t reg) {
   return read_i2c_register(DS3231_ADDRESS, reg);
}

bool RTC_DS3231::isAlarmSet(uint8_t alarm) {
   return  (read_i2c_register(DS3231_ADDRESS, DS3231_CONTROL) & alarm) ? true : false;
}

bool RTC_DS3231::isFlagSet(uint8_t alarm) {
   return (read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG) & alarm) ? true : false;
}

bool RTC_DS3231::clearFlag(uint8_t alarm) {
   uint8_t regStatus = read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG);
   if(alarm==0)
      regStatus &= ~0x03;
   else
      regStatus &= ~alarm;

   write_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, regStatus);
   return true;
}