#ifndef KOTIOT_HOMEOFFICE_H
#define KOTIOT_HOMEOFFICE_H

void Pulse(int hapticMotorPinNumber, int pulseCount);
void ClimbAndDown(int hapticMotorPinNumber, int climbCount);
void Climb(int hapticMotorPinNumber, int climbCount);
void GoDown(int hapticMotorPinNumber, int downCount);
void StartMotor(int hapticMotorPinNumberint, int motorSpeed);
void StopMotor(int hapticMotorPinNumber);
void ButtonFeedback(int hapticMotorPinNumber, int pulseCount);

#endif // KOTIOT_HOMEOFFICE_H
