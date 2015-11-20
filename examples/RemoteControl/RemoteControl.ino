// RemoteControl - Drive a robot using the laptop keyboard.
//
// An example for the Spring 2015 VMS Robotics elective.
//
// Keys for controlling the robot (all have to be followed by Enter):
//
// \   start/stop
// [   turn left
// ]   turn right
// =   straight ahead, and start if stopped
// 1   slow speed
// 2   medium slow
// 3   medium fast
// 4   fast
// '   reverse direction (stop first, to avoid straining the motors)

// The pins for motor control on the Romeo v2.
const int M1_DIRECTION = 4;
const int M1_SPEED = 5;
const int M2_SPEED = 6;
const int M2_DIRECTION = 7;

// The current direction and relative speed.
float direction = 1.0;
float speed = 1.0;

// The current driving mode.
enum {STOPPED, STRAIGHT, TURN_LEFT, TURN_RIGHT} driveMode = STOPPED;

// Set up the serial communication, and set the pin modes for the motors.
void setup() {
  Serial.begin(38400);

  pinMode(M1_DIRECTION, OUTPUT);
  pinMode(M1_SPEED, OUTPUT);
  pinMode(M2_DIRECTION, OUTPUT);
  pinMode(M2_SPEED, OUTPUT);
}

// Forever, read a command, if available, change speeds if commanded,
// and control the motors.
void loop() {
  int c = readCommand();
  if (c == '\\') {
    if (driveMode == STOPPED) {
      driveMode = STRAIGHT;
    } else {
      driveMode = STOPPED;
    }
  } else if (c == '=') {
    direction = 1.0;
    driveMode = STRAIGHT;
  } else if (c=='\'' || c=='/') {
    direction = -1.0;    
    driveMode = STRAIGHT;
  } else if (driveMode!=STOPPED && c=='[') {
    driveMode = TURN_LEFT;
  } else if (driveMode!=STOPPED && c==']') {
    driveMode = TURN_RIGHT;
  } else if (driveMode!=STOPPED && c=='1') {
    speed = 0.25;
  } else if (driveMode!=STOPPED && c=='2') {
    speed = 0.5;
  } else if (driveMode!=STOPPED && c=='3') {
    speed = 0.75;
  } else if (driveMode!=STOPPED && c=='4') {
    speed = 1.0;
  }
  
  switch (driveMode) {
    case STRAIGHT:
      Serial.print("mode=STRAIGHT speed=");
      Serial.println(speed);
      setSpeed(direction*speed*255, direction*speed*255);
      break;
    case TURN_LEFT:
      Serial.print("mode=TURN_LEFT speed=");
      Serial.println(speed);
      setSpeed(direction*0.25*speed*255, direction*speed*255);
      break;
    case TURN_RIGHT:
      Serial.print("mode=TURN_RIGHT speed=");
      Serial.println(speed);
      setSpeed(direction*speed*255, direction*0.25*speed*255);
      break;
    case STOPPED:
      Serial.println("mode=STOPPED");
      setSpeed(0, 0);
      break;
  }
  
  delay(250);
}

// Reads a single-character command from the keyboard, followed by Enter.
int readCommand() {
  int c = Serial.read();
  while (Serial.available()) {
    Serial.read();
  }
  
  return c;
}

// Sets the speed of the two motors. The speed of each motor is specified
// as an integer from -255 (full reverse) to +255 (full forward).
void setSpeed(int leftSpeed, int rightSpeed) {
  digitalWrite(M1_DIRECTION, (leftSpeed >= 0 ? HIGH : LOW));
  analogWrite(M1_SPEED, abs(leftSpeed));
  digitalWrite(M2_DIRECTION, (rightSpeed >= 0 ? HIGH : LOW));
  analogWrite(M2_SPEED, abs(rightSpeed));
}
