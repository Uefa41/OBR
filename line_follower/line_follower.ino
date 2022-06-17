/// Right and left defined looking from the top with the robot front up

//LED

//Ultrassonic sensor
#define US_ECHO 40
#define US_TRIG 41

//Speed
#define MAX_SPEED 200
#define MIN_SPEED 110
#define BASE_SPEED 115

//PID
#define KP 15
#define KI 0.2
#define KD 0
#define MAX_I 100


struct rgb_sensor {
  int S0, S1, S2, S3, OUT;
};

struct rgb_sensor_values {
  int red, green, blue, ref;
};

struct motor {
  int A, B, SPEED;
};

///Constants
const struct rgb_sensor RGB[] = {
  //Left RGB
  {
    .S0 = 51,
    .S1 = 50,
    .S2 = 52,
    .S3 = 53,
    .OUT = 38,
  },
  //Right RGB
  {
    .S0 = 42,
    .S1 = 43,
    .S2 = 44,
    .S3 = 45,
    .OUT = 39,
  },
  //Mid RGB
  {
    .S0 = 46,
    .S1 = 47,
    .S2 = 48,
    .S3 = 49,
    .OUT = 37,
  },
};

const struct motor MOTORS[] = {
  {
    .A = 36,
    .B = 35,
    .SPEED = 9,
  },
  {
    .A = 34,
    .B = 33,
    .SPEED = 10,
  },
};

/// Variables
struct rgb_sensor_values rgb_values[3];

int motor_speed[2];

//PID
int error;
int lastError;
int P;
int I;
int D;

void get_colors();
void turn(int value);

void setup() {
  // put your setup code here, to run once:
  //Motors
  pinMode(MOTORS[0].A, OUTPUT);
  pinMode(MOTORS[0].B, OUTPUT);
  pinMode(MOTORS[0].SPEED, OUTPUT);
  pinMode(MOTORS[1].A, OUTPUT);
  pinMode(MOTORS[1].B, OUTPUT);
  pinMode(MOTORS[1].SPEED, OUTPUT);


  //RGB
  for (int i = 0; i < 3; i++) {
    pinMode(RGB[i].S0, OUTPUT);
    pinMode(RGB[i].S1, OUTPUT);
    pinMode(RGB[i].S2, OUTPUT);
    pinMode(RGB[i].S3, OUTPUT);
    pinMode(RGB[i].OUT, INPUT);
    digitalWrite(RGB[i].S0, HIGH);
    digitalWrite(RGB[i].S1, LOW);
  }

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  get_colors();

  Serial.print("Red: ");
  Serial.print(rgb_values[0].red);
  Serial.print(" Green: ");
  Serial.print(rgb_values[0].green);
  Serial.print(" Blue: ");
  Serial.print(rgb_values[0].blue);
  Serial.print(" Ref: ");
  Serial.print(rgb_values[0].ref);
  Serial.print(" motor_speed[0]: ");
  Serial.print(motor_speed[0]);
  Serial.print(" motor_speed[1]: ");
  Serial.println(motor_speed[1]);

  error = rgb_values[1].ref - rgb_values[0].ref;

  P = KP * error;
  I += KI * error;
  D = KD * (error - lastError);

  I = min(I, MAX_I);
  I = max(I, -MAX_I);

  turn(P + I + D);
  /* turn(P); */

  lastError = error;
}

void get_colors() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(RGB[i].S2, LOW);
    digitalWrite(RGB[i].S3, LOW);

    if (digitalRead(RGB[i].OUT) == HIGH) {
      rgb_values[i].red = pulseIn(RGB[i].OUT, LOW);
    } else
      rgb_values[i].red = pulseIn(RGB[i].OUT, HIGH);
  }


  for (int i = 0; i < 3; i++) {
    digitalWrite(RGB[i].S3, HIGH);

    if (digitalRead(RGB[i].OUT) == HIGH) {
      rgb_values[i].blue = pulseIn(RGB[i].OUT, LOW);
    } else {
      rgb_values[i].blue = pulseIn(RGB[i].OUT, HIGH);
    }
  }

  for (int i = 0; i < 3; i++) {
    digitalWrite(RGB[i].S2, HIGH);
    digitalWrite(RGB[i].S3, LOW);

    if (digitalRead(RGB[i].OUT) == HIGH) {
      rgb_values[i].ref = pulseIn(RGB[i].OUT, LOW);
    } else {
      rgb_values[i].ref = pulseIn(RGB[i].OUT, HIGH);
    }
  }

  for (int i = 0; i < 3; i++) {
    digitalWrite(RGB[i].S3, HIGH);

    if (digitalRead(RGB[i].OUT) == HIGH) {
      rgb_values[i].green = pulseIn(RGB[i].OUT, LOW);
    } else {
      rgb_values[i].green = pulseIn(RGB[i].OUT, HIGH);
    }
  }
}

void turn(int value) {
  motor_speed[0] = BASE_SPEED + value;
  motor_speed[1] = BASE_SPEED - value;


  motor_speed[0] = max(-MAX_SPEED, motor_speed[0]);
  motor_speed[1] = max(-MAX_SPEED, motor_speed[1]);

  if (motor_speed[0] >= 0) {
    digitalWrite(MOTORS[0].A, HIGH);
    digitalWrite(MOTORS[0].B, LOW);
    motor_speed[0] = motor_speed[0] * (MAX_SPEED - MIN_SPEED) / MAX_SPEED + MIN_SPEED;
  } else {
    digitalWrite(MOTORS[0].A, LOW);
    digitalWrite(MOTORS[0].B, HIGH);
    motor_speed[0] = motor_speed[0] * (MAX_SPEED - MIN_SPEED) / MAX_SPEED - MIN_SPEED;
  }

  analogWrite(MOTORS[0].SPEED, abs(motor_speed[0]));

  if (motor_speed[1] >= 0) {
    digitalWrite(MOTORS[1].A, HIGH);

  } else {
    digitalWrite(MOTORS[1].A, LOW);
  }

  analogWrite(MOTORS[1].SPEED, abs(motor_speed[1]));
}
