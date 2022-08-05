#include "pid.h"
#include <Arduino.h>

Pid::Pid(float kP, float kI, float kD, float maxI)
    : kP(kP), kI(kI), kD(kD), maxI(maxI) {}

int Pid::output(float error) {
  P = kP * error;
  I += kI * error;
  D = kD * (error - lastError);

  I = min(I, maxI);
  I = max(I, -maxI);

  lastError = error;

  return round(P + I + D);
}
