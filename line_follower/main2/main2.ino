#include <NewPing.h>

/// Structs
typedef struct rgb_sensor {
  unsigned short S0, S1, S2, S3, OUT;
} rgb_sensor;

typedef struct rgb_values {
  long red, green, blue, ref;
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
const ultrasonic US[] = {
  {
    .echo = 40,
    .trig = 41,
  },
  {
    .echo = 27,
    .trig = 26,
  },
  {
    .echo = 24,
    .trig = 25,
  },
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

// Noise
const int NOISE = 29;

/// Constants
const int MAX_SPEED = 200;
const int MIN_SPEED = 100;
const int BASE_SPEED = 120;

const float KP = 1.2;
const float KI = 0.01;
const float KD = 0.1;
const float MAX_I = 100;
const float SR = 0.6;

const long TIME_90 = 1000;
const long TIME_180 = 4000;

const bool DO_GREEN = true;
const int MARGIN_OF_ERROR_GREEN = 7;
const int MARGIN_OF_ERROR_BLACK = 2;
const int MARGIN_OF_ERROR_BLACK_OBS = 12;

const bool DO_OBSTACLE = true;
const float OBSTACLE_DISTANCE = 4.0;
const float WALL_DISTANCE = 20.0;

/// Variables
NewPing sonar_front(US[0].trig, US[0].echo, 100);
NewPing sonar_left(US[1].trig, US[1].echo, 100);
NewPing sonar_right(US[2].trig, US[2].echo, 100);

rgb_values rgb_sensor_values[2];

rgb_values green_range[2], black_range[2], black_range_obs[2];

int motor_speed[2];

float error, lastError, P, I, D;

int PID;

unsigned long PIDTimer;

unsigned long time, lastTime;

float rot;

unsigned long pingTimer;
unsigned int usDistance[3];
int usID;
float lastUltrasonic;
float ultrasonicValue;

/// Setup
void setup() {
    Serial.begin(9600);

    // Ultrassonic
    pinMode(US[0].echo, INPUT);
    pinMode(US[0].trig, OUTPUT);
    pinMode(US[1].echo, INPUT);
    pinMode(US[1].trig, OUTPUT);
    pinMode(US[2].echo, INPUT);
    pinMode(US[2].trig, OUTPUT);

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

  pinMode(NOISE, OUTPUT);

  digitalWrite(LED.R, LOW);
  digitalWrite(LED.G, HIGH);
  digitalWrite(LED.B, HIGH);
  while (digitalRead(BUTTON) == HIGH);
  while (digitalRead(BUTTON) == LOW);

  if (DO_GREEN) {
    /* digitalWrite(NOISE, HIGH); */
    calibrate(black_range, MARGIN_OF_ERROR_BLACK);
    calibrate(black_range_obs, MARGIN_OF_ERROR_BLACK_OBS);
    /* digitalWrite(NOISE, LOW); */

    motors_spin(MAX_SPEED);
    delay(30);
    motors_stop();

    digitalWrite(LED.R, HIGH);
    digitalWrite(LED.G, HIGH);
    digitalWrite(LED.B, LOW);
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
  for (int i = 0; i < 2; i++) {
    get_rgb(i);
  }

  if (DO_GREEN) {
    turn_green();
  }

  if (DO_OBSTACLE && millis() - pingTimer >= 29) {
    switch (usID) {
      case 0:
        usDistance[0] = sonar_front.ping_cm();
        break;
      
      case 1:
        usDistance[1] = sonar_left.ping_cm();
        break;

      case 2:
        usDistance[2] = sonar_right.ping_cm();
        break;
    }
    usID = (usID + 1) % 3;
    pingTimer = millis();
  }

  if (DO_OBSTACLE && usDistance[0] > 0 && usDistance[0] < OBSTACLE_DISTANCE) {
    motors_stop();
    delay(100);
    usDistance[0] = sonar_front.ping_cm();

    if (usDistance[0] > 0 && usDistance[0] < OBSTACLE_DISTANCE) {
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
    if (usDistance[1] <= WALL_DISTANCE && usDistance[1] > 0 && usDistance[2] <= WALL_DISTANCE && usDistance[2] > 0) {
      pid_turn(MAX_SPEED + 20, false);
    } else {
      pid_turn(BASE_SPEED, false);
    }
    PIDTimer = millis();
  }
}
