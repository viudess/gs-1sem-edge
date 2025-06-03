#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD I2C (endereço 0x27, 16 colunas, 2 linhas)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pinos do sensor HC-SR04
#define trigPin 9
#define echoPin 10

// LEDs
#define greenLED 2
#define yellowLED 3
#define redLED 4

// Buzzer
#define buzzerPin 5

// Variável para armazenar distância anterior
int lastDistance = 0;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Sistema Iniciado");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Mede distância
  long duration;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;

  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Define estado com base na distância
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dist: ");
  lcd.print(distance);
  lcd.print(" cm");

  if (distance > 150) {
    digitalWrite(greenLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, LOW);
    digitalWrite(buzzerPin, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Nivel seguro");
  } else if (distance > 75) {
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(redLED, LOW);
    digitalWrite(buzzerPin, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Risco moderado");
  } else {
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, HIGH);
    tone(buzzerPin, 2000, 300); // Toca som agudo de 2kHz por 300ms
	delay(400);                 // Pequena pausa
	tone(buzzerPin, 1500, 300); // Outro tom de 1.5Hz por 300ms
	delay(400);
    lcd.setCursor(0, 1);
    lcd.print("ALERTA ENCHENTE!");
  }

  // Previsão simples: a distância está aumentando? (melhora)
  if (lastDistance > 0) {
    if (distance > lastDistance) {
      lcd.setCursor(0, 1);
      lcd.print("Melhora notada");
    } else if (distance < lastDistance) {
      lcd.setCursor(0, 1);
      lcd.print("Piorando...       ");
    }
  }

  lastDistance = distance;
  delay(2000);
}