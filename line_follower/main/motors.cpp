#include "motors.h"
#include <Arduino.h>

Motor::Motor(byte in1, byte in2) : pin1(in1), pin2(in2) {}

void Motor::begin() {
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);

  stop();
}

void Motor::spin(int speed) {
  analogWrite(pin1, max(speed, 0));
  analogWrite(pin2, max(-speed, 0));
}

void Motor::stop() {
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
}

void Motor::brake() {
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, HIGH);
}
