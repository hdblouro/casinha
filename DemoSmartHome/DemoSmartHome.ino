// Menu Demo Casinha Smart Home
#include "DHT.h"        // Opção 3
#include <Servo.h>      // Opção 4, 5 e 6
#include "EM_TTP229.h"  // Opção 5
#include <Wire.h>       // Opção 6 e 7
#include "TM1650.h"     // Opção 6 e 7

// TM1650 tm_4display;

volatile int _light;
//volatile int yudi;
volatile int itemsc;

Servo servo_13;

EM_TTP229 mTTP229;

int SCLPin = 9, SDOPin = 8;

String Read_Key() {
  String key_name = mTTP229.GetKeyMap();
  char *result = (char *)key_name.c_str();
  return result;
}

String item;
volatile int number;
String password[] = { "0", "0", "0" };

Servo servo_12;

Servo servo_11;

int tempo = 0;
int opcao = 0;
String rc = "";

DHT dhtA0(A0, 11);  // Portas DHT Temperatura - Opção 3
DHT dhtA3(A3, 11);

void setMotor8833(int speedpin, int dirpin, int speed) {
  if (speed == 0) {
    digitalWrite(dirpin, LOW);
    analogWrite(speedpin, 0);
  } else if (speed > 0) {
    digitalWrite(dirpin, HIGH);
    analogWrite(speedpin, speed);
  } else {
    digitalWrite(dirpin, LOW);
    analogWrite(speedpin, 255 + speed);
  }
}

void setup() {
  Serial.begin(9600);
  // Config Opção 1: Detect Human Body Turn Light On
  pinMode(4, INPUT);   // Set port 4 as input mode
  pinMode(3, OUTPUT);  // Set port 3 as output mode
  // Config Opção 2: Door Bell
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  // Config Opção 3: Temperatura e Ventilador
  dhtA0.begin();
  dhtA3.begin();
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  // Config Opção 4: Porta Automática Garagem
  pinMode(7, INPUT);
  servo_11.attach(11);
  servo_11.write(0);
  // Config Opção 5: Acesso Inteligente
  item = "";
  number = 0;
  mTTP229.initTTP229(SCLPin, SDOPin);
  servo_12.attach(12);
  // Config Opção 6: Janela Automática
  Wire.begin();
  //  tm_4display.init();
  _light = 0;
  //  tm_4display.clear();
  servo_13.attach(13);
  // Config Opção 7: Janela/Chuva Inteligente
  //  yudi = 0;
  //  itemsc = 0;
  Serial.println("Setup finalizado...");
}

void loop() {
  if (tempo == 0 && opcao == 0) {
    showMenu();
    recvWithEndMarker();
    showNewData();
  } else {
    switch (opcao) {
      case 1:
        tempo++;
        if (tempo == 50) {
          tempo = 0;
          opcao = 0;
          Serial.println("Terminado...");
          break;
        }
        if (digitalRead(4) == 1) {  // Judge whether the human body is sensed
          digitalWrite(3, HIGH);    // Turn on the light
        } else {
          digitalWrite(3, LOW);  // Turn off the light
        }
        delay(500);
        break;
      case 2:
        tempo++;
        if (tempo == 50) {
          tempo = 0;
          opcao = 0;
          Serial.println("Terminado...");
          break;
        }
        if (digitalRead(2) == 1) {
          tone(3, 659);
          delay(600);
          tone(3, 523);
          delay(500);
          tone(3, 587);
          delay(500);
          tone(3, 392);
          delay(1000);
          noTone(3);
          tone(3, 392);
          delay(500);
          tone(3, 587);
          delay(500);
          tone(3, 659);
          delay(500);
          tone(3, 523);
          delay(500);
        } else {
          noTone(3);
        }
        delay(500);
        break;
      case 3:
        tempo++;
        Serial.println(String("Temperatura:") + String(dhtA3.readTemperature()) + "ºC - Tempo: " + tempo);
        if (tempo == 50) {
          tempo = 0;
          opcao = 0;
          setMotor8833(5, 6, 0);
          Serial.println("Terminado...");
          break;
        }

        if (dhtA3.readTemperature() > 23) {
          setMotor8833(5, 6, 100);
          delay(500);
        } else {
          setMotor8833(5, 6, 0);
        }
        break;
      case 4:
        tempo++;
        if (tempo == 50) {
          tempo = 0;
          opcao = 0;
          Serial.println("Terminado...");
          break;
        }

        if (digitalRead(7) == 0) {
          Serial.println("Carro detectado...");
          servo_11.write(90);
          delay(3000);
        } else {
          servo_11.write(0);
        }
        delay(500);
        break;
      case 5:
        tempo++;
        if (tempo == 600) {
          tempo = 0;
          opcao = 0;
          Serial.println("Terminado...");
          break;
        }
        item = Read_Key();
        if (item == "1" && number == 0) {
          Serial.println(item);
          password[(int)(0)] = item;
          number = 1;
          digitalWrite(3, HIGH);  // Turn on the green light
          tone(3, 392);
          delay(100);
          noTone(3);
        }
        if (item == "2" && number == 1) {
          Serial.println(item);
          password[(int)(1)] = item;
          number = 2;
          digitalWrite(5, HIGH);  // Turn on the yellow light
          tone(3, 392);
          delay(100);
          noTone(3);
          digitalWrite(5, LOW);  // Turn off the yellow light
        }
        if (item == "3" && number == 2) {
          Serial.println(item);
          password[(int)(2)] = item;
          number = 3;
          digitalWrite(6, HIGH);  // Turn on the red light
          tone(3, 392);
          delay(100);
          noTone(3);
          digitalWrite(6, LOW);  // Turn off the red light
        }
        if (password[(int)(0)] == "1" && (password[(int)(1)] == "2" && password[(int)(2)] == "3")) {
          Serial.println("Acesso liberado...");
          servo_12.write(0);
          delay(2000);
          digitalWrite(3, LOW);  // Turn off the green light
          digitalWrite(5, LOW);  // Turn off the yellow light
          digitalWrite(6, LOW);  // Turn off the red light
          password[(int)(0)] = "0";
          password[(int)(1)] = "0";
          password[(int)(2)] = "0";
          number = 0;
        } else {
          servo_12.write(90);
          delay(20);
        }
        break;
      case 6:
        tempo++;
        if (tempo == 50) {
          tempo = 0;
          opcao = 0;
          Serial.println("Terminado...");
          break;
        }
        /* Assign the analog value of the photosensitive sensor connected to the A2 interface to the light variable */
        _light = analogRead(A2);
        //        tm_4display.displayString(_light);
        Serial.println(_light);
        delay(500);
        /* Judge whether the light simulation value is greater than 100, if so, open the window; otherwise, close the window */
        if (_light > 100) {
          servo_13.write(0);
          delay(500);
        } else {
          servo_13.write(90);
          delay(20);
        }
        delay(500);
        break;
    }
  }
}

void showMenu() {
  Serial.println("== Menu Casinha ==");
  Serial.println("1. Lâmpada Detecta Humano");
  Serial.println("2. Campainha Porta");
  Serial.println("3. Temperatura e Ventilador");
  Serial.println("4. Porta Automática Garagem");
  Serial.println("5. Acesso Inteligente");
  Serial.println("6. Janela Automática");
  Serial.println("7. Controle Chuva/Janela");
}

void recvWithEndMarker() {
  rc = "";
  opcao = 0;
  while (rc == "" && opcao >= 0 && opcao <= 7) {
    rc = Serial.readString();
    opcao = rc.toInt();
  }
}

void showNewData() {
  Serial.print("Opção escolhida: ");
  Serial.print(opcao);
  Serial.println(" Executando...");
}