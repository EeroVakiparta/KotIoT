#include "kotiot_homeoffice.h"
#include <Arduino.h>

const int maxVibrateSpeed = 255;
const int minVibrateSpeed = 50;
//Whe using arduino functions need to have Arduino header included. Otherways you get weird errors.

// --- Motor functions --- //
void Pulse(int hapticMotorPinNumber, int pulseCount) {
  for (int i = 0; i < pulseCount; i++ ) {
    analogWrite(hapticMotorPinNumber, maxVibrateSpeed);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    analogWrite(hapticMotorPinNumber, 0);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    Serial.println("Pulse");
  }
}

void Climb(int hapticMotorPinNumber, int climbCount)
{
  for (int i = 0; i < climbCount; i++ ) {
    for (int i = minVibrateSpeed; i < maxVibrateSpeed; i = i + 5) {
      analogWrite(hapticMotorPinNumber, i);
      Serial.println(i);
      vTaskDelay(20 / portTICK_PERIOD_MS);
    }
    for (int i = maxVibrateSpeed; i > minVibrateSpeed; i = i - 5) {
      analogWrite(hapticMotorPinNumber, i);
      Serial.println(i);
      vTaskDelay(20 / portTICK_PERIOD_MS);
    }
    Serial.println("Climbed");
    Serial.println();
    analogWrite(hapticMotorPinNumber, 0);
  }
}

void StopMotor(int hapticMotorPinNumber)
{
  digitalWrite(hapticMotorPinNumber, LOW);
  Serial.println("Stop Vibration");
}

void ButtonFeedback(int hapticMotorPinNumber, int pulseCount) {
  analogWrite(hapticMotorPinNumber, maxVibrateSpeed);
  vTaskDelay(15 / portTICK_PERIOD_MS);
  analogWrite(hapticMotorPinNumber, 0);
  Serial.println("Button Feedback");
}
