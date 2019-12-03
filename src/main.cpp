#include <Arduino.h>
#include <HTTPClient.h>
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
  //Set maximum frequency -Optimize peformance (Power is not an issue)
  setCpuFrequencyMhz(240); 
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


//this wont work we need to schedule the tasks. with a task scheduler
//most likely freeRTOS. 

void loop()
{
 
  delay(1000);

  if (WiFi.status()==WL_CONNECTED) {
    HTTPClient http;
  //in the future this will be our own url -> for dev localhost:8080 etc (make springboot application using spring inititilizer)
  //which does the ui and back end etc and then we can get data from that. 
    http.begin("http://jsonplaceholder.typicode.com/comments?id=10"); //Specify the URL
    int httpCode = http.GET();                                        //Make the request
 
    if (httpCode > 0) { //Check for the returning code
 
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
 
    http.end(); //Free the resources
  }
  if ((getSeconds() == 20)|(getSeconds() == 40)) {
    Serial.println("We made it baby");
    digitalWrite(GPIO_NUM_16, HIGH);
    delay(5000);
    digitalWrite(GPIO_NUM_16, LOW);
        
  } 
  Serial.println(getSeconds());  

  


}