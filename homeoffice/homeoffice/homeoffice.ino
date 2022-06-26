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
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_BMP280.h>
#include "kotiot_homeoffice.h" //If the library is in same folder, use quotes.

SGP30 airquality;
Adafruit_BMP280 bmp;

const int movementSenosr = 2;
const int enableWorkingButtonPin = 4;
const int hapticMotorPin = 12;

const char* ssid = "xxxxxxx";
const char* password = "xxxxxxxxxxxxxx";
const char* mqtt_server = "192.1.1.1";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

// variable declarations
bool isMoving = false;
bool isWorking = false;
float co2 = 0;
float voc = 0;
float temp = 0;
float pressure = 0;
float altitude = 0;

float altitudeCalibrationValue = 1013.25;
int motionCheckInterval = 1000;
int ariqualityCheckInterval = 1000;

void setup() {
  Serial.begin(9600);
  while ( !Serial ) delay(100);


  //-- Setup Wifi and MQTT
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

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

  //-- Pin for haptic motor
  pinMode(hapticMotorPin, OUTPUT);

  Serial.println("Setup finished.");
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "esp32/alarm") {
    Serial.print("Changing alarm to ");
    if(messageTemp == "on"){
      Serial.println("on");
      ClimbAndDown(hapticMotorPin, 3);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void checkResourceButton(bool resourceIsWorking) {
  if (!resourceIsWorking) {
    if (digitalRead(enableWorkingButtonPin) == HIGH) {
      ButtonFeedback(hapticMotorPin, 1);
      Serial.println("enableWorking");
      isWorking = true;
    }
  } else if (resourceIsWorking) {
    if (digitalRead(enableWorkingButtonPin) == LOW) {
      ButtonFeedback(hapticMotorPin, 2);
      Serial.println("disableWorking");
      isWorking = false;
    }
  }
}

//Movement sensor has built in delay.
//TODO: find out the delay
void checkResourceMovement(bool resourceIsMoving) {
  int motion = digitalRead(movementSenosr);
  if (motion == 1) {
    Serial.println("Resource is moving. Sign of life.");
    isMoving = true;
  } else {
    Serial.println("Resource is motionless or slacking.");
    isMoving = false;
  }
  char movementString[8];
  dtostrf(isMoving, 1, 2, movementString);
  client.publish("esp32/movement", movementString);
}

void measureAirQuality() {
  Serial.println("Measuring air quality...");
  airquality.measureAirQuality();
  co2 = airquality.CO2;
  voc = airquality.TVOC;
  Serial.print("CO2: "); Serial.print(co2); Serial.println(" ppm");
  Serial.print("VOC: "); Serial.print(voc); Serial.println(" ppb");

  char co2String[8];
  dtostrf(co2, 1, 2, co2String);
  client.publish("esp32/co2", co2String);
  
  char vocString[8];
  dtostrf(voc, 1, 2, vocString);
  client.publish("esp32/voc", vocString);
}

void measureAthmosphere() {
  Serial.println("Measuring atmosphere...");
  temp = bmp.readTemperature();
  pressure = bmp.readPressure();
  altitude = bmp.readAltitude(altitudeCalibrationValue);
  Serial.print(F("Temperature = ")); Serial.print(temp); Serial.println(" *C");
  Serial.print(F("Pressure = ")); Serial.print(pressure); Serial.println(" Pa");
  Serial.print(F("Approx altitude = ")); Serial.print(altitude);
  /* TODO: make ability to adjust this to local normal to get best altitude readings */
  Serial.println(" m");

  //TODO: make a function
  char tempString[8];
  dtostrf(temp, 1, 2, tempString);
  client.publish("esp32/temp", tempString);
  
  char pressureString[8];
  dtostrf(pressure, 1, 2, pressureString);
  client.publish("esp32/pressure", pressureString);
  
  char altString[8];
  dtostrf(altitude, 1, 2, altString);
  client.publish("esp32/altitude", altString);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  //SGP30 needs to warm up. It will display wrong values at start (400 ppm TVOC 0 ppb
  checkResourceButton(isWorking);
  checkResourceMovement(isMoving);
  measureAirQuality();
  measureAthmosphere();
}
