/*
Source: Wisconsin Department of Health Services
The levels of CO2 in the air and potential health problems are:

  400 ppm: average outdoor air level.
  400–1,000 ppm: typical level found in occupied spaces with good air exchange.
  1,000–2,000 ppm: level associated with complaints of drowsiness and poor air.
  2,000–5,000 ppm: level associated with headaches, sleepiness, and stagnant, stale, stuffy air. Poor concentration, loss of attention, increased heart rate and slight nausea may also be present.
  5,000 ppm: this indicates unusual air conditions where high levels of other gases could also be present. Toxicity or oxygen deprivation could occur. This is the permissible exposure limit for daily workplace exposures.
  40,000 ppm: this level is immediately harmful due to oxygen deprivation.
 */

#include "SparkFun_SGP30_Arduino_Library.h"
#include <Wire.h>

SGP30 airquality; 

void setup() 
{
  Serial.begin(9600);
  Wire.begin();
  if (airquality.begin() == false){
    Serial.println("No SGP30 -airquality sensor detected. Problem with wiring?");
    while (1);
  }
  airquality.initAirQuality();
}
 
void loop() 
{
  //SGP30 needs to warm up. It will display wrong values at start (400 ppm TVOC 0 ppb
  delay(1000); 
  airquality.measureAirQuality();
  Serial.print("CO2: ");
  Serial.print(airquality.CO2);
  Serial.println(" ppm");
  Serial.print("TVOC:  ");
  Serial.print(airquality.TVOC);
  Serial.println(" ppb");
}
