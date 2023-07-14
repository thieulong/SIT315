#include <PinChangeInterrupt.h>
#include <Adafruit_Sensor.h>
#include <dht.h>
#define DHTTYPE DHT11 
#include <TimerOne.h>

const uint8_t sensorPins[] = {2, 7, 4};   // Group of sensor'pins accordingly (2-PIR Motion, 4-Temperature, 7-Soil Moisture)                

dht DHT;
const uint8_t ledPin = 13;                             
const uint8_t buttonPin = A1;                        

volatile bool pirState = false;                      
volatile float temperature = 0.0;  
volatile float humidity = 0.0;                   
volatile int soilMoisture = 0;                         

void setup() {

  pinMode(ledPin, OUTPUT);                            
  pinMode(buttonPin, INPUT);  
  
  pinMode(sensorPins[0], INPUT); 
  pinMode(sensorPins[1], INPUT); 
  pinMode(sensorPins[2], INPUT); 

  Timer1.initialize(5000000);                          
  Timer1.attachInterrupt(timerInterrupt);              

  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonInterrupt, CHANGE); // Attach interrupt to button when pressed
  attachPCINT(digitalPinToPCINT(sensorPins[0]), pirInterrupt, CHANGE);        // Attach interrupt to PIR sensor when detects value changed
  attachPCINT(digitalPinToPCINT(sensorPins[1]), tempInterrupt, CHANGE);       // Attach interrupt to teperature sensor when temperature changed
  attachPCINT(digitalPinToPCINT(sensorPins[2]), soilMoistureInterrupt, CHANGE); // Attach interrupt to moisture sensor when moisture level changed

  Serial.begin(9600);                                
}

void loop() {
//  sensorValues(); 
  delay(1000);
}

void pirInterrupt() {
  pirState = digitalRead(sensorPins[0]);
  if (pirState) {
    digitalWrite(ledPin, HIGH); // Turn LED on if motion detected
  } else {
    digitalWrite(ledPin, LOW);  // Turn LED off if no motion detected
  }
  Serial.print("[INTERRUPT] PIR Status:");
  Serial.println(pirState);
}

void tempInterrupt() { 
  int readData = DHT.read11(sensorPins[1]); // Read temperature value
  temperature = DHT.temperature;
//  temperature = dht.readTemperature(); // Read temperature value
  Serial.print("[INTERRUPT] Temperature:");
  Serial.print(temperature);
  Serial.println("°C");
}

void soilMoistureInterrupt() {
  soilMoisture = analogRead(sensorPins[2]); // Read soil moisture value
  Serial.print("[INTERRUPT] Soil moisture:");
  Serial.println(soilMoisture);
}

void timerInterrupt() {
  digitalWrite(ledPin, HIGH); // Flash the LED on when timer interrupt kicks in 
  delay(100);                
  digitalWrite(ledPin, LOW);  
  Serial.println("[TIMER] Timer interrupt triggered"); 
}

void buttonInterrupt() {
  digitalWrite(ledPin, HIGH);  // Flash the LED for longer duration when button interrupted
  delay(500);                 
  digitalWrite(ledPin, LOW);  
  Serial.println("[BUTTON] Button interrupt triggered"); 
}

void sensorValues() {
//  pirInterrupt();
  Serial.print("[INFO] PIR State: ");
  Serial.println(pirState);

//  tempInterrupt();
  Serial.print("[INFO] Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

//  soilMoistureInterrupt();
  Serial.print("[INFO] Soil Moisture: ");
  Serial.print(soilMoisture);
  Serial.println();
}
