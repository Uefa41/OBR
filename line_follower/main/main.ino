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

typedef struct led {
  int R, G, B;
} led;

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

// LED
const led LED = {
  .R = 31,
  .G = 28,
  .B = 30,
};

/// Constants
const int MAX_SPEED = 200;
const int MIN_SPEED = 100;
const int BASE_SPEED = 110;

const float KP = 1.2;
const float KI = 0.01;
const float KD = 2.5;
const float MAX_I = 100;
const float SR = 0.6;

const float GYRO_90 = 90;
const float GYRO_180 = 180;
const long TIME_90 = 500;
const long TIME_180 = 1000;

const bool DO_GREEN = true;
const int MARGIN_OF_ERROR_GREEN = 9;
const int MARGIN_OF_ERROR_BLACK = 8;

const bool DO_OBSTACLE = true;
const float OBSTACLE_DISTANCE = 3.0;

/// Variables
MPU6050 mpu(Wire);

NewPing sonar(US.trig, US.echo, 100);

rgb_values rgb_sensor_values[2];

rgb_values green_range[2], black_range[2];

int motor_speed[2];

float error, lastError, P, I, D;

int PID;

unsigned long PIDTimer;

unsigned long time, lastTime;

float rot;

unsigned long pingTimer;
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
    Wire.setWireTimeout(3000, true);
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
  
  pinMode(LED.R, OUTPUT);
  pinMode(LED.G, OUTPUT);
  pinMode(LED.B, OUTPUT);

  digitalWrite(LED.R, LOW);
  digitalWrite(LED.G, HIGH);
  digitalWrite(LED.B, HIGH);
  while (digitalRead(BUTTON) == LOW);

  mpu.calcGyroOffsets();

  motors_spin(MAX_SPEED);
  delay(30);
  motors_stop();

  digitalWrite(LED.R, HIGH);
  digitalWrite(LED.G, HIGH);
  digitalWrite(LED.B, LOW);
  while (digitalRead(BUTTON) == HIGH);
  while (digitalRead(BUTTON) == LOW);

  if (DO_GREEN) {
    calibrate(black_range, MARGIN_OF_ERROR_BLACK);

    motors_spin(MAX_SPEED);
    delay(30);
    motors_stop();

    digitalWrite(LED.R, HIGH);
    digitalWrite(LED.G, LOW);
    digitalWrite(LED.B, HIGH);
    while (digitalRead(BUTTON) == HIGH);
    while (digitalRead(BUTTON) == LOW);

    calibrate(green_range, MARGIN_OF_ERROR_GREEN);

    motors_spin(MAX_SPEED);
    delay(30);
    motors_stop();

    digitalWrite(LED.R, LOW);
    digitalWrite(LED.G, HIGH);
    digitalWrite(LED.B, LOW);
    while (digitalRead(BUTTON) == HIGH);
    while (digitalRead(BUTTON) == LOW);

  }
  digitalWrite(LED.R, LOW);
  digitalWrite(LED.G, LOW);
  digitalWrite(LED.B, HIGH);

  pingTimer = millis();
  PIDTimer = millis();
}

void loop() {
  /* mpu.update(); */

  for (int i = 1; i >= 0; i--) {
    get_rgb(i);
  }

  /* if (abs(mpu.getAngleZ() - rot) > 60) { */
  /*   go_back(BASE_SPEED); */
  /*   delay(400); */
  /*   rot = mpu.getAngleZ(); */
  /* } */

  if (DO_GREEN) {
    turn_green();
  }

  if (DO_OBSTACLE && millis() - pingTimer >= 50) {
    usDistance = sonar.ping_cm();
    pingTimer = millis();
  }

  if (DO_OBSTACLE && usDistance > 0 && usDistance < OBSTACLE_DISTANCE) {
    motors_stop();
    delay(100);
    usDistance = sonar.ping_cm();

    if (usDistance > 0 && usDistance < OBSTACLE_DISTANCE) {
      divert_obstacle(false);
    }
  }

  if (digitalRead(BUTTON) == HIGH) {
    digitalWrite(LED.R, LOW);
    digitalWrite(LED.G, HIGH);
    digitalWrite(LED.B, HIGH);
    motors_stop();
    while (digitalRead(BUTTON) == HIGH);
    while (digitalRead(BUTTON) == LOW);
    while (digitalRead(BUTTON) == HIGH);
    digitalWrite(LED.R, LOW);
    digitalWrite(LED.G, LOW);
    digitalWrite(LED.B, HIGH);
  }

  if (millis() - PIDTimer >= 5) {
    pid_turn(BASE_SPEED, false);
    PIDTimer = millis();
  }
}
