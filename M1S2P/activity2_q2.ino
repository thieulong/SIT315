
//Initialize variables store button and LED pin 
const uint8_t BTN_PIN = 2;
const uint8_t LED_PIN = 13;

//Initialize variables store the state of button and LED
uint8_t buttonPrevState = LOW;
uint8_t ledState = LOW;

//Set up function, run once after executed
void setup()
{
  //Set pin mode for the button as input with pull-up resistor
  pinMode(BTN_PIN, INPUT_PULLUP);
  //Set pin mode for the LED as output
  pinMode(LED_PIN, OUTPUT);
  //Start serial communication at port 9600
  Serial.begin(9600);
}

void loop()
{
  //Read the current state of the button
  uint8_t buttonState = digitalRead(BTN_PIN);
  
  //Print the button's current state, previous state and LED state
  Serial.print(buttonState);
  Serial.print(buttonPrevState);
  Serial.print(ledState);
  Serial.println("");
  
  
  //Check if the state of the button changed, turn the LED on accordingly and vice versa
  if(buttonState != buttonPrevState)
  {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
  }
  
  buttonPrevState = buttonState;
    
  //Pause the program for 500 miliseconds
  delay(500);
}