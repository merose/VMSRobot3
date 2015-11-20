// DriveForward - Drive forward at maximum speed for a few seconds, then stop.
//
// An example for the Spring 2015 VMS Robotics elective.
//
// This example uses the two gear motors to drive the robot forward.
// No feedback is used, so whether or not the robot drives exactly
// straight depends on whether the motors are evenly matched.
//
// This program isolates the control of the motor pins into the setSpeed()
// function.

const int ONBOARD_SWITCH_PIN = A7;

// The pins for motor control on the Romeo v2.
const int M1_DIRECTION = 4;
const int M1_SPEED = 5;
const int M2_SPEED = 6;
const int M2_DIRECTION = 7;

void setup() {
  Serial.begin(115200);

  pinMode(M1_DIRECTION, OUTPUT);
  pinMode(M2_DIRECTION, OUTPUT);
  
  // Wait for the user to press S1.
  waitForSwitch1();

  // Drive forward with highest speed for five seconds.
  setSpeed(255, 255);
  delay(5000);
     
  // Stop the robot.
  setSpeed(0, 0);
}

void loop() {
  // Do nothing - all work is done in setup().
}

// Waits until switch "S1" is pressed on the Romeo board.
void waitForSwitch1() {
  while (readSwitch() != 1) {
    // do nothing
  }
}

// Reads the built-in switches on the Romeo and returns a value indicating
// which switch, if any, is depressed. The analog read value thresholds used
// were determined empirically and fall roughly mid-way between the actual
// value you can read for each switch.
//
// Return value:
//   0 - no switches
//   1 - S1
//   2 - S2
//   3 - S3
//   4 - S4
//   5 - S5
int readSwitch() {
  int value = analogRead(ONBOARD_SWITCH_PIN);
  
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

// Sets the speed of the two motors. The speed of each motor is specified
// as an integer from -255 (full reverse) to +255 (full forward).
void setSpeed(int leftSpeed, int rightSpeed) {
  digitalWrite(M1_DIRECTION, (leftSpeed >= 0 ? HIGH : LOW));
  analogWrite(M1_SPEED, abs(leftSpeed));
  digitalWrite(M2_DIRECTION, (rightSpeed >= 0 ? HIGH : LOW));
  analogWrite(M2_SPEED, abs(rightSpeed));
}
