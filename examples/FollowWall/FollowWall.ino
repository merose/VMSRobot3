// FollowWall - Avoids obstacles based on the IR sensor value, panning
//     it back and forth to detect obstacles, and tries to follow walls
//     on its right. This is an extension of the Wander example.

// An example for the VMS Robotics elective.

#include <Servo.h>
#include <VMSRobot3.h>

// The angles we should sense, and the danger thresholds for each angle.
const int IR_ANGLES[] = {30, -15, -60};
const int IR_THRESHOLD[] = {400, 400, 400};
const int IR_ANGLE_COUNT = sizeof(IR_ANGLES) / sizeof(IR_ANGLES[0]);

int curAngleIndex = 0;
int angleDirection = 1;

// Initialize the robot and wait for switch S1 to be pressed.
void setup() {
  Serial.begin(115200);
  initRobot();

  while (readSwitch() != 1) {
    // Do nothing.
  }
}

// Forever, if there is an obstacle ahead, stop and turn. Otherwise drive
//     forward and move the servo to a new angle.
void loop() {
  delay(100); // Wait for a good IR reading.
  int value = analogRead(IR_PIN);
  
  // If we are at the rightmost angle, try to follow the wall. Otherwise,
  // Just avoid obstacles.
  if (curAngleIndex == IR_ANGLE_COUNT-1) {
    // Follow wall
    if (value > 250) {
      // Turn left
      Serial.println("too close");
      setSpeed(65, 120);
    } else if (value < 150) {
      // Turn right
      Serial.println("too far");
      setSpeed(120, 65);
    } else {
      Serial.println("good");
      setSpeed(100, 100);
    }
  } else if (value >= IR_THRESHOLD[curAngleIndex]) {
    Serial.print("danger: ");
    Serial.println(value);
    turnLeft();
    setSpeed(100, 100);
  }
  
  curAngleIndex += angleDirection;
  if (curAngleIndex >= IR_ANGLE_COUNT) {
    angleDirection = -1;
    curAngleIndex = IR_ANGLE_COUNT - 2;
  } else if (curAngleIndex < 0) {
    angleDirection = 1;
    curAngleIndex = 1;
  }

  setServoAngle(IR_ANGLES[curAngleIndex]);
  delay(60);
}

// Stop, then pivot left for a while, and stop again.
void turnLeft() {
  setSpeed(0, 0);
  delay(100);
  setSpeed(-80, 80);
  delay(1000);
  setSpeed(0, 0);
  delay(100);
}

// Finds an angle where it is clear ahead.
int findClearAngle() {
  setServoAngle(-60);
  delay(500);
  
  for (int angle=-60; angle<=60; angle += 5) {
    setServoAngle(angle);
  }
}
