#include "button.h"
#include <Arduino.h>

Button::Button(byte pin) : pin(pin) {}

void Button::begin() { pinMode(pin, INPUT); }

void Button::update() {
  bool newReading = digitalRead(pin);

  if (newReading != lastReading) {
    time = millis();
  }

  if (millis() - time >= minTime) {
    state = newReading;
  }

  lastReading = newReading;
}

bool Button::getState() {
  update();

  return state;
}

void Button::waitPress() {
  while (!getState())
    ;
  while (getState())
    ;
}
