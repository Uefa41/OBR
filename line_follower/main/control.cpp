#include "control.h"
#include "gyro.h"
#include "pid.h"
#include "rgb.h"
#include "traction.h"
#include <Arduino.h>

Control::Control(Traction &traction, Rgb &rgbLeft, Rgb &rgbRight, Pid &colorPid,
                 Gyro &gyro, Button &button)
    : traction(traction), rgbLeft(rgbLeft), rgbRight(rgbRight),
      colorPid(colorPid), gyro(gyro), button(button){};

void Control::followLine(int baseSpeed, float speedReduct) {
  float error = pow(rgbRight.getChannel(Rgb::REF), 2) -
                pow(rgbLeft.getChannel(Rgb::REF), 2);

  int direction = colorPid.output(error);

  traction.turnDirection(direction, baseSpeed - abs(direction) * speedReduct);
}

void Control::turnGyro(int rot, int speed) {
  gyro.reset();
  traction.spin(speed);

  while (abs(gyro.getRot()) < rot)
    ;
}

void Control::turnGreen(int greenMargin, int blackMargin) {
  GreenPattern pattern = checkGreen(greenMargin);

  if (checkBlack(pattern, blackMargin)) {
    traction.forward(traction.baseSpeed);
    delay(150);

    switch (pattern) {
    case LEFT:
      turnGyro(gyro.ROT_90, traction.maxSpeed);
      break;

    case RIGHT:
      turnGyro(gyro.ROT_90, -traction.maxSpeed);
      break;

    case BOTH:
      turnGyro(2 * gyro.ROT_90, -traction.maxSpeed);
      break;

    case NONE:
      return;
      break;
    }

    traction.backwards(traction.baseSpeed);
    delay(300);
  }
}

Control::GreenPattern Control::checkGreen(int greenMargin) {
  if (rgbLeft.currentInRange(rangeGreen, greenMargin)) {
    unsigned long time = millis();

    traction.forward(traction.baseSpeed);

    while (millis() - time < 50) {
      if (rgbRight.currentInRange(rangeGreen, greenMargin)) {
        return BOTH;
      }
    }

    return LEFT;
  } else if (rgbRight.currentInRange(rangeGreen, greenMargin)) {
    unsigned long time = millis();

    traction.forward(traction.baseSpeed);

    while (millis() - time < 50) {
      if (rgbLeft.currentInRange(rangeGreen, greenMargin)) {
        return BOTH;
      }
    }

    return RIGHT;
  }

  return NONE;
}

bool Control::checkBlack(GreenPattern pattern, int blackMargin) {
  unsigned long time = millis();

  traction.forward(traction.baseSpeed);

  while (millis() - time < 200) {
    switch (pattern) {
    case BOTH:
      if (rgbLeft.currentInRange(rangeBlack, blackMargin) ||
          rgbRight.currentInRange(rangeBlack, blackMargin)) {
        return true;
      }
      break;

    case LEFT:
      if (rgbLeft.currentInRange(rangeBlack, blackMargin)) {
        return true;
      }
      break;

    case RIGHT:
      if (rgbRight.currentInRange(rangeBlack, blackMargin)) {
        return true;
      }
      break;

    case NONE:
      return false;
    }
  }

  return false;
}

void Control::calibrate() {
  button.waitPress();
  gyro.calibrate(1000);

  button.waitPress();
  rangeGreen = rgbLeft.getRgbRange(1000);

  button.waitPress();
  rangeBlack = rgbLeft.getRgbRange(1000);

  button.waitPress();
}
