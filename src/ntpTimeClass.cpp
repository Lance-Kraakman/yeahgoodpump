#include "ntpTimeClass.h"

class ntpTimeClass { 

    const char* ntpServer = "pool.ntp.org";
    const long  gmtOffset_sec = 3600;
    const int   daylightOffset_sec = 3600;

    //when we create an instance we must have a connection to the internet
    ntpTimeClass() {
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    }
    
    void ntpTimeClass::printLocalTime()
    {
        struct tm timeinfo;
        
        if(!getLocalTime(&timeinfo)){
            Serial.println("Failed to obtain time");
            return;
        }
        
        Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    }

    int ntpTimeClass::getSeconds() {
        struct tm timeinfo;
       
        if(!getLocalTime(&timeinfo)){
            Serial.println("Failed to obtain time");
            return -1;
        }
        
        return timeinfo.tm_sec;
    }

    void ntpTimeClass::ntpTimerConfig() {
        //if we need to config the time again....
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    }


};

