#include <Arduino.h>
#include "WiFi.h"
#include "time.h"

const char* ssid       = "SPARK-9EKLRC";
const char* password   = "FVBLEW5LVT";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;



void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

int getSeconds() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return -1;
  }
  return timeinfo.tm_sec;
}

void setup()
{
  Serial.begin(9600);
  
  pinMode(GPIO_NUM_16, OUTPUT);

  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
  

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop()
{
 
  delay(1000);

  if ((getSeconds() == 20)|(getSeconds() == 40)) {
    Serial.println("We made it baby");
    digitalWrite(GPIO_NUM_16, HIGH);
    delay(5000);
    digitalWrite(GPIO_NUM_16, LOW);
        
  }
  Serial.println(getSeconds());  

}