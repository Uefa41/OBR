#include "rgb.h"
#include <Arduino.h>

Rgb::Rgb(byte pinS0, byte pinS1, byte pinS2, byte pinS3, byte pinOut)
    : pinS0(pinS0), pinS1(pinS1), pinS2(pinS2), pinS3(pinS3), pinOut(pinOut) {}

void Rgb::begin() {
  pinMode(pinS0, OUTPUT);
  pinMode(pinS1, OUTPUT);
  pinMode(pinS2, OUTPUT);
  pinMode(pinS3, OUTPUT);
  pinMode(pinOut, INPUT);

  digitalWrite(pinS0, HIGH);
  digitalWrite(pinS1, LOW);
}

int Rgb::getChannel(Channel channel) {
  int period;

  digitalWrite(pinS2, (channel & 1 << 1) ? HIGH : LOW);
  digitalWrite(pinS3, (channel & 1) ? HIGH : LOW);

  period = pulseIn(pinOut, digitalRead(pinOut) == LOW ? HIGH : LOW);
  return period;
}

int *Rgb::getRgbState() {
  static int state[4];

  for (uint8_t i = 0; i < 4; i++) {
    state[i] = getChannel((Channel)i);
  }

  return state;
}

int (*Rgb::getRgbRange(int iterations))[2] {
  int *state;
  static int(*range)[2];

  state = getRgbState();

  for (uint8_t i = 0; i < 4; i++) {
    range[0][i] = state[i];
    range[1][i] = state[i];
  }

  for (int i = 0; i < iterations - 1; i++) {
    state = getRgbState();
    for (uint8_t j = 0; j < 4; i++) {
      range[0][j] = min(range[0][j], state[j]);
      range[1][j] = max(range[1][j], state[j]);
    }

    delay(1);
  }

  return range;
}

bool Rgb::inRange(int *state, int (*range)[2], int margin) {
  bool res = true;

  for (uint8_t i = 0; i < 4; i++) {
    res = res && (state[i] >= range[0][i] && state[i] <= range[1][i]);
  }

  return res;
}

bool Rgb::currentInRange(int (*range)[2], int margin) {
  return inRange(getRgbState(), range, margin);
}
