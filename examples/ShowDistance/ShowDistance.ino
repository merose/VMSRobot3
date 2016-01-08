// ShowDistance - Show the distance to an object based on an IR sensor.

// An example for the VMS Robotics elective.

#include <Servo.h>
#include <VMSRobot3.h>

// Set up the serial port and wait for it to initialize, and center the servo.
void setup() {
  Serial.begin(115200);
  initRobot();
}

// Forever, read the IR sensor value and display it to the serial port.
void loop() {
  Serial.print("Value=");
  Serial.print(analogRead(IR_PIN));
  Serial.println();

  delay(100);
}
