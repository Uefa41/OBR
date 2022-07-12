void pid_turn(int base_speed, bool backwards) {
  for (int i = 0; i < 2; i++) {
    get_rgb(i);
  }

  error = pow(rgb_sensor_values[1].ref, 2) - pow(rgb_sensor_values[0].ref, 2);

  /* error *= K; */

  P = KP * error;
  I += KI * error;
  D = KD * (error - lastError);

  I = min(I, MAX_I);
  I = max(I, -MAX_I);

  lastError = error;

  PID = floor(P + I + D);

  motors_turn(PID, base_speed - SR * error, backwards);
}
