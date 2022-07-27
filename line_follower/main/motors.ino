void motors_turn(int direction, int base_speed, bool backwards) {
  if (backwards) {
    direction = - direction;
  }
  motor_speed[0] = base_speed + direction;
  motor_speed[1] = base_speed - direction;

  for (int i = 0; i < 2; i++) {
    motor_speed[i] = min(MAX_SPEED, motor_speed[i]);
    motor_speed[i] = max(-MAX_SPEED, motor_speed[i]);

    if (backwards) {
      motor_speed[i] = - motor_speed[i];
    }

    if (motor_speed[i] >= 0) {
      digitalWrite(MOTORS[i].A, HIGH);
      digitalWrite(MOTORS[i].B, LOW);
      motor_speed[i] = motor_speed[i] * (MAX_SPEED - MIN_SPEED) / MAX_SPEED + MIN_SPEED;
    } else {
      digitalWrite(MOTORS[i].A, LOW);
      digitalWrite(MOTORS[i].B, HIGH);
      motor_speed[i] = motor_speed[i] * (MAX_SPEED - MIN_SPEED) / MAX_SPEED - MIN_SPEED;
    }

    analogWrite(MOTORS[i].SPEED, abs(motor_speed[i]));
  }
}

void go_back(int speed) {
  for (int i = 0; i < 2; i++) {
    digitalWrite(MOTORS[i].A, LOW);
    digitalWrite(MOTORS[i].B, HIGH);

    analogWrite(MOTORS[i].SPEED, speed);
  }
}

void motors_spin(int speed) {
    if (speed >= 0) {
      digitalWrite(MOTORS[0].A, HIGH);
      digitalWrite(MOTORS[0].B, LOW);

      digitalWrite(MOTORS[1].A, LOW);
      digitalWrite(MOTORS[1].B, HIGH);
    } else {
      digitalWrite(MOTORS[0].A, LOW);
      digitalWrite(MOTORS[0].B, HIGH);

      digitalWrite(MOTORS[1].A, HIGH);
      digitalWrite(MOTORS[1].B, LOW);
    }

    analogWrite(MOTORS[0].SPEED, abs(speed));
    analogWrite(MOTORS[1].SPEED, abs(speed));
}

void motors_stop() {
  digitalWrite(MOTORS[0].A, LOW);
  digitalWrite(MOTORS[0].B, LOW);
  digitalWrite(MOTORS[1].A, LOW);
  digitalWrite(MOTORS[1].B, LOW);
}

void motors_rotate(float rotation, int speed) {
  mpu.update();
  rot = mpu.getAngleZ();
  
  motors_spin(speed);
  
  while (abs(mpu.getAngleZ() - rot) < rotation) {
    mpu.update();
  }
}

void motors_rotate_time(long rotation, int speed) {
  motors_spin(speed);
  delay(rotation);
  motors_stop();
}
