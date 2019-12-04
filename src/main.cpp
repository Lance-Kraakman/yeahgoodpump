#include <Arduino.h>
#include "wifiConfigurations.h"
#include "WiFi.h"
#include "HTTPClient.h"

#define NTP_SERVER "pool.ntp.org"
#define GMTOFFSET_SEC 3600
#define DAYLIGHTOFFSET_SEC 3600

int getSeconds();
void printLocalTime();

uint32_t previousMillis = 0;

void setup()
{
  //Set maximum frequency -Optimize peformance (Power is not an issue)
  setCpuFrequencyMhz(160); 
  Serial.begin(9600);
  pinMode(GPIO_NUM_16, OUTPUT);
  

  //connect to WiFi
  Serial.printf("Connecting to %s ", WILMAS_SSID);
  WiFi.begin(WILMAS_SSID, WILMAS_PASS);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  configTime(GMTOFFSET_SEC, DAYLIGHTOFFSET_SEC, NTP_SERVER);
  Serial.println(" CONNECTED");
   delay(1000);
}

void loop()
{

  //The below three lines and if statement is basically a delay without actually halting the cpu. look at "Blink without delay Arduino"
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 10000) {  
    previousMillis = currentMillis;

    if (WiFi.status()==WL_CONNECTED) {
      HTTPClient http;
    //@Caleb the http://json ... website is a test website which you can call the url and it will give you data in JSON format (whats printed on screen)
    //this is just testing sending requests and getting data from http requests. 
    //in the future there will be comms between our own website w/ http requests
    //in the future this will be a requesest sent our website asking for data. and even more in the future
    //the website will send the ESP data and requests which will be returned to the website!
      http.begin("http://jsonplaceholder.typicode.com/comments?id=10"); //Specify the URL
      int httpCode = http.GET();                                        //Make the request
  
      if (httpCode > 0) { //Check for the returning code
  
          String payload = http.getString();
          Serial.println(httpCode);
          Serial.println(payload); //payload is our data in JSON format we need to extract it somehow
        }

      else {
        Serial.println("Error on HTTP request");
      }
      http.end(); //Free the resources
    }
  }
  
    if ((getSeconds() == 20)|(getSeconds() == 40)) {
      Serial.println("We made it baby");
      digitalWrite(GPIO_NUM_16, HIGH);
      delay(5000);
      digitalWrite(GPIO_NUM_16, LOW);
      Serial.println(getSeconds());
    }

}

//Theese functinons get the time and print the time.     
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
