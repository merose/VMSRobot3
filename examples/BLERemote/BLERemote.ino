//

#include <Servo.h>
#include <VMSRobot3.h>

int ledPin = 13;

void setup (){
  Serial.begin(115200);
  pinMode(M1_DIRECTION, OUTPUT);
  pinMode(M2_DIRECTION, OUTPUT);
  pinMode(ledPin,OUTPUT);
  setSpeed(0, 0);
  Goble.begin();
}

void loop (){
  if (Goble.available()) {
    int joystickX = Goble.readJoystickX();  
    int joystickY = Goble.readJoystickY();

    int leftSpeed = 2*joystickX - 256;
    int rightSpeed = leftSpeed;
    int turnBias = 2*joystickY - 256;

    Serial.print("Base speed: ");
    Serial.print(leftSpeed);
    Serial.print("  bias: ");
    Serial.print(turnBias);

    leftSpeed += turnBias;
    rightSpeed -= turnBias;

    if (leftSpeed < -255) {
      int diff = -255 - leftSpeed;
      leftSpeed += diff;
      rightSpeed += diff;
    }
    if (leftSpeed > 255) {
      int diff = leftSpeed - 255;
      leftSpeed -= diff;
      rightSpeed -= diff;
    }
    if (rightSpeed < -255) {
      int diff = -255 - rightSpeed;
      leftSpeed += diff;
      rightSpeed += diff;
    }
    if (rightSpeed > 255) {
      int diff = rightSpeed - 255;
      leftSpeed -= diff;
      rightSpeed -= diff;
    }
    leftSpeed = constrain(leftSpeed, -255, 255);
    rightSpeed = constrain(rightSpeed, -255, 255);

    Serial.print("  left: ");
    Serial.print(leftSpeed);
    Serial.print(" right: ");
    Serial.print(rightSpeed);
    Serial.println();

    setSpeed(leftSpeed, rightSpeed);

    if (Goble.readSwitchUp() == PRESSED) {
        digitalWrite(ledPin,HIGH);  
    } else if (Goble.readSwitchUp() == RELEASED){
        digitalWrite(ledPin,LOW);
    }
  }
}
