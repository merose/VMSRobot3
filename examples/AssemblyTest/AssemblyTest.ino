// AssemblyTest - Tests the various parts of the VMS Robot, version 3.

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

// Set up the motor controls for output, center the servo, and wait
// for a switch press. The five switches run 5 different tests.
void setup() {
  Serial.begin(115200);
  irServo.attach(SERVO_PIN);
  setServoAngle(0);

  pinMode(ONBOARD_LED_PIN, OUTPUT);
  pinMode(M1_DIRECTION, OUTPUT);
  pinMode(M2_DIRECTION, OUTPUT);
  pinMode(LEFT_TRACK_SENSOR_PIN, INPUT);
  pinMode(RIGHT_TRACK_SENSOR_PIN, INPUT);

  int switchValue;
  do {
    switchValue = readSwitch();
  } while (switchValue == 0);

  for (;;) {
    switch (switchValue) {
    case 1:
      blinkLoop();
      break;
    case 2:
      motorLoop();
      break;
    case 3:
      servoLoop();
      break;
    case 4:
      distanceLoop();
      break;
    case 5:
      trackSensorLoop();
      break;
    }
  }
}

void loop() {
  // Do nothing, since everything is handled in setup().
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

// Test the Romeo board by blinking the onboard LED once per second.
void blinkLoop() {
  digitalWrite(ONBOARD_LED_PIN, !digitalRead(ONBOARD_LED_PIN));
  delay(500);
}

// Test the motors by driving forward and back with the left motor,
// then forward and back with the right motor.
void motorLoop() {
  setSpeed(128, 0);
  delay(2000);
  setSpeed(-128, 0);
  delay(2000);

  setSpeed(0, 128);
  delay(2000);
  setSpeed(0, -128);
  delay(2000);

  // And then stop.
  setSpeed(0, 0);
  for (;;) {
    // Do nothing.
  }
}

const int ANGLES[] = {60, 30, 0, -30, -60, -30, 0, 30};
const int NUM_ANGLES = sizeof(ANGLES) / sizeof(ANGLES[0]);

int curAngle = 0;

// Test the servo by moving the servo repeatedly through the angles above.
void servoLoop() {
  setServoAngle(ANGLES[curAngle]);
  curAngle = (curAngle + 1) % NUM_ANGLES;
  delay(250);
}

const int IR_THRESHOLD = 375;

// Test the distance sensor by lighting the onboard LED if the analog
// value is greater than a threshold.
void distanceLoop() {
  digitalWrite(ONBOARD_LED_PIN, analogRead(IR_PIN) >= IR_THRESHOLD);
  delay(50);
}

// Test the track sensors by turning the servo to the left or right,
// if one of the track sensors detects an edge.
void trackSensorLoop() {
  int foundLeftEdge = !digitalRead(LEFT_TRACK_SENSOR_PIN);
  int foundRightEdge = !digitalRead(RIGHT_TRACK_SENSOR_PIN);
  digitalWrite(ONBOARD_LED_PIN, foundLeftEdge || foundRightEdge);

  if (foundLeftEdge) {
    setServoAngle(60);
  } else if (foundRightEdge) {
    setServoAngle(-60);
  } else {
    setServoAngle(0);
  }

  delay(100);
}

// Sets the left and right motor speeds.
void setSpeed(int leftSpeed, int rightSpeed) {
  digitalWrite(M1_DIRECTION, (leftSpeed >= 0 ? HIGH : LOW));
  analogWrite(M1_SPEED, abs(leftSpeed));
  digitalWrite(M2_DIRECTION, (rightSpeed >= 0 ? HIGH : LOW));
  analogWrite(M2_SPEED, abs(rightSpeed));
}

// Sets the servo angle, as an angle from center, where positive angles
// are to the left (counter-clockwise).
void setServoAngle(int angle) {
  irServo.write(90 + angle);
}
