#include <Servo.h>

const int ONBOARD_SWITCH_PIN = A7;
const int ONBOARD_LED_PIN = 13;

const int SERVO_PIN = 9;
const int IR_PIN = A4;
const int LEFT_TRACK_SENSOR_PIN = A2;
const int RIGHT_TRACK_SENSOR_PIN = A3;

// The pins for motor control on the Romeo BLE.
const int M1_DIRECTION = 4;
const int M1_SPEED = 5;
const int M2_SPEED = 6;
const int M2_DIRECTION = 7;

Servo irServo;

void setup() {
  Serial.begin(115200);
  irServo.attach(SERVO_PIN);
  setServoAngle(0);

  pinMode(ONBOARD_LED_PIN, OUTPUT);
  pinMode(M1_DIRECTION, OUTPUT);
  pinMode(M2_DIRECTION, OUTPUT);
  pinMode(LEFT_TRACK_SENSOR_PIN, INPUT);
  pinMode(RIGHT_TRACK_SENSOR_PIN, INPUT);

  while (readSwitch() != 1) {
    // Do nothing.
  }
}

void loop() {
  if (!digitalRead(LEFT_TRACK_SENSOR_PIN)) {
    // Hit edge on left. Spin back round right wheel.
    setSpeed(-128, 0);
    delay(2200);
  } else if (!digitalRead(RIGHT_TRACK_SENSOR_PIN)) {
    // Hit edge on right. Spin back around left wheel.
    setSpeed(0, -128);
    delay(2200);
  } else if (analogRead(IR_PIN) >= 375) {
    // Obstacle straight ahead. Pivot left.
    setSpeed(-128, 128);
    delay(750);
  } else {
    // No obstacles, go straight.
    setSpeed(128, 128);
  }
}

// Reads the built-in switches on the Romeo and returns a value indicating
// which switch, if any, is depressed. The analog read value thresholds used
// were determined empirically and fall roughly mid-way between the actual
// value you can read for each switch.
//
// Return value:
//   0 - no switches
//   1 - S1
//   2 - S2
//   3 - S3
//   4 - S4
//   5 - S5
int readSwitch() {
  int value = analogRead(ONBOARD_SWITCH_PIN);
  
  if (value > 800) {
    return 0;
  } else if (value > 600) {
    return 5;
  } else if (value > 400) {
    return 4;
  } else if (value > 250) {
    return 3;
  } else if (value > 75) {
    return 2;
  } else {
    return 1;
  }
}

// Sets the left and right motor speeds.
void setSpeed(int leftSpeed, int rightSpeed) {
  digitalWrite(M1_DIRECTION, (leftSpeed >= 0 ? HIGH : LOW));
  analogWrite(M1_SPEED, abs(leftSpeed));
  digitalWrite(M2_DIRECTION, (rightSpeed >= 0 ? HIGH : LOW));
  analogWrite(M2_SPEED, abs(rightSpeed));
}

// Sets the servo angle, as an angle from center.
void setServoAngle(int angle) {
  irServo.write(90 + angle);
}
