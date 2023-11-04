const int buttonPin = 2;  // Push button pin (interrupt pin 0 for Arduino Uno/Nano)
const int ledPin = 13;    // LED pin

volatile bool buttonPressed = false; // Flag to indicate if the button is pressed

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); // Use internal pull-up resistor
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonInterrupt, FALLING); // Trigger interrupt on falling edge (button press)
}

void loop() {
  
}

void buttonInterrupt() {
  Serial.println("Button is pressed - LED flashed!");
  digitalWrite(ledPin, HIGH);  // Turn on the LED
  delay(500); // Wait for 0.5 second
  digitalWrite(ledPin, LOW);   // Turn off the LED
}
