#include "button.h"
#include "gyro.h"
#include "control.h"
#include "motors.h"
#include "pid.h"
#include "pins.h"
#include "rgb.h"
#include "traction.h"

/// Constants
const int GREEN_MARGIN = 7;
const int BLACK_MARGIN = 2;
const int MAX_SPEED = 255;
const int MIN_SPEED = 100;
const int BASE_SPEED = 120;

const float KP = 1.2;
const float KI = 0.01;
const float KD = 0.1;
const float MAX_I = 100;
const float SR = 0.6;

const int ROT_90 = 1000;

/// Components

// RGB sensors
Rgb rgbLeft(RGB_L_S0, RGB_L_S1, RGB_L_S2, RGB_L_S3, RGB_L_OUT);
Rgb rgbRight(RGB_R_S0, RGB_R_S1, RGB_R_S2, RGB_R_S3, RGB_R_OUT);

// Motors
Motor motorLeft(MOTOR_L_IN1, MOTOR_L_IN2);
Motor motorRight(MOTOR_R_IN1, MOTOR_R_IN2);
Traction traction(motorLeft, motorRight, MIN_SPEED, MAX_SPEED, BASE_SPEED);

//PID
Pid colorPid(KP, KI, KD, MAX_I);

// Gyro
Gyro gyro(GYRO, ROT_90);

// Button
Button button(BUTTON);

// Control
Control control(traction, rgbLeft, rgbRight, colorPid, gyro, button);

void setup() {
  Serial.begin(9600);

  rgbLeft.begin();
  rgbRight.begin();

  traction.begin();

  control.calibrate();
}

void loop() {
  control.followLine(BASE_SPEED, SR);

  control.turnGreen(GREEN_MARGIN, BLACK_MARGIN);
}
