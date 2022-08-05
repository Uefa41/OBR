#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button {
private:
  const byte pin;

  bool state = false, lastReading = false;

  unsigned long time = 0, minTime = 50;

public:
  Button(byte pin);

  void begin();

  void update();

  bool getState();

  void waitPress();
};

#endif
