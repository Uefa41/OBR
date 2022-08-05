#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>

class Motor {
private:
  const byte pin1, pin2;

public:
  Motor(byte in1, byte in2);

  void begin();

  void spin(int speed);

  void stop();

  void brake();
};

#endif
