//
// VMSRobot3.h - Common subroutines and definitions for the VMS Robot.
//

#ifndef VMSROBOT3_H
#define VMSROBOT3_H

#include <arduino.h>
#include "GoBLE.h"
#include "RobotPose.h"
#include "AnalogScanner.h"
#include "AnalogEncoder.h"

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

// The servo for the IR sensor.
extern Servo irServo;

// Initializes the robot by setting pin modes and centering the servo.
void initRobot();

// Waits until a desired switch is pressed. 1=S1, 2=S2, ..., 5=S5.
void waitForSwitch(int switchIndex);

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
int readSwitch();

// Sets the left and right motor speeds.
void setSpeed(int leftSpeed, int rightSpeed);

// Gets the current servo angle, as an angle from center.
int getServoAngle();

// Sets the servo angle, as an angle from center.
void setServoAngle(int angle);

// Gets the IR sensor value, reading the sensor twice and returning the smaller
// value. This helps eliminates random errors.
int getIRValue();

// Converts an IR sensor value to a distance in meters.
float valueToDistance(int value);

#endif //VMSROBOT3_H
