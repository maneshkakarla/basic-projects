#include <SoftwareSerial.h>

// -------- IR SENSOR PINS --------
const int IR1 = A1;
const int IR2 = A2;

// -------- MOTOR PINS --------
int m1a = 4;
int m1b = 5;
int m2a = 6;
int m2b = 7;

// -------- ENABLE PINS --------
int en1 = 9;
int en2 = 8;

// -------- ULTRASONIC --------
const int trigPin = 11;
const int echoPin = 12;

// -------- BUZZER --------
const int buzzer = 13;

// -------- MODE BUTTON --------
const int modeBtn = A0;

// -------- ESP --------
SoftwareSerial esp(2, 3);
String DATA = "";

// -------- VARIABLES --------
long duration;
float distance;

int mode = 1; // 1=Ultrasonic, 2=Line, 3=ESP
bool lastButtonState = HIGH;

// -------- SETUP --------
void setup() {
  Serial.begin(9600);
  esp.begin(9600);

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);

  pinMode(m1a, OUTPUT);
  pinMode(m1b, OUTPUT);
  pinMode(m2a, OUTPUT);
  pinMode(m2b, OUTPUT);

  pinMode(en1, OUTPUT);
  pinMode(en2, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(buzzer, OUTPUT);
  pinMode(modeBtn, INPUT_PULLUP);

  Serial.println("🚀 Robot Started");
  Serial.println("Mode 1: Ultrasonic (Default)");

  stopMotors();
}

// -------- MAIN LOOP --------
void loop() {

  checkModeButton();

  if (mode == 1) ultrasonicMode();
  else if (mode == 2) lineFollowerMode();
  else if (mode == 3) espMode();
}

// -------- MODE BUTTON --------
void checkModeButton() {
  bool currentState = digitalRead(modeBtn);

  if (lastButtonState == HIGH && currentState == LOW) {
    delay(200);

    mode++;
    if (mode > 3) mode = 1;

    stopMotors();

    if (mode == 1) {
      Serial.println("🔁 Mode 1: Ultrasonic");
      beep(1);
    }
    else if (mode == 2) {
      Serial.println("🔁 Mode 2: Line Follower");
      beep(2);
    }
    else {
      Serial.println("🔁 Mode 3: ESP Control");
      beep(3);
    }
  }

  lastButtonState = currentState;
}

// -------- ULTRASONIC MODE --------
void ultrasonicMode() {

  distance = getDistance();

  Serial.print("[ULTRA] ");
  Serial.println(distance);

  if (distance > 0 && distance < 25) {

    digitalWrite(buzzer, HIGH);
    stopMotors();

    backward(); delay(400);
    left(); delay(350);

    digitalWrite(buzzer, LOW);
  }
  else {
    forward();
  }
}

// -------- LINE FOLLOWER MODE --------
void lineFollowerMode() {

  int leftIR  = digitalRead(IR1);
  int rightIR = digitalRead(IR2);

  if (leftIR == 0 && rightIR == 0) forward();
  else if (leftIR == 0 && rightIR == 1) sharpLeft();
  else if (leftIR == 1 && rightIR == 0) sharpRight();
  else stopMotors();

  delay(20);
}

// -------- ESP MODE --------
void espMode() {

  if (esp.available()) {

    DATA = esp.readStringUntil('\n');
    DATA.trim();

    Serial.print("[ESP] ");
    Serial.println(DATA);

    if (DATA == "front") forward();
    else if (DATA == "back") backward();
    else if (DATA == "left") left();
    else if (DATA == "right") right();
    else if (DATA == "stop") stopMotors();
  }
}

// -------- DISTANCE --------
float getDistance() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 20000);

  return duration * 0.034 / 2;
}

// -------- MOTOR FUNCTIONS --------
void forward() {
  analogWrite(en1, 130);
  analogWrite(en2, 130);

  digitalWrite(m1a, HIGH); digitalWrite(m1b, LOW);
  digitalWrite(m2a, LOW);  digitalWrite(m2b, HIGH);
}

void backward() {
  analogWrite(en1, 130);
  analogWrite(en2, 130);

  digitalWrite(m1a, LOW);  digitalWrite(m1b, HIGH);
  digitalWrite(m2a, HIGH); digitalWrite(m2b, LOW);
}

void left() {
  analogWrite(en1, 130);
  analogWrite(en2, 130);

  digitalWrite(m1a, LOW);  digitalWrite(m1b, HIGH);
  digitalWrite(m2a, LOW);  digitalWrite(m2b, HIGH);
}

void right() {
  analogWrite(en1, 130);
  analogWrite(en2, 130);

  digitalWrite(m1a, HIGH); digitalWrite(m1b, LOW);
  digitalWrite(m2a, HIGH); digitalWrite(m2b, LOW);
}

void sharpLeft() {
  analogWrite(en1, 130);
  analogWrite(en2, 130);

  digitalWrite(m1a, LOW); digitalWrite(m1b, LOW);
  digitalWrite(m2a, LOW); digitalWrite(m2b, HIGH);
}

void sharpRight() {
  analogWrite(en1, 130);
  analogWrite(en2, 130);

  digitalWrite(m1a, HIGH); digitalWrite(m1b, LOW);
  digitalWrite(m2a, LOW);  digitalWrite(m2b, LOW);
}

void stopMotors() {
  analogWrite(en1, 0);
  analogWrite(en2, 0);

  digitalWrite(m1a, LOW);
  digitalWrite(m1b, LOW);
  digitalWrite(m2a, LOW);
  digitalWrite(m2b, LOW);
}

// -------- BUZZER --------
void beep(int mode) {

  if (mode == 1) {
    digitalWrite(buzzer, HIGH);
    delay(1000);
    digitalWrite(buzzer, LOW);
  }

  else if (mode == 2) {
    for (int i = 0; i < 2; i++) {
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer, LOW);
      delay(300);
    }
  }

  else if (mode == 3) {
    for (int i = 0; i < 3; i++) {
      digitalWrite(buzzer, HIGH);
      delay(300);
      digitalWrite(buzzer, LOW);
      delay(200);
    }
  }
}