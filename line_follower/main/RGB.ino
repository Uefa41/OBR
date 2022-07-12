void get_rgb(int id) {
  digitalWrite(RGB[id].S2, LOW);
  digitalWrite(RGB[id].S3, LOW);

  if (digitalRead(RGB[id].OUT) == LOW) {
    rgb_sensor_values[id].red = pulseIn(RGB[id].OUT, HIGH);
  } else {
    rgb_sensor_values[id].red = pulseIn(RGB[id].OUT, LOW);
  }

  digitalWrite(RGB[id].S2, HIGH);

  if (digitalRead(RGB[id].OUT) == LOW) {
    rgb_sensor_values[id].ref = pulseIn(RGB[id].OUT, HIGH);
  } else {
    rgb_sensor_values[id].ref = pulseIn(RGB[id].OUT, LOW);
  }

  digitalWrite(RGB[id].S2, LOW);
  digitalWrite(RGB[id].S3, HIGH);

  if (digitalRead(RGB[id].OUT) == LOW) {
    rgb_sensor_values[id].blue = pulseIn(RGB[id].OUT, HIGH);
  } else {
    rgb_sensor_values[id].blue = pulseIn(RGB[id].OUT, LOW);
  }

  digitalWrite(RGB[id].S2, HIGH);

  if (digitalRead(RGB[id].OUT) == LOW) {
    rgb_sensor_values[id].green = pulseIn(RGB[id].OUT, HIGH);
  } else {
    rgb_sensor_values[id].green = pulseIn(RGB[id].OUT, LOW);
  }
}

bool rgb_in_range(rgb_values range[], int id) {
  bool red = (rgb_sensor_values[id].red >= range[0].red && rgb_sensor_values[id].red <= range[1].red);
  bool green = (rgb_sensor_values[id].green >= range[0].green && rgb_sensor_values[id].green <= range[1].green);
  bool blue = (rgb_sensor_values[id].blue >= range[0].blue && rgb_sensor_values[id].blue <= range[1].blue);
  bool ref = (rgb_sensor_values[id].ref >= range[0].ref && rgb_sensor_values[id].ref <= range[1].ref);

  return red && green && blue && ref;
}
