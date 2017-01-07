// Nguyen Hai Duong

#include "log.h"

#define PRINTF_BUF 128 // define the tmp buffer size (change if desired)

LOG::LOG() {}
LOG::~LOG() {}

void LOG::printf(const char *fmt, ... ) {
   va_list args;
   va_start (args, fmt );
   write(fmt, args);
   va_end (args);
}

#ifdef F // check to see if F() macro is available
void LOG::log(const __FlashStringHelper *fmt, ... ) {
   va_list args;
   va_start (args, fmt );
   write(fmt, args);
   va_end (args);
}
#endif

void LOG::log(const char *fmt, ... ) {
   if(log_level>1) {
      va_list args;
      va_start (args, fmt );
      write(fmt, args);
      va_end (args);
   }
}

#ifdef F // check to see if F() macro is available
void LOG::printf(const __FlashStringHelper *fmt, ... ) {
   if(log_level>1) {
      va_list args;
      va_start (args, fmt );
      write(fmt, args);
      va_end (args);
   }
}
#endif

void LOG::verbose(const char *fmt, ... ) {
   if(log_level>2) {
      va_list args;
      va_start (args, fmt );
      write(fmt, args);
      va_end (args);
   }
}

#ifdef F // check to see if F() macro is available
void LOG::verbose(const __FlashStringHelper *fmt, ... ) {
   if(log_level>2) {
      va_list args;
      va_start (args, fmt );
      write(fmt, args);
      va_end (args);
   }
}
#endif

void LOG::error(const char *fmt, ... ) {
   if(log_level>0) {
      va_list args;
      va_start (args, fmt );
      write(fmt, args);
      va_end (args);
   }
}

#ifdef F // check to see if F() macro is available
void LOG::error(const __FlashStringHelper *fmt, ... ) {
   if(log_level>0) {
      va_list args;
      va_start (args, fmt );
      write(fmt, args);
      va_end (args);
   }
}
#endif

void LOG::write(const char *fmt, va_list args) {
   if(log_serial || log_online) {
      char buf[PRINTF_BUF];
      vsnprintf(buf, PRINTF_BUF, fmt, args);

      if(log_serial)
         Serial.print(buf);
   }
}

#ifdef F // check to see if F() macro is available
void LOG::write(const __FlashStringHelper *fmt, va_list args) {
   if(log_serial || log_online) {
      char buf[PRINTF_BUF];
      #ifdef __AVR__
      vsnprintf_P(buf, sizeof(buf), (const char *)fmt, args); // progmem for AVR
      #else
      vsnprintf(buf, sizeof(buf), (const char *)fmt, args); // for the rest of the world
      #endif

      if(log_serial)
         Serial.print(buf);
   }
}
#endif

// define a global Log for every file can use it.
LOG Log;
