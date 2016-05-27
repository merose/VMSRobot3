#include <Arduino.h>
#include "RobotPose.h"

// Creates a new instance and calculates the distance per tick and the
// delta heading angle per tick.
RobotPose::RobotPose(
  float wheelDiameter,
  float wheelSeparation,
  float ticksPerRev
) {
  distancePerTick = wheelDiameter * PI / ticksPerRev;
  anglePerTick = distancePerTick / wheelSeparation * 180.0 / PI;
  
  heading = 0;
  x = 0;
  y = 0;
  speed = 0.0;
  curvature = 0.0;
  lastUpdateTime = micros();
}

// Gets the current heading.
float RobotPose::getHeading() {
  return heading;
}

// Gets the X position.
float RobotPose::getX() {
  return x;
}

// Gets the Y position.
float RobotPose::getY() {
  return y;
}

// Sets the assumed heading.
void RobotPose::setHeading(float newHeading) {
  heading = newHeading;
}

// Sets the assumed robot position.
void RobotPose::setPosition(float x, float y) {
  this->x = x;
  this->y = y;
}

// Gets the robot's current speed.
float RobotPose::getSpeed() {
  return speed;
}

// Gets the current curvature of the robot's path.
float RobotPose::getCurvature() {
  return curvature;
}

// Updates the robot's pose based on the new tick counts from the left
// and right wheel encoders.
void RobotPose::updatePose(int leftTicks, int rightTicks) {
  unsigned long now = micros();

  int deltaLeftTicks = leftTicks - lastLeftTicks;
  int deltaRightTicks = rightTicks - lastRightTicks;
  
  lastLeftTicks = leftTicks;
  lastRightTicks = rightTicks;

  int diffTicks = deltaRightTicks - deltaLeftTicks;
  float deltaHeading = (float) diffTicks * anglePerTick;
  
  // Update x and y before updating the heading.
  float distance = (float) (deltaRightTicks + deltaLeftTicks) / 2.0
    * distancePerTick;
  x += distance * cos(heading * PI / 180.0);
  y += distance * sin(heading * PI / 180.0);
  
  heading = normalizeAngle(heading + deltaHeading);

  speed = distance / (float) (now - lastUpdateTime);
  if (distance < distancePerTick / 10.0) {
    curvature = 0.0;
  } else {
    curvature = deltaHeading * PI / 180.0 / distance;
  }

  lastUpdateTime = now;
}

// Resets the tick counts to zero, and resets the calculated curvature.
void RobotPose::resetTicks() {
  lastLeftTicks = 0;
  lastRightTicks = 0;
  curvature = 0.0;
}

// Normalizes an angle to the range 0 to +360 degrees.
float RobotPose::normalizeAngle(float angle) {
  while (angle < 0.0) {
    angle += 360.0;
  }
  while (angle > 360.0) {
    angle -= 360.0;
  }
  
  return angle;
}

// Returns the absolute angle from the current position to the goal.
float RobotPose::getGoalHeading(float goalX, float goalY) {
  return atan2(goalY - y, goalX - x) * 180.0 / PI;
}

// Returns the angle from the current heading to the goal, in +/- 180 deg.
float RobotPose::getGoalAngle(float goalX, float goalY) {
  return angleDifference(getGoalHeading(goalX, goalY), heading);
}

float RobotPose::getGoalDistance(float goalX, float goalY) {
  float diffX = goalX - x;
  float diffY = goalY - y;
  return sqrt(diffX*diffX + diffY*diffY);
}

float RobotPose::angleDifference(float angle1, float angle2) {
  angle1 = normalizeAngle(angle1);
  angle2 = normalizeAngle(angle2);

  float delta = normalizeAngle(angle1 - angle2);
  if (delta > 180.0) {
    return delta - 360.0;
  } else {
    return delta;
  }
}
