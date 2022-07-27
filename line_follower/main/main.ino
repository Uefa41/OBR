#include <Wire.h>
#include <NewPing.h>
#include <MPU6050_light.h>

/// Structs
typedef struct rgb_sensor {
  unsigned short S0, S1, S2, S3, OUT;
} rgb_sensor;

typedef struct rgb_values {
  unsigned long red, green, blue, ref;
} rgb_values;

typedef struct motor {
  int A, B, SPEED;
} motor;

typedef struct ultrasonic {
    unsigned short echo, trig;
} ultrasonic;

/// Pins

// Ultrassonic sensor
ultrasonic US = {
    .echo = 40,
    .trig = 41,
  };

//RGB sensors
const rgb_sensor RGB[] = {
    // Left RGB
    {
        .S0 = 51,
        .S1 = 50,
        .S2 = 52,
        .S3 = 53,
        .OUT = 38,
    },
    // Right RGB
    {
        .S0 = 42,
        .S1 = 43,
        .S2 = 44,
        .S3 = 45,
        .OUT = 39,
    },
};

//Motors
const motor MOTORS[] = {
    // Left Motor
    {
        .A = 36,
        .B = 35,
        .SPEED = 9,
    },
    // Right motor
    {
        .A = 34,
        .B = 33,
        .SPEED = 10,
    },
};

// Gyroscope

const int MPU = 0x68;

// Button
const int BUTTON = 32;

/// Constants
const int MAX_SPEED = 200;
const int MIN_SPEED = 100;
const int BASE_SPEED = 110;

const float KP = 1.5;
const float KI = 0.02;
const float KD = 3.5;
const float MAX_I = 100;
const float SR = 0.7;

const float GYRO_90 = 90;
const float GYRO_180 = 180;
/* const long TIME_90 = 500; */
/* const long TIME_180 = 1000; */

const bool DO_GREEN = false;
const int MARGIN_OF_ERROR_GREEN = 15;
const int MARGIN_OF_ERROR_BLACK = 8;

const bool DO_OBSTACLE = true;
const float OBSTACLE_DISTANCE = 10.0;

/// Variables
MPU6050 mpu(Wire);

NewPing sonar(US.trig, US.echo, 100);

rgb_values rgb_sensor_values[2];

rgb_values green_range[2], black_range[2];

int motor_speed[2];

float error, lastError, P, I, D;

int PID;

unsigned long time, lastTime;

float rot;

unsigned int usDistance;
float lastUltrasonic;
float ultrasonicValue;

/// Setup
void setup() {
    Serial.begin(9600);

    // Ultrassonic
    pinMode(US.echo, INPUT);
    pinMode(US.trig, OUTPUT);

    // Gyroscope
    Wire.begin();
    mpu.begin();

  // RGB
  for (int i = 0; i < 2; i++) {
    pinMode(RGB[i].S0, OUTPUT);
    pinMode(RGB[i].S1, OUTPUT);
    pinMode(RGB[i].S2, OUTPUT);
    pinMode(RGB[i].S3, OUTPUT);
    pinMode(RGB[i].OUT, INPUT);

    digitalWrite(RGB[i].S0, HIGH);
    digitalWrite(RGB[i].S1, LOW);
  }

  // Motors
  for (int i = 0; i < 2; i++) {
    pinMode(MOTORS[i].A, OUTPUT);
    pinMode(MOTORS[i].B, OUTPUT);
    pinMode(MOTORS[i].SPEED, OUTPUT);
  }

  pinMode(BUTTON, INPUT);

  while (digitalRead(BUTTON) == LOW);

  mpu.calcGyroOffsets();

  motors_spin(MAX_SPEED);
  delay(30);
  motors_stop();

  while (digitalRead(BUTTON) == HIGH);
  while (digitalRead(BUTTON) == LOW);

  if (DO_GREEN) {
    calibrate(black_range, MARGIN_OF_ERROR_BLACK);

    motors_spin(MAX_SPEED);
    delay(30);
    motors_stop();

    while (digitalRead(BUTTON) == HIGH);
    while (digitalRead(BUTTON) == LOW);

    calibrate(green_range, MARGIN_OF_ERROR_GREEN);

    motors_spin(MAX_SPEED);
    delay(30);
    motors_stop();

    while (digitalRead(BUTTON) == HIGH);
    while (digitalRead(BUTTON) == LOW);
  }
}

void loop() {
  for (int i = 0; i < 2; i++) {
    get_rgb(i);
  }

  if (DO_GREEN) {
    turn_green();
  }

  usDistance = sonar.ping_cm();
  if (DO_OBSTACLE && usDistance > 0 && usDistance < OBSTACLE_DISTANCE) {
    motors_stop();
    delay(100);
    usDistance = sonar.ping_cm();

    if (usDistance > 0 && usDistance < OBSTACLE_DISTANCE) {
      divert_obstacle(false);
    }
  }

  if (digitalRead(BUTTON) == HIGH) {
    motors_stop();
    while (digitalRead(BUTTON) == HIGH);
    while (digitalRead(BUTTON) == LOW);
    while (digitalRead(BUTTON) == HIGH);
  }

  pid_turn(BASE_SPEED, false);
}
