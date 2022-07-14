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
