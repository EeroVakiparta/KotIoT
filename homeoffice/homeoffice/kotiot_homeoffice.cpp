#include "kotiot_homeoffice.h"
#include <Arduino.h>

const int maxVibrateSpeed = 255;
const int minVibrateSpeed = 50;
//TODO: Get the following away from here.
const int buttonPulseLenght = 100;
const int buttonFeedBackLenght = 50;
const int climbSpeed = 20;
//Whe using arduino functions need to have Arduino header included. Otherways you get weird errors.

// --- Motor functions --- //
void Pulse(int hapticMotorPinNumber, int pulseCount) {
  for (int i = 0; i < pulseCount; i++ ) {
    analogWrite(hapticMotorPinNumber, maxVibrateSpeed);
    vTaskDelay(buttonPulseLenght / portTICK_PERIOD_MS);
    analogWrite(hapticMotorPinNumber, 0);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    Serial.println("Pulse");
  }
}

void ClimbAndDown(int hapticMotorPinNumber, int climbCount)
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
    Serial.println("ClimbAndDown done");
    Serial.println();
    analogWrite(hapticMotorPinNumber, 0);
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
    Serial.println("Climbed");
    Serial.println();
    analogWrite(hapticMotorPinNumber, 0);
  }
}

void GoDown(int hapticMotorPinNumber, int downCount)
{
  for (int i = 0; i < downCount; i++ ) {
    for (int i = maxVibrateSpeed; i > minVibrateSpeed; i = i - 5) {
      analogWrite(hapticMotorPinNumber, i);
      Serial.println(i);
      vTaskDelay(20 / portTICK_PERIOD_MS);
    }
    Serial.println("Went down");
    Serial.println();
    analogWrite(hapticMotorPinNumber, 0);
  }
}

/*There is a minumum power to which is needed to start the motor*/
void StartMotor(int hapticMotorPinNumber, int motorSpeed) {
  Serial.print("Start Vibration: ");
  analogWrite(hapticMotorPinNumber, (motorSpeed, 1, 100, minVibrateSpeed, maxVibrateSpeed));
}

void StopMotor(int hapticMotorPinNumber)
{
  analogWrite(hapticMotorPinNumber, 0);
  Serial.println("Stop Vibration");
}

void ButtonFeedback(int hapticMotorPinNumber, int pulseCount) {
  analogWrite(hapticMotorPinNumber, maxVibrateSpeed);
  vTaskDelay(buttonFeedBackLenght / portTICK_PERIOD_MS);
  analogWrite(hapticMotorPinNumber, 0);
  Serial.println("Button Feedback");
}
