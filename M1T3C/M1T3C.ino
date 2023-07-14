#include <dht.h>  
     
int temp_sensor = 4;    
int led1 = 12;     
int led2 = 13;           
int motion_sensor = 2;              
int led_state = LOW;            
int motion = 0;  
   
dht DHT;              

void setup() {
  Serial.begin(9600);
  pinMode(led1, OUTPUT); 
  pinMode(led2, OUTPUT); 
  pinMode(motion_sensor, INPUT);
}

void loop() {
  motion = digitalRead(motion_sensor); 
  int readData = DHT.read11(temp_sensor);
  float temp = DHT.temperature;        
  if (temp > 25){
    digitalWrite(led1, HIGH);
    Serial.print("Temperature = ");
    Serial.print(temp);
    Serial.print("°C");
    Serial.println(" > 25°C -> Red LED is ON");
  } else if (temp < 25){
    digitalWrite(led1, LOW);
    Serial.print("Temperature = ");
    Serial.print(temp);
    Serial.print("°C");
    Serial.println(" < 25°C -> Red LED is OFF");
  }
  if (motion == HIGH) {         
    if (led_state == LOW) {
      Serial.println("Movement detected -> Blue LED is ON");
      led_state = HIGH;     
      digitalWrite(led2, HIGH); 
    }
  } else {
    if (led_state == HIGH) {
      Serial.println("No movement detected -> Blue LED is OFF");
      led_state = LOW;     
      digitalWrite(led2, LOW);  
    }
  }
  delay(1000); 
}
