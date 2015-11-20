// ShowScene - When requested, analyze distances over a range of angles
//     and show a graphic display of the scene.

// An example for the VMS Robotics elective.

#include <Servo.h>
#include <VMSRobot3.h>

// The maximum angle off of center to move to.
const int MAX_ANGLE = 60;

// How much to turn the servo in one step.
const int DELTA_ANGLE = 5;

int curAngle = 0;
int deltaAngle = DELTA_ANGLE;

const int MAX_X = 35;
const int MAX_Y = 25;
const int X_SCALE = 50.0;
const int Y_SCALE = 50.0;

int minDistance[2*MAX_X + 1];


// Start the serial port and attach to the servo control pin.
void setup() {
  Serial.begin(115200);
  initRobot();

  delay(1000);
}


// Forever, wait for the switch to be pressed and then scan the scene.
// Convert readings into a picture of what the robot sees and display
// it on the output.

void loop() {
  while (readSwitch() != 1) {
    // Do nothing.
  }
  
  showScene();
}


// Show the scene of what the robot sees.
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
    float distance = valueToDistance(value);

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

// Linear fit of voltage vs. 1/distance based on Sharp data sheet
// for 10-80cm sensor. The 5/1024 factor is to convert from analog
// input value to voltage. (Fitted 1/d = m*V + b: m=0.05, b=-0.14)
const float M = 0.05 * 5.0 / 1024.0;
const float B = -0.014;

float valueToDistance(int value) {
  float invDistance = max(M*value + B, 1E-6);
  return min(.8, .01/invDistance);
}
