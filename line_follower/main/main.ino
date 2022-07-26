/* #include <Wire.h> */

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

/* typedef struct gyro_values { */
/*     int AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ; */
/* } gyro_values; */

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

/* const int MPU = 0x68; */

// Button
const int BUTTON = 32;

/// Constants
const int MAX_SPEED = 200;
const int MIN_SPEED = 100;
const int BASE_SPEED = 110;

/* const float K = 10000; */
const float KP = 1.5;
const float KI = 0.02;
const float KD = 3.5;
const float MAX_I = 100;
const float SR = 0.7;

const long GYRO_90 = 10000;
const long GYRO_180 = 20000;
const long TIME_90 = 500;
const long TIME_180 = 1000;

const bool DO_GREEN = true;
const int MARGIN_OF_ERROR_GREEN = 15;
const int MARGIN_OF_ERROR_BLACK = 8;

const bool DO_OBSTACLE = true;
const float OBSTACLE_DISTANCE = 3.0;

const long GRAV = 14000;

/// Variables
/* gyro_values gyro; */

rgb_values rgb_sensor_values[2];

rgb_values green_range[2], black_range[2];

int motor_speed[2];

float error, lastError, P, I, D;

int PID;

unsigned long time, lastTime;

float rot;

/// Setup
void setup() {
    Serial.begin(9600);

    // Ultrassonic
    pinMode(US.echo, INPUT);
    pinMode(US.trig, OUTPUT);

    // Gyroscope
    /* Wire.begin(); */
    /* Wire.beginTransmission(MPU); */
    /* Wire.write(0x6B); */

    /* Wire.write(0); */
    /* Wire.endTransmission(true); */

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

  /* motors_rotate_time(TIME_90, MAX_SPEED); */

  if (DO_GREEN) {
    while (digitalRead(BUTTON) == LOW);

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

  Serial.println(black_range[0].red);
  Serial.println(black_range[0].green);
  Serial.println(black_range[0].blue);
  Serial.println(black_range[0].ref);

  Serial.println(black_range[1].red);
  Serial.println(black_range[1].green);
  Serial.println(black_range[1].blue);
  Serial.println(black_range[1].ref);
  
  Serial.println(green_range[0].red);
  Serial.println(green_range[0].green);
  Serial.println(green_range[0].blue);
  Serial.println(green_range[0].ref);

  Serial.println(green_range[1].red);
  Serial.println(green_range[1].green);
  Serial.println(green_range[1].blue);
  Serial.println(green_range[1].ref);

  /* black_range[0].red = 131; */
  /* black_range[0].green = 128; */
  /* black_range[0].blue = 124; */
  /* black_range[0].ref = 37; */

  /* black_range[1].red = 162; */
  /* black_range[1].green = 136; */
  /* black_range[1].blue = 135; */
  /* black_range[1].ref = 48; */

  /* green_range[0].red = 103; */
  /* green_range[0].green = 75; */
  /* green_range[0].blue = 87; */
  /* green_range[0].ref = 24; */

  /* green_range[1].red = 115; */
  /* green_range[1].green = 83; */
  /* green_range[1].blue = 96; */
  /* green_range[1].ref = 31; */
}

void loop() {
  lastTime = time;
  time = millis();

  /* get_gyro(); */

  /* rot += gyro.GyZ / 1000 * (int) (time - lastTime); */

  /* if (abs(rot) >= 6000) { */
  /*   motors_turn(PID / abs(PID) * 110, BASE_SPEED, true); */
  /*   while (millis() - time < 275); */
  /*   motors_turn(PID / abs(PID) * 110, BASE_SPEED, false); */
  /*   while (millis() - time < 375); */
  /*   rot = 0; */
  /* } */

  for (int i = 0; i < 2; i++) {
    get_rgb(i);
  }

  if (DO_GREEN) {
    turn_green();
  }

  if (us_distance() < OBSTACLE_DISTANCE) {
    motors_stop();
    delay(100);

    if (us_distance() < OBSTACLE_DISTANCE && DO_OBSTACLE) {
      divert_obstacle(false);
    }
  }

  if (digitalRead(BUTTON) == HIGH) {
    motors_stop();
    while (digitalRead(BUTTON) == HIGH);
    while (digitalRead(BUTTON) == LOW);
    while (digitalRead(BUTTON) == HIGH);
  }

  /* if (gyro.AcZ < GRAV) { */
  /*   pid_turn(MAX_SPEED, false); */
  /* } else { */
  /*   pid_turn(BASE_SPEED, false); */
  /* } */
  pid_turn(BASE_SPEED, false);

  /* Serial.println(rot); */
}
