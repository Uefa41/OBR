#include "gyro.h"
#include <Arduino.h>

Gyro::Gyro(byte pin, int rot90) : pin(pin), ROT_90(rot90) {}

void Gyro::calibrate(int iterations) {
  zeroSpeed = 0;

  for (int i = 0; i < iterations; i++) {
    zeroSpeed += analogRead(pin);
  }

  zeroSpeed /= iterations;

  time = millis();
}

void Gyro::reset() {
  rot = 0;

  time = millis();
}

int Gyro::getSpeed() { return analogRead(pin) - zeroSpeed; }

float Gyro::getRot() {
  rot += getSpeed() * ((signed long)(millis() - time));

  time = millis();

  return rot;
}
