// AvoidDanger - Wander around without falling off a table, and turn
//     left away from an obstacle straight ahead.

#include <Servo.h>
#include <VMSRobot3.h>

// 
void setup() {
  Serial.begin(115200);
  initRobot();

  while (readSwitch() != 1) {
    // Do nothing.
  }
}

// Forever, try to drive forward. If an edge is detected turn safely
// away. If an obstacle is in front, turn to the left.
void loop() {
  if (!digitalRead(LEFT_TRACK_SENSOR_PIN)) {
    // Hit edge on left. Spin back round right wheel.
    setSpeed(-128, 0);
    delay(2200);
  } else if (!digitalRead(RIGHT_TRACK_SENSOR_PIN)) {
    // Hit edge on right. Spin back around left wheel.
    setSpeed(0, -128);
    delay(2200);
  } else if (analogRead(IR_PIN) >= 375) {
    // Obstacle straight ahead. Pivot left.
    setSpeed(-128, 128);
    delay(750);
  } else {
    // No obstacles, go straight.
    setSpeed(128, 128);
  }
}
