#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHTesp.h"

LiquidCrystal_I2C mylcd(0x27, 20, 4);

int pinDHT = 26;
DHTesp dht;

void setup() {
  mylcd.begin(16, 2);
  mylcd.init();
  mylcd.backlight();
  dht.setup(pinDHT, DHTesp::DHT11);
}

void loop() {
  mylcd.clear();
  mylcd.setCursor(0, 0);
  TempAndHumidity data = dht.getTempAndHumidity();
  mylcd.print(String("T:") + String(data.temperature,2));
  mylcd.setCursor(0, 1);
  mylcd.print(String("H:") + String(data.humidity,2));
  delay(5000);
}