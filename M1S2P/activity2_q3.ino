
//Initialize pin number of button and LED
const uint8_t BTN_PIN = 2;
const uint8_t LED_PIN = 13;

//Initialize variables to store the state of button and LED
uint8_t buttonPrevState = LOW;
uint8_t ledState = LOW;

//Initialize button delay and time from last pressed variables
const long button_delay = 10;
long last_pressed = 0;

//Set up function, set pin modes and begin serial output
void setup()
{
  //Set the pin mode of button to be input with pull-up resistor
  pinMode(BTN_PIN, INPUT_PULLUP);
  //Set the pin mode of LED to be ourput
  pinMode(LED_PIN, OUTPUT);
  //Open serial output at port 9600
  Serial.begin(9600);
}

void loop()
{
  //Read the button's state currently
  uint8_t buttonState = digitalRead(BTN_PIN);
  
  if (buttonState!=buttonPrevState){
    if (button_delay < millis()-last_pressed){
      last_pressed = millis();
      buttonPrevState = buttonState;
      if (buttonState == LOW){
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
      }
    }
  }
  
  //Print the button's current state, previous state and LED'state
  Serial.print(buttonState);
  Serial.print(buttonPrevState);
  Serial.print(ledState);
  Serial.println("");
}