void turn_green() {
  int green_id = check_green(); 
  if (green_id && check_black(green_id - 1)) {
    motors_turn(0, BASE_SPEED, false);
    delay(200);

    rot = 0;

    if (green_id > 1) {
      motors_spin(MAX_SPEED);
    } else if ( green_id == 1) {
      motors_spin(-MAX_SPEED);
    }

    do {
      lastTime = time;
      time = millis();

      get_gyro();

      rot += gyro.GyZ / 1000 * (int) (time - lastTime);
    } while (abs(rot) < ((green_id == 3) ? GYRO_180 : GYRO_90));
  }
}

/// Output:
// 0: no green
// 1: left green
// 2: right green
// 3: both green
int check_green() {
  if (rgb_in_range(green_range, 0)) {
    lastTime = time;
    time = millis();

    motors_turn(0, BASE_SPEED, false);

    while (millis() - time < 50) {
      get_rgb(1);
      
      if (rgb_in_range(green_range, 1)) {
        return 3;
      }
    }

    return 1;
  } else if (rgb_in_range(green_range, 1)) {
    lastTime = time;
    time = millis();

    motors_turn(0, BASE_SPEED, false);

    while ( millis() - time < 50) {
      get_rgb(0);

      if(rgb_in_range(green_range, 0)) {
        return 3;
      }
    }

    return 2;
  }

  return 0;
}

/// Note:
// 2 means both sensors
bool check_black(int sensor_id) {
  lastTime = time;
  time = millis();

  motors_turn(0, BASE_SPEED, false);
  delay(10);

  while (millis() - time < 100) {
    if (sensor_id == 2) {
      get_rgb(0);
      get_rgb(1);

      if (rgb_in_range(black_range, 0) || rgb_in_range(black_range, 1)) {
        return true;
      }
    } else {
      get_rgb(sensor_id);

      if(rgb_in_range(black_range, sensor_id)) {
        return true;
      }
    }
  }

  return false;
}
