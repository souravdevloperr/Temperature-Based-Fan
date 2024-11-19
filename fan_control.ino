#include <DHT11.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin configurations
DHT11 dht11(2);                     // DHT11 sensor on pin 2
const int motorPin = 3;              // Motor PWM pin
const int motorDirPin = 4;           // Motor direction pin
const int trigPin = 9;               // Sonar sensor trigger
const int echoPin = 8;               // Sonar sensor echo

int ltemp = 20;         //Change it to set the lower range of temperature
int utemp = 30;         //Change it to set the upper range of temperature

int mindist = 20; //in CM (change it to set the minimum distance from which fan  ctivates )
// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C LCD at address 0x27

// Variables
int temperature = 0, humidity = 0, pwmValue = 0;
long duration;
int distance;

void setup() {
  pinMode(motorPin, OUTPUT);
  pinMode(motorDirPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  lcd.init();
  lcd.backlight();
  digitalWrite(motorDirPin, LOW);
}

void loop() {
  dht11.readTemperatureHumidity(temperature, humidity);
  
  // Sonar sensor distance reading
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  
  // Control motor based on temperature and distance
  if (distance <= mindist && temperature >= ltemp) {
    pwmValue = map(temperature, ltemp, utemp, 125, 250);
    pwmValue = constrain(pwmValue, 125, 250);
    int fanSpeedPercent = map(pwmValue, 125, 250, 50, 100);
    
    // Display temperature and fan speed on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temperature:");
    lcd.print(temperature);
    lcd.print("C ");
    lcd.setCursor(0, 1);
    lcd.print("Fan Speed:");
    lcd.print(fanSpeedPercent);
    lcd.print("%");

    analogWrite(motorPin, pwmValue);
  } else {
    // Turn off motor and update LCD
    digitalWrite(motorPin, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temperature:");
    lcd.print(temperature);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Fan: OFF");
  }

  delay(500);
}
