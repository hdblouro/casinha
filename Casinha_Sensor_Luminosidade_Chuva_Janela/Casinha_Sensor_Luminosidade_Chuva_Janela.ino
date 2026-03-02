#include <Wire.h>
#include <TM1650.h>
#include <Servo.h>

TM1650 tm_4display;
volatile int _light;
volatile int yudi;
volatile int item;
Servo servo_13;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  tm_4display.init();
  _light = 0;
  yudi = 0;
  item = 0;
  tm_4display.clear();
  servo_13.attach(13);
}

void loop() {
  // Assign the analog value of the photosensitive sensor connected to the A2 interface to the light variable 
  _light = analogRead(A2);
  // Assign the return value of raindrop sensor connected to A1 interface to Yudi variable
  yudi = analogRead(A1);
  tm_4display.displayString(_light);
  Serial.print(String(" light:") + String(_light));
  Serial.print(",");
  Serial.println(String("yudi:") + String(yudi));
  delay(500);
  if (_light > 100) { /*Judge whether the light simulation value is greater than 100, if the window is opened, otherwise, close the window */
    if (yudi < 500) { /*When the raindrop sensor senses rain, the value will be smaller, otherwise the value will be larger. Here, judge whether the return value of raindrop simulation is less than 200. If so, close the window; otherwise, open the window.*/
      servo_13.write(90);
      delay(300);
    } else {
      servo_13.write(0);
      delay(3000);
    }
  } else {
    servo_13.write(90);
    delay(20);
  }
}