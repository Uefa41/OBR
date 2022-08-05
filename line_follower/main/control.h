#ifndef CONTROL_H
#define CONTROL_H

#include "button.h"
#include "gyro.h"
#include "pid.h"
#include "rgb.h"
#include "traction.h"
#include <Arduino.h>

class Control {
private:
  Traction &traction;

  Rgb &rgbLeft;
  Rgb &rgbRight;

  Pid &colorPid;

  Gyro &gyro;

  Button &button;

  int (*rangeGreen)[2];
  int (*rangeBlack)[2];

public:
  enum GreenPattern {
    NONE,
    LEFT,
    RIGHT,
    BOTH,
  };

  Control(Traction &traction, Rgb &rgbLeft, Rgb &rgbRight, Pid &colorPid,
          Gyro &gyro, Button &button);

  void followLine(int baseSpeed, float speedReduc);

  void turnGyro(int rot, int speed);

  void turnGreen(int greenMargin, int blackMargin);

  GreenPattern checkGreen(int greenMargin);

  bool checkBlack(GreenPattern pattern, int blackMargin);

  void calibrate();
};

#endif
