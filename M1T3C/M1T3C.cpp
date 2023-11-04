const int buttonPin = 2;   
const int pirPin = 3;   
const int ledButtonPin = 12; 
const int ledMotionPin = 13; 

void setup() {
  pinMode(ledButtonPin, OUTPUT);
  pinMode(ledMotionPin, OUTPUT);
  pinMode(pirPin, INPUT_PULLUP); 
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
  // Button triggers Blue LED, Motion sensor triggers RED LED
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonInterrupt, FALLING); // Trigger interrupt on button press
  attachInterrupt(digitalPinToInterrupt(pirPin), pirInterrupt, RISING); // Trigger interrupt on motion detected
}

void loop() {
}

void buttonInterrupt() {
  Serial.println("Button is pressed - Blue LED flashed!");
  digitalWrite(ledButtonPin, HIGH); 
  delay(500); 
  digitalWrite(ledButtonPin, LOW); 
}

void pirInterrupt() {
  Serial.println("Motion detected - RED LED flased!");
  digitalWrite(ledMotionPin, HIGH);
  delay(500); 
  digitalWrite(ledMotionPin, LOW); 
}
