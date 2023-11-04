const int buttonPin = 2;  
const int ledPin = 13;    

volatile bool buttonPressed = false; 

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); 
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonInterrupt, FALLING); // Trigger interrupt when detect button is pressed
}

void loop() {
}

void buttonInterrupt() {
  digitalWrite(ledPin, HIGH);  
  delay(1000); 
  digitalWrite(ledPin, LOW);   
}
