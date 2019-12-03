#ifndef ntpTimeClass_h
#define ntpTimeClass_h

#include <Arduino.h>
#include "time.h"

class ntpTimeClass {
    public:
        ntpTimeClass();
        void printLocalTime();
        int getSeconds();
        void ntpTimerConfig();

};

#endif 
