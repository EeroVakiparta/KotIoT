int analogPin = 35;

void setup() {
  Serial.begin(115200);
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
}
