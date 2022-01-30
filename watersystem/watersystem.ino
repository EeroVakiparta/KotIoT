
#include <OneWire.h>
#include <DallasTemperature.h>

#define TEMPERATURESENSOR_PIN 4

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire1(TEMPERATURESENSOR_PIN);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire1);

// TODO: use RTC data to save previous values

String temperatureC1 = "";
String temperatureC2 = "";
String temperatureC3 = "";
String temperatureC4 = "";
String temperatureC5 = "";

// Timer variables
unsigned long lastTime = 0;  
unsigned long timerDelay = 60000;

const char* ssid = "******";
const char* password = "*******";

String readDSTemperatureC(int indexi) {
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures(); 
  float tempC = sensors.getTempCByIndex(indexi);
  
  if(tempC == -127.00) {
    Serial.println("Failed to read from DS18B20 sensor");
    return "--";
  } else {
    Serial.print("Temperature Celsius: ");
    Serial.println(tempC); 
  }
  return String(tempC);
}

void setup(){
  Serial.begin(115200);
  Serial.println();
  
  sensors.begin();
    
  temperatureC1 = readDSTemperatureC(0);
  temperatureC2 = readDSTemperatureC(1);
  temperatureC3 = readDSTemperatureC(2);
  temperatureC4 = readDSTemperatureC(3);
  temperatureC5 = readDSTemperatureC(4);
  
}
 
void loop(){
  if ((millis() - lastTime) > timerDelay) {
    temperatureC1 = readDSTemperatureC(0);
    temperatureC2 = readDSTemperatureC(1);
    temperatureC3 = readDSTemperatureC(2);
    temperatureC4 = readDSTemperatureC(3);
    temperatureC5 = readDSTemperatureC(4);
    lastTime = millis();
  }  
}