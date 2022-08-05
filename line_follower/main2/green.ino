void turn_green() {
  int green_id = check_green(); 
  if (green_id && check_black(green_id - 1)) {
    motors_turn(0, BASE_SPEED, false);
    delay(150);

    switch (green_id) {
      case 1:
        /* motors_rotate(GYRO_90, -MAX_SPEED); */
        motors_rotate_time(TIME_90, -MAX_SPEED);
        go_back(BASE_SPEED);
        delay(300);
        break;

      case 2:
        /* motors_rotate(GYRO_90, MAX_SPEED); */
        motors_rotate_time(TIME_90, MAX_SPEED);
        go_back(BASE_SPEED);
        delay(300);
        break;

      case 3:
        /* motors_rotate(GYRO_180, MAX_SPEED); */
        motors_rotate_time(TIME_90, MAX_SPEED);
        motors_spin(MAX_SPEED);
        
        lastTime = time;
        time = millis();
        while(millis() - time < TIME_180) {
          get_rgb(0);
          get_rgb(1);

          if (rgb_in_range(black_range_obs, 0) || rgb_in_range(black_range_obs, 1)) {
            break;
          }
        }
        go_back(BASE_SPEED);
        digitalWrite(NOISE, HIGH);
        delay(200);
        digitalWrite(NOISE, LOW);
        break;
    }

    motors_stop();
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

    while (millis() - time < 80) {
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

    while ( millis() - time < 80) {
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

  while (millis() - time < 200) {
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
