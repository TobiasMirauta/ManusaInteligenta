#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// HC-SR04 pins
#define trig 2
#define echo 3

// DHT11 pins
#define DHT_PIN  4
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);

// I2C 16x2 LCD
#define LCD_ADDR 0x27 
LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);

// LED
const uint8_t LED_PIN = 5;  



void setup() {
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  dht.begin();

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Starting...");
  delay(800);
  lcd.clear();
}

float readDistanceCm() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  unsigned long duration = pulseIn(echo, HIGH, 30000UL);
  if (duration == 0) return -1.0;
  return duration / 60.0;
}

void loop() {
  float distance = readDistanceCm();
  float tempC = dht.readTemperature();

  // Serial monitor
  Serial.print("Distance: ");
  if (distance < 0) Serial.print("Out");
  else Serial.print(String(distance,2) + " cm");

  Serial.print(" | Temp: ");
  if (isnan(tempC)) Serial.print("ERR");
  else Serial.print(String(tempC,2) + " C");
  Serial.println();

  // LCD display
  lcd.setCursor(0,0);
  if (distance < 0) lcd.print("Out   ");  
  else {
    int d = (int)distance;
    if (d > 999) d = 999;
    char buf[7];
    snprintf(buf, sizeof(buf), "%3d cm", d); 
    lcd.print(buf);
  }

  lcd.setCursor(0,1);
  if (isnan(tempC)) lcd.print("ERR ");
  else {
    int t = (int)tempC;
    if (t > 99) t = 99;
    char buf[7];
    snprintf(buf, sizeof(buf), "%2d C", t);
    lcd.print(buf);
  }

  // LED alarm logic
  if (!isnan(tempC) && tempC >= 30.0) {
    digitalWrite(LED_PIN, HIGH); // LED ON
  } else {
    digitalWrite(LED_PIN, LOW);  // LED OFF
  }

  delay(1000);
}
