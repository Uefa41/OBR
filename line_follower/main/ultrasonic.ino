// True for right, false for left;
void divert_obstacle(bool side) {
  int rotate_speed = MAX_SPEED;
  if (side) {
    rotate_speed = -rotate_speed;
  }

  align_obstacle();

  usDistance[0] = sonar_front.ping_cm();
  while (usDistance[0] < OBSTACLE_DISTANCE) {
    go_back(MIN_SPEED);
    usDistance[0] = sonar_front.ping_cm();
    delay(50);
  }

  /* motors_rotate(GYRO_90, -rotate_speed); */
  motors_rotate_time(TIME_90, -rotate_speed);
  motors_turn(0, MAX_SPEED, false);
  delay(400);

  /* motors_rotate(GYRO_90, rotate_speed); */
  motors_rotate_time(TIME_90, rotate_speed);
  motors_turn(0, MAX_SPEED, false);
  delay(1100);

  /* motors_rotate(GYRO_90, rotate_speed); */
  motors_rotate_time(TIME_90, rotate_speed);
  motors_turn(0, MIN_SPEED, false);
  
  lastTime = time;
  time = millis();

  while (millis() - time < 700) {
    get_rgb(0);
    get_rgb(1);

    if (rgb_in_range(black_range_obs, 0) || rgb_in_range(black_range_obs, 1)) {
      break;
    }
  }
  delay(200);

  /* motors_rotate(GYRO_90, -rotate_speed); */
  motors_rotate_time(TIME_90, -rotate_speed);

  go_back(MAX_SPEED);
  delay(400);
}

void align_obstacle() {
  motors_stop();
  ultrasonicValue = sonar_front.ping_median(10);
  motors_spin(MAX_SPEED);
  delay(50);
  motors_stop();
  lastUltrasonic = ultrasonicValue;
  ultrasonicValue = sonar_front.ping_median(10);

  if (ultrasonicValue == lastUltrasonic) {
    return;
  }

  if (ultrasonicValue - lastUltrasonic < 0 ) {
    while(ultrasonicValue - lastUltrasonic < 0) {
      motors_spin(MAX_SPEED);
      delay(50);
      motors_stop();
      lastUltrasonic = ultrasonicValue;
      ultrasonicValue = sonar_front.ping_median(10);
    }

    return;
  }

  if (ultrasonicValue - lastUltrasonic > 0) {
    motors_spin(-MAX_SPEED);
    delay(50);
    motors_stop();
    lastUltrasonic = ultrasonicValue;
    ultrasonicValue = sonar_front.ping_median(10);

    while(ultrasonicValue - lastUltrasonic < 0) {
      motors_spin(-MAX_SPEED);
      delay(50);
      motors_stop();
      lastUltrasonic = ultrasonicValue;
      ultrasonicValue = sonar_front.ping_median(50);
    }

    motors_stop();
    return;
  }
}
