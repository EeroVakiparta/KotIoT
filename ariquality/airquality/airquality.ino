#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "*******";
const char* password = "*******";
const char* mqtt_server = "192.168.100.11";
const int measuringInterval = 10000; //millis, take into account the average-value-count-time

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

const int analogPin = 35;

void setup() {
  Serial.begin(115200);
  delay(1000);
  WiFi.begin(ssid, password);
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, 1883);
}

int getCo2Measurement(int pin) {
  int adcVal = getAverageCo2(pin, 10);
  float voltage = adcVal * (3.3 / 4095.0);
  if (voltage == 0) {
    return -1;
  } else if (voltage < 0.4) {
    return -2;
  } else {
    float voltageDiference = voltage - 0.4;
    return (int) ((voltageDiference * 5000.0) / 1.6);
  }
}

int getAverageCo2(int sensorPin, int sampleCount) {
  int co2values = 0;
  for (int i = 0; i < sampleCount; i++) {
    co2values = co2values + analogRead(sensorPin);
    delay(200);
  }
  return co2values / sampleCount;
}

void loop() {
  delay(measuringInterval);

  float co2measurement = getCo2Measurement(analogPin);
  if (isnan(co2measurement)) {
    co2measurement = -1;
    Serial.println("Check wiring.");
  }

  Serial.print("ppm: ");
  Serial.println(co2measurement);
  char co2Sting[8];
  dtostrf(co2measurement, 1, 2, co2Sting);
  client.publish("kotiote/airquality", co2Sting);
}
