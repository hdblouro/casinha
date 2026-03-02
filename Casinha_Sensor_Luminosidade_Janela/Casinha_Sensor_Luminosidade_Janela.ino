#include <Wire.h>
#include "TM1650.h"
#include <Servo.h>

TM1650 tm_4display;
volatile int _light;
Servo servo_13;
void setup(){
  Serial.begin(9600);
  Wire.begin();
  tm_4display.init();
  _light = 0; 
  tm_4display.clear(); 
  servo_13.attach(13);
}

void loop(){
  /* Assign the analog value of the photosensitive sensor connected to the A2 interface to the light variable */ 
  _light = analogRead(A2); 
  tm_4display.displayString(_light);
  Serial.println(_light); 
  delay(500); 
  /* Judge whether the light simulation value is greater than 100, if so, open the window; otherwise, close the window */ 
  if (_light > 100) { 
    servo_13.write(0); 
    delay(5000); } 
  else { 
    servo_13.write(90); 
    delay(20); 
  } 
}