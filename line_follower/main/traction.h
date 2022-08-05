#ifndef TRACTION_H
#define TRACTION_H

#include "motors.h"
#include <Arduino.h>

class Traction {
private:
  Motor &motorLeft;
  Motor &motorRight;

public:
  const int maxSpeed, minSpeed, baseSpeed;

  Traction(Motor &motorLeft, Motor &motorRight, int minSpeed, int maxSpeed,
           int baseSpeed);

  void begin();

  void turnDirection(int direction, int baseSpeed);

  void forward(int speed);

  void backwards(int speed);

  void spin(int speed);

  void stop();

  void brake();
};

#endif
