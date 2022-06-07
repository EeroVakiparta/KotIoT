/*
  CO2 AND OTHER VOLATILE GASES
    The levels of CO2 in the air and potential health problems are:

    400 ppm: average outdoor air level.
    400–1,000 ppm: typical level found in occupied spaces with good air exchange.
    1,000–2,000 ppm: level associated with complaints of drowsiness and poor air.
    2,000–5,000 ppm: level associated with headaches, sleepiness, and stagnant, stale, stuffy air. Poor concentration, loss of attention, increased heart rate and slight nausea may also be present.
    5,000 ppm: this indicates unusual air conditions where high levels of other gases could also be present. Toxicity or oxygen deprivation could occur. This is the permissible exposure limit for daily workplace exposures.
    40,000 ppm: this level is immediately harmful due to oxygen deprivation.
      Source: Wisconsin Department of Health Services

  TEMPERATURE AND HUMIDITY:
    Summer conditions: optimum temperature of 24.5°C with an acceptable range of 23-26°C
    Winter conditions: optimum temperature of 22°C with an acceptable range of 20-23.5°C

    Humidity in relation to acceptable temperatures in office
    Summer (Light clothing)
    If 30% then 24.5 - 28 °C
    If 60% then 23 - 25.5 °C

    Winter (Warm clothing)
    If 30% then 20.5 - 25.5 °C
    If 60% then 20 - 24 °C

    Optimum humidity levels are between 40% and 60%
    20 to 26 degrees people will become uncomfortable and productivity is likely to drop.
      Source: Canadian Centre of Occupational Health and Safety CSA Z412-17 Office Ergonomics

  MOVING DURING WORKDAY AND RESOURCE PRESENCE MONITORING
    Needs better sources. No good info found online easily.
    For now: move every 30min => good health and resource is actively working. => Better results and less sick leave.
*/

#include "SparkFun_SGP30_Arduino_Library.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

SGP30 airquality;
Adafruit_BMP280 bmp;

const int movementSenosr = 2;
bool isMotion = false;
int motionCheckInterval = 1000;
const int enableWorkingButtonPin = 4;

void setup() {
  Serial.begin(9600);
  while ( !Serial ) delay(100);

  //-- BMP Temperature and humidity sensor setup
  Serial.println(F("BMP280 test"));
  unsigned status;
  status = bmp.begin();
  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                     "try a different address!"));
    Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(), 16);
    Serial.print("        ID of 0xFF probably means a bad address\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    while (1) delay(10);
  }
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  //-- SGP30 CO2 & VOC sensor setup
  Wire.begin();
  if (airquality.begin() == false) {
    Serial.println("No SGP30 -airquality sensor detected. Problem with wiring?");
    while (1);
  }
  airquality.initAirQuality();

  //-- Movement RCWL-0516 microwave radar sensor
  pinMode (movementSenosr, INPUT);

  //-- Button to start and stop working
  pinMode(enableWorkingButtonPin, INPUT);

  Serial.println("Setup finished.");
}

void loop()
{
  //SGP30 needs to warm up. It will display wrong values at start (400 ppm TVOC 0 ppb
  delay(2000);

  int workingButtonState = digitalRead(PushButton);
  if ( workingButtonState == HIGH ) {
    Serial.println("WORKING");
  } else {
    Serial.println("SLACKING");
  }

  int motion = digitalRead(movementSenosr);
  if (motion == 1) {
    Serial.println("Resource is moving. Sign of life.");
    isMotion = true;
  } else {
    Serial.println("Resource is motionless or slacking.");
    isMotion = false;
  }

  airquality.measureAirQuality();
  Serial.print("CO2: ");
  Serial.print(airquality.CO2);
  Serial.println(" ppm");
  Serial.print("TVOC:  ");
  Serial.print(airquality.TVOC);
  Serial.println(" ppb");


  Serial.print(F("Temperature = "));
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");

  Serial.print(F("Pressure = "));
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");

  Serial.print(F("Approx altitude = "));
  Serial.print(bmp.readAltitude(1013.25)); /* TODO: make ability to adjust this to local normal to get best altitude readings */
  Serial.println(" m");

  Serial.println();
}
