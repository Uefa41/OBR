// True for right, false for left;
void divert_obstacle(bool side) {
  int rotate_speed = MAX_SPEED;
  if (side) {
    rotate_speed = -rotate_speed;
  }

  /* align_obstacle(); */

  usDistance = sonar.ping_cm();
  while (usDistance < OBSTACLE_DISTANCE) {
    go_back(MIN_SPEED);
    usDistance = sonar.ping_cm();
    delay(50);
  }

  /* motors_rotate(GYRO_90, -rotate_speed); */
  motors_rotate_time(TIME_90, -rotate_speed);
  motors_turn(0, MAX_SPEED, false);
  delay(400);

  /* motors_rotate(GYRO_90, rotate_speed); */
  motors_rotate_time(TIME_90, rotate_speed);
  motors_turn(0, MAX_SPEED, false);
  delay(800);

  /* motors_rotate(GYRO_90, rotate_speed); */
  motors_rotate_time(TIME_90, rotate_speed);
  motors_turn(0, BASE_SPEED, false);
  
  /* while ((! rgb_in_range(black_range, 0)) && (! rgb_in_range(black_range, 1))) { */
  /*   get_rgb(0); */
  /*   get_rgb(1); */
  /* } */
  delay(500);

  /* motors_rotate(GYRO_90, -rotate_speed); */
  motors_rotate_time(TIME_90, -rotate_speed);

  go_back(MAX_SPEED);
  delay(300);
}

void align_obstacle() {
  motors_stop();
  ultrasonicValue = sonar.ping_median(10);
  motors_spin(MAX_SPEED);
  delay(80);
  motors_stop();
  lastUltrasonic = ultrasonicValue;
  ultrasonicValue = sonar.ping_median(10);

  if (ultrasonicValue == lastUltrasonic) {
    return;
  }

  if (ultrasonicValue - lastUltrasonic < 0 ) {
    while(ultrasonicValue - lastUltrasonic < 0) {
      motors_spin(MAX_SPEED);
      delay(80);
      motors_stop();
      lastUltrasonic = ultrasonicValue;
      ultrasonicValue = sonar.ping_median(10);
    }

    return;
  }

  if (ultrasonicValue - lastUltrasonic > 0) {
    motors_spin(-MAX_SPEED);
    delay(80);
    motors_stop();
    lastUltrasonic = ultrasonicValue;
    ultrasonicValue = sonar.ping_median(10);

    while(ultrasonicValue - lastUltrasonic < 0) {
      motors_spin(-MAX_SPEED);
      delay(80);
      motors_stop();
      lastUltrasonic = ultrasonicValue;
      ultrasonicValue = sonar.ping_median(10);
    }

    motors_stop();
    return;
  }
}
