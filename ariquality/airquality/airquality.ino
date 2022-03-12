#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "*******";
const char* password = "*******";
const char* mqtt_server = "192.168.100.11";

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

int getCo2Measurement() {
  int adcVal = analogRead(analogPin);
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

void loop() {
  delay(10000);
  
  float co2measurement = getCo2Measurement();
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
