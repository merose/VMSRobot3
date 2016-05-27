#ifndef RobotPose_h
#define RobotPose_h

/**
 * Keeps track of the robot's pose (position and heading).
 */
class RobotPose {
  
 public:
  
  /**
   * Creates a new instance of the post. The initial location is
   * (0,0) with a heading of zero degrees. The location and actual
   * heading may be set by calling setLocation() and setHeading().
   *
   * @param wheelDiameter the diameter of the wheels
   * @param wheelSeparation the separation between the two wheels
   * @param ticksPerRev the number of encoder ticks per revolution of a wheel
   */
  RobotPose(float wheelDiameter, float wheelSeparation, float ticksPerRev);
    
  /**
   * Gets the current, calculated heading. The heading is updated by reading
   * the wheel encoders.
   *
   * @return the current robot heading
   */
  float getHeading();
    
  /**
   * Gets the X coordinate of the calculated robot position. The robot position
   * is updated by reading the wheel encoders and the current heading.
   *
   * @return the robot X coordinate
   */
  float getX();
    
  /**
   * Gets the Y coordinate of the calculated robot position. The robot position
   * is updated by reading the wheel encoders and the current heading.
   *
   * @return the robot Y coordinate
   */
  float getY();

  float getGoalHeading(float goalX, float goalY);

  float getGoalAngle(float goalX, float goalY);

  float getGoalDistance(float goalX, float goalY);
  
  /**
   * Sets the assumed heading.
   *
   * @param newHeading the new heading angle, in degrees
   */
  void setHeading(float newHeading);

  /**
   * Sets the assumed position.
   *
   * @param x the x coordinate
   * @param y the y coordinate
   */
  void setPosition(float x, float y);

  /**
   * Gets the current speed, as calculated from the last call to updatePose().
   *
   * @return the current speed
   */
  float getSpeed();

  /**
   * Gets the current curvature of the robot's path. A positive curvature
   * means that the heading is increasing, that is, that the robot is
   * turning left. A negative turning radius means that the robot is
   * turning right. A turning radius of +INF or -INF means the robot is
   * turning in place
   *
   * @return the current curvature
   */
  float getCurvature();

  /**
   * Updates the calculated robot pose (position and heading) based on the
   * ticks counts from the wheel encoders. This function should be called
   * frequently, at least as often as the motor speeds are updated.
   *
   * @param leftTicks the current tick count for the left wheel encoder
   * @param rightTicks the current tick count for the right wheel encoder
   */
  void updatePose(int leftTicks, int rightTicks);

  /**
   * Resets the rmeembered last tick counts to zero for both the left and
   * right wheel encoders. This function should be called whenever the
   * encoder counts are reset to zero.
   */
  void resetTicks();
  
  /**
   * Gets the difference between two angles, normalized to a delta angle
   * between -180 and +180 degrees.
   *
   * @param angle1 the first angle
   * @param angle2 the second angle
   * @return the difference between the two angles
   */
  float angleDifference(float angle1, float angle2);
    
 private:
  
  float distancePerTick;
  float anglePerTick;
    
  float heading;

  float speed;
  float curvature;
    
  int lastLeftTicks;
  int lastRightTicks;

  unsigned long lastUpdateTime;

  float x;
  float y;
    
  /**
   * Normalizes an angle value to be between 0 and +360 degrees.
   *
   * @param angle the original angle
   * @return an equivalent angle, normalized to be between 0 and +360 degrees
   */
  float normalizeAngle(float angle);
  
};

#endif RobotPose_h
