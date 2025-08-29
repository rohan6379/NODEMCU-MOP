#include "ESP8266_OTA.h"

#define motorPin D5

// Ultrasonic Sensor 1 (Front Sensor)
#define trigPin1 D4
#define echoPin1 D3

// Ultrasonic Sensor 2 (Back Sensor)
#define trigPin2 D8
#define echoPin2 D7

// Ultrasonic Sensor 3 (Vertical Sensor)
#define trigPin3 D2
#define echoPin3 D1


// Minimum Distance For The Sensor
int vertDist = 5;   // Vertical Height Minimum Distance In centimetre
int frontDist = 10;  // Front Minimum Distance In centimetre
int backDist = 10;   // Back Minimum Distance In centimetre


ESP8266_OTA OTA;
void setup() {
  OTA.setupWiFi_AP("NodeMCU-MOP", "123456789");
  OTA.begin();
  pinConfig();
  motorOn();
  Serial.println("starting");
}

void loop() {
  OTA.handle();
  if (isSafe()) {
    if (frontSensor() && backSensor()) {
      motorOn();
    } else {
      motorOff();
    }
  } else {
    motorOff();
  }
}

void pinConfig() {
  pinMode(motorPin, OUTPUT);

  // Ultrasonic Sensor 1
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  // Ultrasonic Sensor 2
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  // Ultrasonic Sensor 3
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
}

bool isSafe() {
  digitalWrite(trigPin3, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin3, LOW);

  long duration = pulseIn(echoPin3, HIGH);
  long distance = duration * 0.034 / 2;

  if (distance <= vertDist)
    return true;
  else
    return false;
}

bool frontSensor() {
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);

  long duration = pulseIn(echoPin1, HIGH);
  long distance = duration * 0.034 / 2;

  return (distance > frontDist);
}

bool backSensor() {
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);

  long duration = pulseIn(echoPin2, HIGH);
  long distance = duration * 0.034 / 2;

  return (distance > backDist);
}

void motorOn() {
  digitalWrite(motorPin, 0);
}

void motorOff() {
  analogWrite(motorPin, 100);
}
