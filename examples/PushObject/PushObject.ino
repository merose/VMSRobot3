// PushObject - Try to find an object within range and push it, without
//     falling off the table.

// An example for the VMS Robotics elective.

#include <Servo.h>
#include <VMSRobot3.h>


// Perform one-time setup. We must call initRobot() to set up the
// I/O pins and center the servo.

void setup() {
  Serial.begin(115200);
  initRobot();

  int switchValue;
  do {
    switchValue = readSwitch();
  } while (switchValue == 0);
}


// Forever, drive forward unless we see an edge, in which case move to
// stay on the table.f

void loop() {
  delay(20);
  
  int foundLeftEdge = !digitalRead(LEFT_TRACK_SENSOR_PIN);
  int foundRightEdge = !digitalRead(RIGHT_TRACK_SENSOR_PIN);

  if (foundRightEdge) {
    // We've found the table edge on the right, turn to the left.
    setSpeed(-150, 0);
    setServoAngle(60);
    delay(random(1000,2500));
  } else if (foundLeftEdge) {
    // We've found the table edge on the left. Turn to the right.
    setSpeed(0, -150);
    setServoAngle(60);
    delay(random(1000,2500));
  } else {
    // We're not at a table edge. Check to see if we see an object.
    int value = analogRead(IR_PIN);
    int curAngle = getServoAngle();

    if (value > 250) {
      // We see an object. Go toward it and turn the servo slightly more
      // to the left.
      curAngle = curAngle + 2.0;
    } else {
      // No object. Turn the servo to the right.
      curAngle = curAngle - 2.0;
    }

    // The difference between the two motors is 1.5 times the current
    // servo angle, in degrees, to turn more sharply the more
    // the servo is turned away from center.
    int motorDifference = 3*curAngle/2;
    setSpeed(160 - motorDifference, 160 + motorDifference);

    // Keep the servo within 60 degrees of center.
    curAngle = constrain(curAngle, -60, 60);
    setServoAngle(curAngle);
  }
}
