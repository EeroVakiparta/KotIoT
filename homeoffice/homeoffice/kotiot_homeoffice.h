#ifndef KOTIOT_HOMEOFFICE_H
#define KOTIOT_HOMEOFFICE_H

void Pulse(int hapticMotorPinNumber, int pulseCount);
void Climb(int hapticMotorPinNumber, int climbCount);
void StopMotor(int hapticMotorPinNumber);
void buttonFeedback(int hapticMotorPinNumber, int pulseCount);

#endif // KOTIOT_HOMEOFFICE_H
