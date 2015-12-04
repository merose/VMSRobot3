// RemoteControl - Control the robot throught the Bluetooth interface.

#include <Servo.h>
#include <VMSRobot3.h>

const float WHEEL_SEPARATION = 0.140;
const float MAX_RADIANS_PER_SECOND = 3.0;

// Set up the robot for driving, and set up the onboard LED as an output.
void setup () {
  initRobot();

  pinMode(ONBOARD_LED_PIN, OUTPUT);
  setSpeed(0, 0);
  Goble.begin();
}

// Whenever we receive information from the GoBLE app, set the robot
// speed according to the joystick value, and blink the LED if button
// one is pressed.
void loop (){
  if (Goble.available()) {
    int joystickX = Goble.readJoystickX();  
    int joystickY = Goble.readJoystickY();

    if (Goble.readSwitchUp() == PRESSED) {
      digitalWrite(ONBOARD_LED_PIN, HIGH);
    } else {
      digitalWrite(ONBOARD_LED_PIN, LOW);
    }

    int speed = 2*joystickX - 256;
    float turnRate = -(2*joystickY - 256)/256.0;

    int leftSpeed = speed - turnRate*255/2;
    int rightSpeed = speed + turnRate*255/2;

    leftSpeed = constrain(leftSpeed, -255, 255);
    rightSpeed = constrain(rightSpeed, -255, 255);
    
    setSpeed(leftSpeed, rightSpeed);
  }
}
