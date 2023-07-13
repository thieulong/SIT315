int led = 13;                // the pin that the LED is attached to
int sensor = 2;              // the pin that the sensor is attached to
int state = LOW;             // by default, no motion detected
int val = 0;                 // variable to store the sensor status (value)

void setup() {
  pinMode(led, OUTPUT);      // initialize LED as an output
  pinMode(sensor, INPUT);    // initialize sensor as an input
  Serial.begin(9600);        // initialize serial
}

void loop() {
  val = digitalRead(sensor);   // read sensor value
  if (val == HIGH) {           // check if the sensor is HIGH
    if (state == LOW) {
      Serial.println("Movement detected! - LED is ON");
      state = HIGH;       // update variable state to HIGH
      digitalWrite(led, HIGH);  // turn the LED on
    }
  } else {
    if (state == HIGH) {
      Serial.println("No movement found! - LED is OFF");
      state = LOW;       // update variable state to LOW
      digitalWrite(led, LOW);  // turn the LED off
    }
  }
  delay(1000);
}
