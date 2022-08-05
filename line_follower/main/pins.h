#ifndef PINS_H
#define PINS_H

#include <Arduino.h>

enum Pins {
  /// Ultrasonic
  // Front
  US_F_ECHO = 40,
  US_F_TRIG = 41,

  // Left
  US_L_ECHO = 27,
  US_L_TRIG = 26,

  // Right
  US_R_ECHO = 24,
  US_R_TRIG = 25,

  /// RGB sensors
  // Left
  RGB_L_S0 = 51,
  RGB_L_S1 = 50,
  RGB_L_S2 = 52,
  RGB_L_S3 = 53,
  RGB_L_OUT = 38,

  // Right
  RGB_R_S0 = 42,
  RGB_R_S1 = 43,
  RGB_R_S2 = 44,
  RGB_R_S3 = 45,
  RGB_R_OUT = 39,

  /// Motors
  // Left
  MOTOR_L_IN1 = 13,
  MOTOR_L_IN2 = 12,

  // Right
  MOTOR_R_IN1 = 10,
  MOTOR_R_IN2 = 11,

  /// Button
  BUTTON = 32,

  /// LED
  LED_R = 31,
  LED_G = 28,
  LED_B = 30,

  /// Buzzer
  NOISE = 29,

  /// Gyroscope
  GYRO = A0,
};

#endif
