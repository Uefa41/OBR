// Output in centimeters
float us_distance() {
  digitalWrite(US.trig, LOW);
  delayMicroseconds(2);
  digitalWrite(US.trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(US.trig, LOW);

  unsigned long us_duration = pulseIn(US.echo, HIGH);

  return 340 * ((float) (us_duration) / 20000);
}

// True for right, false for left;
void divert_obstacle(bool side) {
  int rotate_speed = MAX_SPEED;
  if (side) {
    rotate_speed = -rotate_speed;
  }

  motors_rotate(GYRO_90, -rotate_speed);
  motors_turn(0, MAX_SPEED, false);
  delay(400);

  motors_rotate(GYRO_90, rotate_speed);
  motors_turn(0, MAX_SPEED, false);
  delay(1000);

  motors_rotate(GYRO_90, rotate_speed);
  motors_turn(0, MAX_SPEED, false);
  
  while (! rgb_in_range(black_range, 0) && ! rgb_in_range(black_range, 1)) {
    get_rgb(0);
    get_rgb(1);
  }

  delay(100);
  motors_rotate(GYRO_90, -rotate_speed);
}
