// ShowEncoders - Show the analog values of the voltages from the
//     wheel encoders.

// An example for the Spring 2015 VMS Robotics class.

// Set up the serial output and wait for it to initialize.
void setup() {
  Serial.begin(115200);
}

// Forever, display the most recently read values for the two
// encoders, assumed to be on analog pins 0 (left) and 1 (right).
void loop() {
  Serial.print("left=");
  Serial.print(analogRead(0));
  Serial.print(" right=");
  Serial.print(analogRead(1));
  Serial.println();
  delay(500);
}
