void calibrate(rgb_values range[], int margin) {
  get_rgb(0);

  range[0] = rgb_sensor_values[0];
  range[1] = rgb_sensor_values[0];
  for (int i = 0; i < 1000; i++) {
    get_rgb(0);
    range[0].red = min(rgb_sensor_values[0].red, range[0].red);
    range[0].green = min(rgb_sensor_values[0].green, range[0].green);
    range[0].blue = min(rgb_sensor_values[0].blue, range[0].blue);
    range[0].ref = min(rgb_sensor_values[0].ref, range[0].ref);

    range[1].red = max(rgb_sensor_values[0].red, range[1].red);
    range[1].green = max(rgb_sensor_values[0].green, range[1].green);
    range[1].blue = max(rgb_sensor_values[0].blue, range[1].blue);
    range[1].ref = max(rgb_sensor_values[0].ref, range[1].ref);

    delay(1);
  }

  range[0].red = max(0, range[0].red - margin);
  range[0].green = max(0, range[0].green - margin);
  range[0].blue = max(0, range[0].blue - margin);
  range[0].ref = max(0, range[0].ref - margin);
  
  range[1].red += margin;
  range[1].green += margin;
  range[1].blue += margin;
  range[1].ref += margin;
}
