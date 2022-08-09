#ifndef CONTROL_H
#define CONTROL_H

#include "button.h"
#include "gyro.h"
#include "pid.h"
#include "rgb.h"
#include "traction.h"
#include <Arduino.h>
#include <NewPing.h>

class Control {
private:
  Traction &traction;

  Rgb &rgbLeft;
  Rgb &rgbRight;

  Pid &colorPid;

  Gyro &gyro;

  Button &button;

  NewPing (&sonar)[];

  int (*rangeGreen)[2];
  int (*rangeBlack)[2];

  int usDistance[3];
  uint8_t usId = 0;
  unsigned long usTimer = 0;

public:
  enum GreenPattern {
    NONE,
    LEFT,
    RIGHT,
    BOTH,
  };

  Control(Traction &traction, Rgb &rgbLeft, Rgb &rgbRight, Pid &colorPid,
          Gyro &gyro, Button &button, NewPing (&sonar)[]);

  void followLine(int baseSpeed, float speedReduc);

  void turnGyro(int rot, int speed);

  void turnGreen(int greenMargin, int blackMargin);

  GreenPattern checkGreen(int greenMargin);

  bool checkBlack(GreenPattern pattern, int blackMargin);

  void calibrate();

  void checkUs();

  void checkObstacle();

  void dodgeObstacle(bool right, int obstacleDistance, int blackMargin);

  void alignObstacle();
};

#endif
