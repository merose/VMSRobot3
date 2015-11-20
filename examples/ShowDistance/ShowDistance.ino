// ShowDistance - Show the distance to an object based on an IR sensor.

// An example for the VMS Robotics elective.

#include <Servo.h>
#include <VMSRobot.h>

// The analog pin for IR sensor.
const int IR_PIN = 4;

// The pin for a servo for the IR sensor.
const int IR_SERVO_PIN = 9;

Servo irServo;

// Set up the serial port and wait for it to initialize, and center the servo.
void setup() {
  Serial.begin(115200);
  irServo.attach(IR_SERVO_PIN);
  irServo.write(90);
  delay(1000);
  irServo.detach();
}

// Forever, read the IR sensor value and display it to the serial port.
void loop() {
  Serial.print("Value=");
  Serial.print(getIRValue());
  Serial.println();

  delay(250);
}

// Gets the IR sensor value, reading the sensor twice and returning the smaller
// value. This helps eliminates random errors.
int getIRValue() {
  int v1 = analogRead(IR_PIN);
  delay(50);
  int v2 = analogRead(IR_PIN);

  return min(v1, v2);
}
