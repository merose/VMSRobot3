// Wander - Avoids obstacles based on the IR sensor value, panning it back
//     and forth to detect obstacles.

// An example for the VMS Robotics elective.

#include <Servo.h>
#include <VMSRobot3.h>

// The angles we should sense, and the danger thresholds for each angle.
const int IR_ANGLES[] = {60, 15, -15, -60};
const int IR_THRESHOLD[] = {400, 400, 400, 400};
const int IR_ANGLE_COUNT = sizeof(IR_ANGLES) / sizeof(IR_ANGLES[0]);

int curAngleIndex = 0;
int angleDirection = 1;

int dangerCount = 0;

// Set up the serial port and wait for it to initialize, and center the servo.
void setup() {
  Serial.begin(115200);
  initRobot();
  
  setServoAngle(IR_ANGLES[curAngleIndex]);
  delay(1000);
  waitForSwitch(1);
}

// Forever, if there is an obstacle ahead, stop and turn. Otherwise drive
//     forward and move the servo to a new angle.
void loop() {
  delay(100); // Wait for a good IR value.
  int value = analogRead(IR_PIN);
  if (value >= IR_THRESHOLD[curAngleIndex]) {
    delay(50);
    value = min(value, analogRead(IR_PIN));
  }
  
  if (value < IR_THRESHOLD[curAngleIndex]) {
    setSpeed(150, 150);
    dangerCount = 0;
  } else if (++dangerCount >= 1) {
    Serial.print("Obstacle at angle ");
    Serial.print(IR_ANGLES[curAngleIndex]);
    Serial.println();
    turnLeft();
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
