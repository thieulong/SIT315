const int buttonPin = 2;  // Push button pin
const int ledPin = 13;    // LED pin

volatile int buttonState = LOW;  

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);  
  pinMode(ledPin, OUTPUT);           
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonInterrupt, CHANGE);  // Attach the interrupt to the button pin and keep track of the status of the button
}

void loop() {
}

void buttonInterrupt() {
  buttonState = digitalRead(buttonPin);  // Read the button's status
  if (buttonState == 0){
    Serial.println("Button is pressed -> LED is ON");
  } else if (buttonState == 1){
    Serial.println("Button is released -> LED is OFF");
  }
  digitalWrite(ledPin, buttonState);     // Set the LED on if the button is pressed and vice versa
}
