// FollowWall - Avoids obstacles based on the IR sensor value, panning
//     it back and forth to detect obstacles, and tries to follow walls
//     on its right. This is an extension of the Wander example.

// An example for the VMS Robotics elective.

#include <Servo.h>
#include <VMSRobot3.h>

// The distance for a dangerous object.
const int DANGER_THRESHOLD = 90;

Servo irServo;

// The angles we should sense, and the danger thresholds for each angle.
const int IR_ANGLES[] = {45, 0, -45};
const int IR_THRESHOLD[] = {150, 130, 150};
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
  int value = getIRValue();
  
  // If we are at the rightmost angle, try to follow the wall. Otherwise,
  // Just avoid obstacles.
  if (curAngleIndex == IR_ANGLE_COUNT-1) {
    // Follow wall
    if (value > 80) {
      // Turn left
      setSpeed(65, 120);
    } else if (value < 70) {
      // Turn right
      setSpeed(120, 65);
    } else {
      setSpeed(100, 100);
    }
  } else if (value >= IR_THRESHOLD[curAngleIndex]) {
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
  delay(75);
}

// Stop, then pivot left for a while, and stop again.
void turnLeft() {
  setSpeed(0, 0);
  delay(100);
  setSpeed(-60, 60);
  delay(1000);
  setSpeed(0, 0);
  delay(100);
}

// Gets the IR sensor value, reading the sensor three times, and returning
// the median of the three values. This median filter method eliminates
// stray values that sometimes result.
int getIRValue() {
  int v1 = analogRead(3);
  delay(20);
  int v2 = analogRead(3);
  delay(20);
  int v3 = analogRead(3);
  
  if (v2 <= v1 && v1 <= v3) {
    return v1;
  } else if (v3 <= v1 && v1 <= v3) {
    return v1;
  } else if (v1 <= v2 && v2 <= v3) {
    return v2;
  } else if (v3 <= v2 && v2 <= v1) {
    return v2;
  } else {
    return v3;
  }
}

// Finds an angle where it is clear ahead.
int findClearAngle() {
  setServoAngle(-60);
  delay(500);
  
  for (int angle=-60; angle<=60; angle += 5) {
    setServoAngle(angle);
  }
}

// Waits for switch S1 to be depressed.
void waitForSwitch1() {
  while (analogRead(0) > 75) {
    // do nothing
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
  irServo.write(90 + angle - 7);
}

// Reads the value of the built-in switches S1 through S5. The switches
// are connected in parallel with resistors of five different sizes to
// analog pin A0. The difference in voltage at A0 allows us to determine
// which switch is pressed.
int readSwitch() {
  int value = analogRead(0);

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

