#include <PinChangeInterrupt.h>
#include <Adafruit_Sensor.h>
#include <dht.h>
#define DHTTYPE DHT11 
#include <TimerOne.h>

const uint8_t sensors[] = {2, 7, 4};   // Group of sensor'pins accordingly (2-PIR Motion, 4-Temperature, 7-Soil Moisture)                

dht DHT;
const uint8_t LED = 13;                             
const uint8_t button = A1;                        

volatile bool movement = false;                      
volatile float temperature = 0.0;  
volatile float humidity = 0.0;                   
volatile int soilMoisture = 0;                         

void setup() {

  pinMode(LED, OUTPUT);                            
  pinMode(button, INPUT);  
  
  pinMode(sensors[0], INPUT); 
  pinMode(sensors[1], INPUT); 
  pinMode(sensors[2], INPUT); 

  Timer1.initialize(5000000);   // Make a timer interuption for every 5 seconds                       
  Timer1.attachInterrupt(timerInterrupt);              

  attachInterrupt(digitalPinToInterrupt(button), buttonInterrupt, CHANGE); // Attach interrupt to button when pressed
  attachPCINT(digitalPinToPCINT(sensors[0]), pirInterrupt, CHANGE);        // Attach interrupt to PIR sensor when detects value changed
  attachPCINT(digitalPinToPCINT(sensors[1]), tempInterrupt, CHANGE);       // Attach interrupt to teperature sensor when temperature changed
  attachPCINT(digitalPinToPCINT(sensors[2]), soilMoistureInterrupt, CHANGE); // Attach interrupt to moisture sensor when moisture level changed

  Serial.begin(9600);                                
}

void loop() {
//  sensorValues(); 
  delay(1000);
}

void pirInterrupt() {
  movement = digitalRead(sensors[0]);
  if (movement) {
    digitalWrite(LED, HIGH); // Turn LED on if motion detected
  } else {
    digitalWrite(LED, LOW);  // Turn LED off if no motion detected
  }
  Serial.print("[INTERRUPT] PIR Status:");
  Serial.println(movement);
}

void tempInterrupt() { 
  int readData = DHT.read11(sensors[1]); // Read temperature value
  temperature = DHT.temperature;
//  temperature = dht.readTemperature(); // Read temperature value
  Serial.print("[INTERRUPT] Temperature:");
  Serial.print(temperature);
  Serial.println("°C");
}

void soilMoistureInterrupt() {
  soilMoisture = analogRead(sensors[2]); // Read soil moisture value
  Serial.print("[INTERRUPT] Soil moisture:");
  Serial.println(soilMoisture);
}

void timerInterrupt() {
  digitalWrite(LED, HIGH); // Flash the LED on when timer interrupt kicks in 
  delay(100);                
  digitalWrite(LED, LOW);  
  Serial.println("[TIMER] Timer interrupt triggered"); 
}

void buttonInterrupt() {
  digitalWrite(LED, HIGH);  // Flash the LED for longer duration when button interrupted
  delay(500);                 
  digitalWrite(LED, LOW);  
  Serial.println("[BUTTON] Button interrupt triggered"); 
}

void sensorValues() {
//  pirInterrupt();
  Serial.print("[INFO] PIR State: ");
  Serial.println(movement);

//  tempInterrupt();
  Serial.print("[INFO] Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

//  soilMoistureInterrupt();
  Serial.print("[INFO] Soil Moisture: ");
  Serial.print(soilMoisture);
  Serial.println();
}
