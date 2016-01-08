// FollowObject - Tries to find an object within range and follow the
//    left edge of the object. The program uses this simple strategy:
//
// If there is an object within range, turn the IR sensor slightly more
// to the left.
//
// Otherwise, turn the IR sensor slightly more to the right.
//
// In either case, turn the robot left or right depending on where the
// IR servo is pointed, and turn more strongly left or right the more
// widely the IR servo is pointed.

#include <Servo.h>
#include <VMSRobot3.h>

void setup() {
  Serial.begin(115200);
  initRobot();

  // Wait for the user to press a switch.
  int switchValue;
  do {
    switchValue = readSwitch();
  } while (switchValue == 0);
}

void loop() {
  delay(20);
  
  int value = analogRead(IR_PIN);
  int curAngle = getServoAngle();
  
  if (value > 250) {
    curAngle = curAngle + 2.0;
  } else {
    curAngle = curAngle - 2.0;
  }

  // Keep the servo within 60 degrees of center.
  curAngle = constrain(curAngle, -60, 60);
  setServoAngle(curAngle);

  // The difference between the two motors is 1.5 times the current
  // servo angle, in degrees. This way we will turn more sharply the more
  // the servo is turned away from center.
  int motorDifference = 3*curAngle/2;
  setSpeed(160 - motorDifference, 160 + motorDifference);
}
