// Waypoints - Drive between specified coordinates locations.

#include <Servo.h>
#include <VMSRobot3.h>

struct Point {
  float x, y;
};
  
// YOUR CODE STARTS HERE

// Modify the middle number here to match the distance between
// the whee centers on your robot, in meters. It will likely
// be about 190mm, or 0.190 meters.
//
// The three numbers are the wheel diameter (69mm), the wheel-to-
// wheel distance, and the number of stripes on each encoder disk.

RobotPose pose(0.069, 0.141, 30);

// Modify the path you want your robot to travel by changing these
// points or adding more. All coordinates are in meters. The
// points given below cause the robot to travel in a square,
// clockwise.

struct Point waypoints[] = {
  { 1.0, 0.0 },
  { 1.0, -1.0 },
  { 0.0, -1.0 },
  { 0.0, 0.0 },
};
const int NUM_WAYPOINTS = sizeof(waypoints) / sizeof(waypoints[0]);

// The proportional constant for controlling how rapidly
// the robot responds to a deviation in heading. You can
// increase or decrease this constant to see how it affects
// the robot's behavior, if you like.
const float Kp = 4.0;

// YOUR CODE ENDS HERE

AnalogScanner scanner;
AnalogEncoder leftEncoder;
AnalogEncoder rightEncoder;

const int LEFT_DIRECTION = 4;
const int LEFT_SPEED = 5;
const int RIGHT_SPEED = 6;
const int RIGHT_DIRECTION = 7;

const int MAX_SPEED_DIFFERENCE = 100;
const int NORMAL_SPEED = 140;

float desiredHeading = 0.0;

int curWaypoint;

void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  
  pinMode(LEFT_DIRECTION, OUTPUT);
  pinMode(LEFT_SPEED, OUTPUT);
  pinMode(RIGHT_DIRECTION, OUTPUT);
  pinMode(RIGHT_SPEED, OUTPUT);
  
  int scanOrder[] = {A0, A1, A7};
  scanner.setScanOrder(sizeof(scanOrder) / sizeof(scanOrder[0]), scanOrder);
  scanner.setCallback(A0, updateEncoder);
  scanner.setCallback(A1, updateEncoder);
  scanner.beginScanning();
  delay(100);

  waitForSwitch1();
  delay(100);
  leftEncoder.resetTicks();
  rightEncoder.resetTicks();
  
  setSpeed(NORMAL_SPEED, NORMAL_SPEED);  
  curWaypoint = 0;
}

void loop() {
  controlMotors();
  delay(200);
}

// If we have not yet reached the next waypoint, adjust the motor speeds
// depending on our heading error. Otherwise switch to a new waypoint.
// When we reach the last waypoint, turn off the motors and idle forever.
void controlMotors() {
  Point dest = waypoints[curWaypoint];
  long leftTicks = leftEncoder.getTicks();
  long rightTicks = rightEncoder.getTicks();
  pose.updatePose(leftTicks, rightTicks);
  
  // We have achieved the waypoint if we are within .1 meters (4").
  if (pose.getGoalDistance(dest.x, dest.y) < 0.1) {
    Serial.print("Accomplished waypoint ");
    Serial.print(curWaypoint);
    Serial.print(": <");
    Serial.print(dest.x);
    Serial.print(", ");
    Serial.print(dest.y);
    Serial.println(">");
    
    // If there are no more waypoints, stop and idle forever.
    if (++curWaypoint >= NUM_WAYPOINTS) {
      Serial.println("Finished.");
      setSpeed(0, 0);
      for (;;) {
        // forever, do nothing
      }
    }
  }

  // Otherwise, update the desired heading and control the motors.
  desiredHeading = pose.getGoalHeading(dest.x, dest.y);
  updateMotors(leftTicks, rightTicks);
}

// Determine the heading error between our desired heading and actual
// heading. Then, update the motor speeds to turn left or right, as needed,
// using a proportional controller.
void updateMotors(long leftTicks, long rightTicks) {
  float headingError = angleDifference(desiredHeading, pose.getHeading());
  int motorDifference = constrain(Kp * headingError, -MAX_SPEED_DIFFERENCE, MAX_SPEED_DIFFERENCE);

  Serial.print("lticks=");
  Serial.print(leftTicks);
  Serial.print("  rticks=");
  Serial.print(rightTicks);
  Serial.print("  x: ");
  Serial.print(pose.getX());
  Serial.print("  y: ");
  Serial.print(pose.getY());
  Serial.print("  desired heading: ");
  Serial.print(desiredHeading);
  Serial.print("  actual heading: ");
  Serial.print(pose.getHeading());
  Serial.print("  heading error: ");
  Serial.print(headingError);
  Serial.print("  motor difference: ");
  Serial.println(motorDifference);
  
  setSpeed(NORMAL_SPEED - motorDifference, NORMAL_SPEED + motorDifference);
}

// Waits until built-in switch S1 is pressed.
void waitForSwitch1() {
  while (scanner.getValue(7) > 75) {
    // do nothing
  }
}

// Determines the difference between two heading angles such that we always get
// a result between -180 and +180 degrees.
float angleDifference(float angle1, float angle2) {
  angle1 = normalizeAngle(angle1);
  angle2 = normalizeAngle(angle2);

  float delta = normalizeAngle(angle1 - angle2);
  if (delta > 180.0) {
    return delta - 360.0;
  } else {
    return delta;
  }
}

// Normalizes a heading angle so that it lies between 0 and 360 degrees.
float normalizeAngle(float angle) {
  while (angle < 0.0) {
    angle += 360.0;
  }
  while (angle > 360.0) {
    angle -= 360.0;
  }
  
  return angle;
}

void updateEncoder(int index, int pin, int value) {
  if (pin == A0) {
    leftEncoder.update(value);
  } else {
    rightEncoder.update(value);
  }
}
