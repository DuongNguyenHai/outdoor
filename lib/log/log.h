

#ifndef LOG_H
#define LOG_H

#include <Arduino.h>
#include "../../src/config.h"
#include <stdarg.h>

class LOG {

public :
   LOG();
   ~LOG();

   void printf(const char *fmt, ... );
   #ifdef F // check to see if F() macro is available
   void printf(const __FlashStringHelper *fmt, ... );
   #endif

   void log(const char *fmt, ... );
   #ifdef F // check to see if F() macro is available
   void log(const __FlashStringHelper *fmt, ... );
   #endif

   void verbose(const char *fmt, ... );
   #ifdef F // check to see if F() macro is available
   void verbose(const __FlashStringHelper *fmt, ... );
   #endif

   void error(const char *fmt, ... );
   #ifdef F // check to see if F() macro is available
   void error(const __FlashStringHelper *fmt, ... );
   #endif

private:
   void write(const char *fmt, va_list args);
   #ifdef F // check to see if F() macro is available
   void write(const __FlashStringHelper *fmt, va_list args );
   #endif

};

extern LOG Log;

#endif
