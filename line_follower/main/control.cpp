#include "control.h"
#include "gyro.h"
#include "pid.h"
#include "rgb.h"
#include "traction.h"
#include <Arduino.h>

Control::Control(Traction &traction, Rgb &rgbLeft, Rgb &rgbRight, Pid &colorPid,
                 Gyro &gyro, Button &button, NewPing (&sonar)[])
    : traction(traction), rgbLeft(rgbLeft), rgbRight(rgbRight),
      colorPid(colorPid), gyro(gyro), button(button), sonar(sonar){};

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

void Control::checkUs() {
  if (millis() - usTimer >= 50) {
    usDistance[usId] = sonar[usId].ping_cm();

    usId = (usId + 1) % 3;
    usTimer = millis();
  }
}

void Control::checkObstacle() {}

void Control::dodgeObstacle(bool right, int obstacleDistance, int blackMargin) {
  alignObstacle();

  usDistance[0] = sonar[0].ping_cm();
  while (usDistance[0] < obstacleDistance) {
    traction.backwards(traction.minSpeed);
    usDistance[0] = sonar[0].ping_cm();
    delay(50);
  }

  turnGyro(gyro.ROT_90, traction.maxSpeed * (right ? -1 : 1));
  traction.forward(traction.maxSpeed);
  delay(400);

  turnGyro(gyro.ROT_90, traction.maxSpeed * (right ? 1 : -1));
  traction.forward(traction.maxSpeed);
  delay(950);

  turnGyro(gyro.ROT_90, traction.maxSpeed * (right ? 1 : -1));
  traction.forward(traction.maxSpeed);

  unsigned long timer = millis();

  while (millis() - timer < 700) {
    if (rgbLeft.currentInRange(rangeBlack, blackMargin) ||
        rgbRight.currentInRange(rangeBlack, blackMargin)) {
      break;
    }
  }
  delay(200);

  turnGyro(gyro.ROT_90, traction.maxSpeed * (right ? -1 : 1));

  traction.backwards(traction.maxSpeed);
  delay(300);
}

void Control::alignObstacle() {
  traction.stop();

  unsigned long usValue = sonar[0].ping_median(10);

  traction.spin(traction.maxSpeed);
  delay(50);
  traction.stop();

  unsigned long lastUsValue = usValue;
  usValue = sonar[0].ping_median(10);

  if (usValue == lastUsValue) {
    return;
  }

  if (usValue < lastUsValue) {
    while (usValue < lastUsValue) {
      traction.spin(traction.maxSpeed);
      delay(50);
      traction.stop();

      lastUsValue = usValue;
      usValue = sonar[0].ping_median(10);
    }

    return;
  }

  if (usValue > lastUsValue) {
    traction.spin(-traction.maxSpeed);
    delay(50);
    traction.stop();

    lastUsValue = usValue;
    usValue = sonar[0].ping_median(10);

    while (usValue < lastUsValue) {
      traction.spin(-traction.maxSpeed);
      delay(50);
      traction.stop();

      lastUsValue = usValue;
      usValue = sonar[0].ping_median(10);
    }

    traction.stop();
    return;
  }
}
