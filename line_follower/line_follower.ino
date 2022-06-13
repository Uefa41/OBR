/// Right and left defined looking from the top with the robot front up

///RGB
//Left RGB
#define RGB_L_S0 51
#define RGB_L_S1 50
#define RGB_L_S2 52
#define RGB_L_S3 53
#define RGB_L_OUT 38

//Right RGB
#define RGB_R_S0 42
#define RGB_R_S1 43
#define RGB_R_S2 44
#define RGB_R_S3 45
#define RGB_R_OUT 39

//Mid RGB
#define RGB_M_S0 46
#define RGB_M_S1 47
#define RGB_M_S2 48
#define RGB_M_S3 49
#define RGB_M_OUT 37


//Motors
//Left motor
#define MOTOR_L_1 36
#define MOTOR_L_2 35
#define MOTOR_L_SPEED 9

//Right motor
#define MOTOR_R_1 34
#define MOTOR_R_2 33
#define MOTOR_R_SPEED 10

#define US_ECHO 40
#define US_TRIG 41

/// Variables
//Left RGB
int l_red;
int l_green;
int l_blue;
int l_ref;

//Right RGB
int r_red;
int r_green;
int r_blue;
int r_ref;

//Mid RGB
int m_red;
int m_green;
int m_blue;
int m_ref;

//Left motor
int l_speed;

//Right motor
int r_speed;

//Error function
int error;

void color_left();
void color_right();
void color_mid();
void turn(int value);

void setup() {
  // put your setup code here, to run once:
  //Motors
  pinMode(MOTOR_L_1, OUTPUT);
  pinMode(MOTOR_L_2, OUTPUT);
  pinMode(MOTOR_L_SPEED, OUTPUT);
  pinMode(MOTOR_R_1, OUTPUT);
  pinMode(MOTOR_R_2, OUTPUT);
  pinMode(MOTOR_R_SPEED, OUTPUT);
  
  //Left RGB
  pinMode(RGB_L_S0, OUTPUT);
  pinMode(RGB_L_S1, OUTPUT);
  pinMode(RGB_L_S2, OUTPUT);
  pinMode(RGB_L_S3, OUTPUT);
  pinMode(RGB_L_OUT, INPUT);
  digitalWrite(RGB_L_S0, HIGH);
  digitalWrite(RGB_L_S1, LOW);

  //Right RGB
  pinMode(RGB_R_S0, OUTPUT);
  pinMode(RGB_R_S1, OUTPUT);
  pinMode(RGB_R_S2, OUTPUT);
  pinMode(RGB_R_S3, OUTPUT);
  pinMode(RGB_R_OUT, INPUT);
  digitalWrite(RGB_R_S0, HIGH);
  digitalWrite(RGB_R_S1, LOW);

  //Mid RGB
  pinMode(RGB_M_S0, OUTPUT);
  pinMode(RGB_M_S1, OUTPUT);
  pinMode(RGB_M_S2, OUTPUT);
  pinMode(RGB_M_S3, OUTPUT);
  pinMode(RGB_M_OUT, INPUT);
  digitalWrite(RGB_M_S0, HIGH);
  digitalWrite(RGB_M_S1, LOW);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  color_left();
  color_right();
  color_mid();
  Serial.print("Red: ");
  Serial.print(l_red);
  Serial.print(" Green: ");
  Serial.print(l_green);
  Serial.print(" Blue: ");
  Serial.print(l_blue);
  Serial.print(" Ref: ");
  Serial.print(l_ref);
  Serial.print(" l_speed: ");
  Serial.print(l_speed);
  Serial.print(" r_speed: ");
  Serial.println(r_speed);


  if(l_ref > r_ref) {
    error = r_ref - l_ref - m_ref;
  } else if(r_ref > l_ref) {
    error = r_ref - l_ref + m_ref;
  } else error = 0;

  turn(error);

  /*digitalWrite(MOTOR_R_1, HIGH);
  digitalWrite(MOTOR_R_2, LOW);
  digitalWrite(MOTOR_L_1, HIGH);
  digitalWrite(MOTOR_L_2, LOW);
  digitalWrite(MOTOR_R_SPEED, HIGH);
  digitalWrite(MOTOR_L_SPEED, HIGH);*/
}

void color_left() {
  digitalWrite(RGB_L_S2, LOW);
  digitalWrite(RGB_L_S3, LOW);

  if(digitalRead(RGB_L_OUT) == HIGH) {
    l_red = pulseIn(RGB_L_OUT, LOW);
  } else {
    l_red = pulseIn(RGB_L_OUT, HIGH);
  }

  digitalWrite(RGB_L_S3, HIGH);

  if(digitalRead(RGB_L_OUT) == HIGH) {
    l_blue = pulseIn(RGB_L_OUT, LOW);
  } else {
    l_blue = pulseIn(RGB_L_OUT, HIGH);
  }

  digitalWrite(RGB_L_S2, HIGH);
  digitalWrite(RGB_L_S3, LOW);

  if(digitalRead(RGB_L_OUT) == HIGH) {
    l_ref= pulseIn(RGB_L_OUT, LOW);
  } else {
    l_ref= pulseIn(RGB_L_OUT, HIGH);
  }

  digitalWrite(RGB_L_S3, HIGH);

  if(digitalRead(RGB_L_OUT) == HIGH) {
    l_green= pulseIn(RGB_L_OUT, LOW);
  } else {
    l_green= pulseIn(RGB_L_OUT, HIGH);
  }
}

void color_right() {
  digitalWrite(RGB_R_S2, LOW);
  digitalWrite(RGB_R_S3, LOW);

  if(digitalRead(RGB_R_OUT) == HIGH) {
    r_red = pulseIn(RGB_R_OUT, LOW);
  } else {
    r_red = pulseIn(RGB_R_OUT, HIGH);
  }

  digitalWrite(RGB_R_S3, HIGH);

  if(digitalRead(RGB_R_OUT) == HIGH) {
    r_blue = pulseIn(RGB_R_OUT, LOW);
  } else {
    r_blue = pulseIn(RGB_R_OUT, HIGH);
  }

  digitalWrite(RGB_R_S2, HIGH);
  digitalWrite(RGB_R_S3, LOW);

  if(digitalRead(RGB_R_OUT) == HIGH) {
    r_ref= pulseIn(RGB_R_OUT, LOW);
  } else {
    r_ref= pulseIn(RGB_R_OUT, HIGH);
  }

  digitalWrite(RGB_R_S3, HIGH);

  if(digitalRead(RGB_R_OUT) == HIGH) {
    r_green= pulseIn(RGB_R_OUT, LOW);
  } else {
    r_green= pulseIn(RGB_R_OUT, HIGH);
  }
}

void color_mid() {
  digitalWrite(RGB_M_S2, LOW);
  digitalWrite(RGB_M_S3, LOW);

  if(digitalRead(RGB_M_OUT) == HIGH) {
    m_red = pulseIn(RGB_M_OUT, LOW);
  } else {
    m_red = pulseIn(RGB_M_OUT, HIGH);
  }

  digitalWrite(RGB_M_S3, HIGH);

  if(digitalRead(RGB_M_OUT) == HIGH) {
    m_blue = pulseIn(RGB_M_OUT, LOW);
  } else {
    m_blue = pulseIn(RGB_M_OUT, HIGH);
  }

  digitalWrite(RGB_M_S2, HIGH);
  digitalWrite(RGB_M_S3, LOW);

  if(digitalRead(RGB_M_OUT) == HIGH) {
    m_ref= pulseIn(RGB_M_OUT, LOW);
  } else {
    m_ref= pulseIn(RGB_M_OUT, HIGH);
  }

  digitalWrite(RGB_M_S3, HIGH);

  if(digitalRead(RGB_M_OUT) == HIGH) {
    m_green= pulseIn(RGB_M_OUT, LOW);
  } else {
    m_green= pulseIn(RGB_M_OUT, HIGH);
  }
}

void turn(int value) {
   l_speed = 100 + value * 355 / 400;
   r_speed = 100 - value * 355 / 400;

   l_speed = min(255, l_speed);
   r_speed = min(255, r_speed);

   l_speed = max(-255, l_speed);
   r_speed = max(-255, r_speed);

   if(l_speed >=0) {
    digitalWrite(MOTOR_L_1, HIGH);
    digitalWrite(MOTOR_L_2, LOW);
   } else {
    digitalWrite(MOTOR_L_1, LOW);
    digitalWrite(MOTOR_L_2, HIGH);
   }

   analogWrite(MOTOR_L_SPEED, abs(l_speed));

  if(r_speed >=0) {
    digitalWrite(MOTOR_R_1, HIGH);
    digitalWrite(MOTOR_R_2, LOW);
   } else {
    digitalWrite(MOTOR_R_1, LOW);
    digitalWrite(MOTOR_R_2, HIGH);
   }

   analogWrite(MOTOR_R_SPEED, abs(r_speed));
}
