
#include <OneWire.h>
#include <DallasTemperature.h>

#define TEMPERATURESENSOR_PIN 4
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  60       /* Time ESP32 will go to sleep (in seconds) */

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire1(TEMPERATURESENSOR_PIN);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire1);

// TODO: read values with ULP core
RTC_DATA_ATTR float oldTempC1 = 0;
RTC_DATA_ATTR float oldTempC2 = 0;
RTC_DATA_ATTR float oldTempC3 = 0;
RTC_DATA_ATTR float oldTempC4 = 0;
RTC_DATA_ATTR float oldTempC5 = 0;

const char* ssid = "******";
const char* password = "*******";

float readDSTemperatureC(int indexi) {
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures(); 
  float tempC = sensors.getTempCByIndex(indexi);

  Serial.println(indexi); 
  if(tempC == -127.00) {
    Serial.println("-Failed to read from DS18B20 sensor");
  } else {
    Serial.print("-index temperature: ");
    Serial.println(tempC); 
  }
  return tempC;
}

void setup(){
  Serial.begin(115200);
  
  sensors.begin(); 
  // does sensors need time to wake up before use?
  delay(100);
  float temperatureC1 = readDSTemperatureC(0);
  float temperatureC2 = readDSTemperatureC(1);
  float temperatureC3 = readDSTemperatureC(2);
  float temperatureC4 = readDSTemperatureC(3);
  float temperatureC5 = readDSTemperatureC(4);
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

  if ( temperatureC1 != oldTempC1 || 
    temperatureC2 != oldTempC2 || 
    temperatureC3 != oldTempC3 || 
    temperatureC4 != oldTempC4 ||
    temperatureC5 != oldTempC5 ){
        //TODO: send values
    }
    oldTempC1 = temperatureC1;
    oldTempC2 = temperatureC2;
    oldTempC3 = temperatureC3;
    oldTempC4 = temperatureC4;
    oldTempC5 = temperatureC5;

    delay(5);
    esp_deep_sleep_start();
}
 
void loop(){

}