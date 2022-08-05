#ifndef GYRO_H
#define GYRO_H

#include <Arduino.h>

class Gyro {
private:
  const byte pin;

  unsigned long time;

  int zeroSpeed;

  float rot = 0;

public:
  const int ROT_90;

  Gyro(byte pin, int rot90);

  void calibrate(int iterations);

  void reset();

  int getSpeed();

  float getRot();
};

#endif
