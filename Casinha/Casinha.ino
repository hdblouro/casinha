#include "Buzzer.h" 
#include "Sounds.h" 
#include "PH20Port.h" 
#include <DHT.h>
#include <Servo.h>

Servo servo_11;
PH20Port buzzerplay(P9); 
int touch_PIN2 = 2; 
Buzzer mBuzzer = Buzzer(buzzerplay.pin1()); 
Buzzer buzzer(buzzerplay.pin1());
DHT dhtA0(A0, 11);
DHT dhtA3(A3, 11);

void setup() {
  Serial.begin(9600);
  pinMode(4, INPUT); // Set port 4 as input mode 
  pinMode(3, OUTPUT); // Set port 3 as output mode
  pinMode(touch_PIN2,INPUT); 
  dhtA0.begin(); 
  dhtA3.begin(); 
  pinMode(5, OUTPUT); 
  pinMode(6, OUTPUT); 
  digitalWrite(5, LOW); 
  digitalWrite(6, LOW);
  pinMode(7, INPUT);
  servo_11.attach(11);
} 

void setMotor8833(int speedpin,int dirpin, int speed) {
  if (speed == 0) { 
    digitalWrite(dirpin, LOW); 
    analogWrite(speedpin, 0);
  } else if (speed > 0) {
    digitalWrite(dirpin, LOW); 
    analogWrite(speedpin, speed); } 
  else { digitalWrite(dirpin, HIGH);
    analogWrite(speedpin, 255 + speed); 
  } 
}

void loop() { 
  if (digitalRead(4) == 1){ // Judge whether the human body is sensed 
    digitalWrite(3, HIGH); // Turn on the light
  } else {
    digitalWrite(3, LOW); // Turn off the light
  }

  if (digitalRead(touch_PIN2)==HIGH) { 
    mBuzzer.bendTones(1500, 2500, 1.05, 20, 8); 
    mBuzzer.bendTones(2499, 1500, 1.05, 25, 8); 
  } else { 
    buzzer.noTone();
  }

  // Using the serial monitor to view the temperature analog value of the temperature sensor 
//  Serial.println(String("T:") + String(dhtA3.readTemperature())); 
//  delay(500); /*Judge whether the temperature simulation value is greater than 30, if so, turn on the fan; 
//  otherwise, turn off the fan*/ 
  if (dhtA3.readTemperature() > 27.5) { 
    setMotor8833(5, 6, 100);
  } else { 
    setMotor8833(5, 6, 0);
  }

  if (digitalRead(7) == 0) { 
    servo_11.write(90); 
    delay(5000);
  } else { servo_11.write(0); 
    delay(1000);
  }
}