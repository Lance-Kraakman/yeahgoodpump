#include <Arduino.h>
#include "WiFi.h"
#include "HTTPClient.h"
#include "ntpTimeClass.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

const char* ssid       = "SPARK-9EKLRC";
const char* password   = "FVBLEW5LVT";

ntpTimeClass ntpTimer;

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
  ntpTimer = ntpTimeClass();
  
  //disconnect WiFi as it's no longer needed (do this on exit)
  //WiFi.disconnect(true);
  //WiFi.mode(WIFI_OFF);
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
  
  while(ntpTimer.getSeconds() < 60) { 
    if ((ntpTimer.getSeconds() == 20)|(ntpTimer.getSeconds() == 40)) {
    Serial.println("We made it baby");
    digitalWrite(GPIO_NUM_16, HIGH);
    delay(5000);
    digitalWrite(GPIO_NUM_16, LOW);
        
  } 
  Serial.println(ntpTimer.getSeconds());  
  }
}