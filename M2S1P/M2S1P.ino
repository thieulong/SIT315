const byte LED_PIN = 13;
const byte METER_PIN = A4;
int meterReading = 0;
int meterState = 0;
const double timerFrequency = 0.5;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(METER_PIN, INPUT);
  Serial.begin(9600);
  setTimerFromPotentiometer();
}

void loop()
{ 
  delay(500);
  setTimerFromPotentiometer();
}

void setTimerFromPotentiometer() 
{
  meterReading = analogRead(METER_PIN);
  if (meterState!=meterReading){
    meterState = meterReading;
    meterReading = map(meterReading, 0, 1023, 1, 10);
    double times_per_second = (double) meterReading;
    startTimer(times_per_second);
    Serial.println("Setting timer to " + String(times_per_second) + "times per second.");
  }
}

void startTimer(double timerFrequency){
  noInterrupts();
  uint16_t timer_compare = 16000000/(1024*timerFrequency)-1;
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  
  TCCR1B |= (1<<WGM12);
  TCCR1B |= (1<<CS12); 
  TCCR1B |= (1<<CS10);
  
  OCR1A = timer_compare;
  
  TIMSK1 = (1<<OCIE1A); 
  
  sei();
  interrupts();
}


ISR(TIMER1_COMPA_vect){
   digitalWrite(LED_PIN, digitalRead(LED_PIN) ^ 1);
}

