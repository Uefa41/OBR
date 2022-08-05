#ifndef RGB_H
#define RGB_H

#include <Arduino.h>

class Rgb {
private:
  const byte pinS0, pinS1, pinS2, pinS3, pinOut;

public:
  enum Channel {
    RED,
    BLUE,
    REF,
    GREEN,
  };

  Rgb(byte pinS0, byte pinS1, byte pinS2, byte pinS3, byte pinOut);

  void begin();

  int getChannel(Channel channel);

  int *getRgbState();

  int (*getRgbRange(int iterations))[2];

  bool inRange(int *state, int (*range)[2], int margin);

  bool currentInRange(int (*range)[2], int margin);
};

#endif
