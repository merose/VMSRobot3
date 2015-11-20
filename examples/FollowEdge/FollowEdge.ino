// FollowEdge - Follow a table edge without falling off.

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
  int foundLeftEdge = !digitalRead(LEFT_TRACK_SENSOR_PIN);
  int foundRightEdge = !digitalRead(RIGHT_TRACK_SENSOR_PIN);

  if (foundRightEdge) {
    // We've found the table edge on the right, turn slightly to the
    // left and continue.
    setSpeed(-150, 0);
    delay(250);
  } else if (foundLeftEdge) {
    // We've found the table edge on the left. Turn more sharply left
    // so we can try to approach it on the right.
    setSpeed(-150, 0);
    delay(1000);
  } else {
    setSpeed(150, 100);
  }

  delay(10);
}
