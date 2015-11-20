// FindTarget - Pans a servo back and forth until a close target is found.

// An example for the VMS Robotics elective.

#include <Servo.h>

// The maximum angle off of center to move to.
const int MAX_ANGLE = 60;

// How much to turn the servo in one step.
const int DELTA_ANGLE = 5;

// What value from the infrared sensor indicates a target present.
const int IR_THRESHOLD_VALUE = 100;

Servo irServo;
int curAngle = 0;
int deltaAngle = DELTA_ANGLE;

// Start the serial port and attach to the servo control pin.
void setup() {
  Serial.begin(38400);
  irServo.attach(9);
  
  delay(1000);
  setServoAngle(curAngle);
  delay(1000);
}

// Forever, turn the servo and try to find a target. If there is no target,
//     adjust the servo angle. If th angle is too big or too small,
//     change the panning direction.
void loop() {
  setServoAngle(curAngle);
  delay(25);
  
  int value = analogRead(3);
  if (value < IR_THRESHOLD_VALUE) {
    int newAngle = curAngle + deltaAngle;
    if (newAngle > MAX_ANGLE) {
      deltaAngle = -DELTA_ANGLE;
      newAngle = curAngle + deltaAngle;
    } else if (newAngle < -MAX_ANGLE) {
      deltaAngle = DELTA_ANGLE;
      newAngle = curAngle + deltaAngle;
    }
    
    curAngle = newAngle;
  }
}

// Sets the servo angle, as an angle from center.
void setServoAngle(int angle) {
  irServo.write(90 + angle);
}
