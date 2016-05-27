// DriveStraight - Drive straight ahead, adjusting the motor speeds as needed.

#include <Servo.h>
#include <VMSRobot3.h>

AnalogScanner scanner;
AnalogEncoder leftEncoder;
AnalogEncoder rightEncoder;

const int LEFT_DIRECTION = 4;
const int LEFT_SPEED = 5;
const int RIGHT_SPEED = 6;
const int RIGHT_DIRECTION = 7;

const int MAX_SPEED_DIFFERENCE = 100;
const int NORMAL_SPEED = 140;

long lastLeftTicks = 0;
long lastRightTicks = 0;

unsigned long startTime;
unsigned long lastTime;

// Sets up the serial output, attaches the interrupt handlers to read the
// encoders, and starts moving when the user presses switch S1.
void setup() {
  Serial.begin(115200);
  delay(3000);
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

  Serial.print("Time\tL Ticks\tL Speed\tR Ticks\tR Speed\tL Motor\tR Motor\n");
  
  setSpeed(NORMAL_SPEED, NORMAL_SPEED);
  lastTime = startTime = millis();
}

// Forever, control the motors to drive straight ahead, delaying
// by a bit in each loop.
void loop() {
  delay(200);
  
  // YOUR CODE HERE
  
  // Change this proportional constant to make your robot
  // drive straight.
  const float Kp = 0.0;

  // END OF YOUR CODE

  long leftTicks = leftEncoder.getTicks();
  long rightTicks = rightEncoder.getTicks();

  unsigned long curTime = millis();
  
  // Choose a left and a right speed based on whether we have more ticks
  // on the left or the right.
  int diffTicks = leftTicks - rightTicks;
  int diffSpeed = round(Kp * diffTicks);

  int leftSpeed = constrain(NORMAL_SPEED - diffSpeed, 0, 255);
  int rightSpeed = constrain(NORMAL_SPEED + diffSpeed, 0, 255);
  
  // END ADD YOUR CODE HERE
  
  setSpeed(leftSpeed, rightSpeed);

  Serial.print((millis() - startTime) / 1000.0, 2);
  Serial.print("\t");
  Serial.print(leftTicks);
  Serial.print("\t");
//  Serial.print((leftTicks - lastLeftTicks) / (float) (curTime - lastTime) * 1000.0);
  Serial.print(leftTicks - lastLeftTicks);
  Serial.print("\t");
  Serial.print(rightTicks);
  Serial.print("\t");
//  Serial.print((rightTicks - lastRightTicks) / (float) (curTime - lastTime) * 1000.0);
  Serial.print(rightTicks - lastRightTicks);
  Serial.print("\t");
  Serial.print(leftTicks - rightTicks);
  Serial.print("\t");
  Serial.print(leftSpeed);
  Serial.print("\t");
  Serial.print(rightSpeed);
  Serial.println();

  lastTime = curTime;
  lastLeftTicks = leftTicks;
  lastRightTicks = rightTicks;
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
