//
// VMSRobot3.cpp - Common subroutines for the VMS Robot.
//

#include <Servo.h>
#include <VMSRobot3.h>

// The servo for the IR sensor.
Servo irServo;

// Initializes the robot by setting pin modes and centering the servo.
void initRobot() {
  irServo.attach(SERVO_PIN);
  setServoAngle(0);

  pinMode(ONBOARD_LED_PIN, OUTPUT);
  pinMode(M1_DIRECTION, OUTPUT);
  pinMode(M2_DIRECTION, OUTPUT);
  pinMode(LEFT_TRACK_SENSOR_PIN, INPUT);
  pinMode(RIGHT_TRACK_SENSOR_PIN, INPUT);
}

// Waits until a desired switch is pressed. 1=S1, 2=S2, ..., 5=S5.
void waitForSwitch(int switchIndex) {
  while (readSwitch() != switchIndex) {
    // Do nothing.
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

// Gets the current servo angle, as an angle from center.
int getServoAngle() {
  return irServo.read() - 90;
}

// Sets the servo angle, as an angle from center.
void setServoAngle(int angle) {
  irServo.write(90 + angle);
}

// Gets the IR sensor value, reading the sensor twice and returning the
// smaller value. This helps eliminates random errors.
int getIRValue() {
  int v1 = analogRead(IR_PIN);
  delay(50);
  int v2 = analogRead(IR_PIN);

  return min(v1, v2);
}

// Linear fit of voltage vs. 1/distance based on Sharp data sheet
// for 10-80cm sensor. The 5/1024 factor is to convert from analog
// input value to voltage.
//     In cm, fitted 1/d = m*V + b: m=0.05, b=-0.14
const float M = 0.05 * 5.0 / 1024.0;
const float B = -0.014;

// Converts an IR sensor value to a distance in meters.
float valueToDistance(int value) {
  float invDistance = max(M*value + B, 1E-6);
  return min(.8, .01/invDistance);
}
