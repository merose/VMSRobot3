// FindTarget - Pans a servo back and forth until a close target is found.

// An example for the VMS Robotics elective.

#include <Servo.h>
#include <VMSRobot3.h>

// The maximum angle off of center to move to.
const int MAX_ANGLE = 60;

// How much to turn the servo in one step.
const int DELTA_ANGLE = 5;

// What value from the infrared sensor indicates a target present.
const int IR_THRESHOLD_VALUE = 400;

int curAngle = 0;
int deltaAngle = DELTA_ANGLE;

// Start the serial port and attach to the servo control pin.
void setup() {
  Serial.begin(115200);
  irServo.attach(9);
  
  delay(1000);
  setServoAngle(curAngle);
  delay(1000);
}

// Forever, turn the servo and try to find a target. If there is no target,
//     adjust the servo angle. If the angle is too big or too small,
//     change the panning direction.
void loop() {
  setServoAngle(curAngle);
  
  delay(50); // Wait for a good IR value.
  int value = analogRead(IR_PIN);
  if (value >= IR_THRESHOLD_VALUE) {
    delay(50);
    value = min(value, analogRead(IR_PIN));
  }

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
