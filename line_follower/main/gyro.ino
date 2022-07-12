void get_gyro() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);

  Wire.requestFrom(MPU, 14, true);

  gyro.AcY = Wire.read() << 8 | Wire.read();
  gyro.AcX = Wire.read() << 8 | Wire.read();
  gyro.AcZ = Wire.read() << 8 | Wire.read();
  gyro.Tmp = Wire.read() << 8 | Wire.read();
  gyro.GyX = Wire.read() << 8 | Wire.read();
  gyro.GyY = Wire.read() << 8 | Wire.read();
  gyro.GyZ = Wire.read() << 8 | Wire.read();
}
