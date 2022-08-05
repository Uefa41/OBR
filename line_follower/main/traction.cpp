#include "traction.h"
#include "motors.h"
#include <Arduino.h>

Traction::Traction(Motor &motorLeft, Motor &motorRight, int minSpeed,
                   int maxSpeed, int baseSpeed)
    : motorLeft(motorLeft), motorRight(motorRight), minSpeed(minSpeed),
      maxSpeed(maxSpeed), baseSpeed(baseSpeed) {}

void Traction::begin() {
  motorLeft.begin();
  motorRight.begin();
}

void Traction::turnDirection(int direction, int baseSpeed) {
  int motorSpeed[2];

  motorSpeed[0] = baseSpeed + direction;
  motorSpeed[1] = baseSpeed - direction;

  for (uint8_t i = 0; i < 2; i++) {
    motorSpeed[i] = min(maxSpeed, motorSpeed[i]);
    motorSpeed[i] = max(-maxSpeed, motorSpeed[i]);

    motorSpeed[i] = motorSpeed[i] * (maxSpeed - minSpeed) / maxSpeed +
                    (motorSpeed[i] >= 0 ? minSpeed : -minSpeed);

    motorLeft.spin(motorSpeed[0]);
    motorRight.spin(motorSpeed[1]);
  }
}

void Traction::forward(int speed) {
  motorLeft.spin(speed);
  motorRight.spin(speed);
}

void Traction::backwards(int speed) {
  motorLeft.spin(-speed);
  motorRight.spin(-speed);
}

void Traction::spin(int speed) {
  motorLeft.spin(speed);
  motorRight.spin(-speed);
}

void Traction::stop() {
  motorLeft.stop();
  motorRight.stop();
}

void Traction::brake() {
  motorLeft.brake();
  motorRight.brake();
}
