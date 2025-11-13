#include <Arduino.h>
#include <SoftwareSerial.h>
#include <VarSpeedServo.h>

// Pins
const int servoPin = 5;          
const int relayPin = 6;     

SoftwareSerial espSerial(7, 8); // RX, TX (UNO receives on 7, sends on 8)
VarSpeedServo door;

void setup() {
  Serial.begin(9600);        // Debugging to PC
  espSerial.begin(9600);     // Communication with ESP32

  door.attach(servoPin);

  pinMode(relayPin, OUTPUT);

 
  Serial.println("Chip Ready");
}

void loop() {
  if (espSerial.available()) {
    char ser = espSerial.read();

    switch (ser) {

      case 'A':
      digitalWrite(relayPin, HIGH);
      break; 
      case 'B': 
      digitalWrite(relayPin, LOW); 
      break;

      case 'C': 
      door.write(0,20,0); 
      break;
      case 'D': 
      door.write(90,20,0); 
      break; 

      default: 
      break;
    }
  }
}
