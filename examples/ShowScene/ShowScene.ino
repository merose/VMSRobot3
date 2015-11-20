// ShowScene - When requested, analyze distances over a range of angles and
//     show a graphic display of the scene.

// An example for the VMS Robotics elective.

#include <Servo.h>

// The maximum angle off of center to move to.
const int MAX_ANGLE = 60;

// How much to turn the servo in one step.
const int DELTA_ANGLE = 5;

// What value from the infrared sensor indicates a target present.
const int IR_THRESHOLD_VALUE = 100;

Servo irServo;
int curAngle = 0;
int deltaAngle = DELTA_ANGLE;

const int MAX_X = 35;
const int MAX_Y = 25;
const int X_SCALE = 50.0;
const int Y_SCALE = 50.0;

int minDistance[2*MAX_X + 1];

// Start the serial port and attach to the servo control pin.
void setup() {
  Serial.begin(38400);
  irServo.attach(9);
  
  delay(1000);
  setServoAngle(0);
  delay(1000);
}

// Forever, turn the servo and try to find a target. If there is no target,
//     adjust the servo angle. If th angle is too big or too small,
//     change the panning direction.
void loop() {
  waitForSwitch1();
  showScene();
}

void showScene() {
  setServoAngle(MAX_ANGLE);
  delay(1000);

  for (int x = -MAX_X; x <= MAX_X; ++x) {
    minDistance[x + MAX_X] = MAX_Y + 1;
  }

  for (int curAngle = MAX_ANGLE; curAngle >= -MAX_ANGLE; --curAngle) {
    setServoAngle(curAngle);
    delay(50);

    int value = getIRValue();
    float distance = getDistance(value);

    int x = round(distance * sin(-curAngle/180.0*PI) * X_SCALE);
    int y = round(distance * cos(curAngle/180.0*PI) * Y_SCALE);

    Serial.print(curAngle);
    Serial.print("\t");
    Serial.print(value);
    Serial.print("\t");
    Serial.print(distance);
    Serial.print("\t");
    Serial.print(x);
    Serial.print("\t");
    Serial.print(y);
    Serial.println();

    if (-MAX_X <= x && x <= MAX_X) {
      if (y < minDistance[x + MAX_X]) {
        minDistance[x + MAX_X] = y;
      }
    }
  }

  Serial.println();
  Serial.print("+");
  for (int x = -MAX_X; x <= MAX_X; ++x) {
    Serial.print("-");
  }
  Serial.println("+");
  
  for (int y = MAX_Y; y >= 0; --y) {
    Serial.print("|");
    for (int x = -MAX_X; x <= MAX_X; ++x) {
      if (minDistance[x + MAX_X] == y) {
        Serial.print("*");
      } else {
        Serial.print(" ");
      }
    }
    Serial.println("|");
  }

  Serial.print("+");
  for (int x = -MAX_X; x <= MAX_X; ++x) {
    Serial.print("-");
  }
  Serial.println("+");
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

// Converts an IR analog value to a distance in meters. This is an approximate
// algorithm based on examining several Sharp sensors.
float getDistance(int irValue) {
  return 22.0 / irValue;
}

// Sets the servo angle, as an angle from center.
void setServoAngle(int angle) {
  irServo.write(90 + angle);
}

// Waits for switch S1 to be depressed.
void waitForSwitch1() {
  while (analogRead(0) > 75) {
    // do nothing
  }
}