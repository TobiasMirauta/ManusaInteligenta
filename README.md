# 🧤 Mănușă smart pentru orientare și monitorizare ambientală

Proiectul **„Mănușă inteligentă”** este realizat cu ajutorul unei plăci **Arduino Nano** și a mai multor senzori, având rolul de a măsura **distanța față de obiecte** și **temperatura mediului ambiant**, afișând informațiile pe un **display LCD 16x2**.

Mănușa este concepută pentru utilizare în **medii de construcții**, **domeniul electric** sau **verificări de gaze**, unde operatorii trebuie să ajungă în zone greu accesibile sau potențial periculoase, permițând realizarea măsurătorilor **în siguranță**.

---

## 🔧 Componente utilizate

- Arduino Nano  
- LCD 16x2 cu interfață I2C  
- Cablu de alimentare  
- Senzor de temperatură DHT11  
- Senzor de distanță ultrasonic HC-SR04  
- LED de avertizare  

---

## 🧠 Arduino Nano

Arduino Nano este unitatea principală de control a proiectului. Aceasta coordonează funcționarea tuturor componentelor conectate.

### Rol în proiect

- Citește datele de la senzori (distanță și temperatură)
- Procesează informațiile primite
- Afișează valorile pe LCD
- Controlează LED-ul de avertizare

### Interacțiunea cu codul

- Pinii digitali **D2** și **D3** sunt folosiți pentru senzorul ultrasonic HC-SR04  
- Pinul **D4** este utilizat pentru senzorul de temperatură DHT11  
- Pinul **D5** controlează LED-ul  
- Comunicarea cu LCD-ul se face prin protocolul **I2C**  
- Codul rulează continuu în funcția `loop()` și actualizează datele la fiecare secundă  

---

## 📟 LCD 16x2 cu interfață I2C

Display-ul LCD este utilizat pentru afișarea **distanței față de obiecte** și a **temperaturii măsurate**.

### Interacțiunea cu codul

- Controlat cu ajutorul bibliotecii `LiquidCrystal_I2C`
- Comunicarea se face prin protocolul **I2C** (pinii **SDA** și **SCL**)
- Funcțiile `lcd.setCursor()` și `lcd.print()` sunt utilizate pentru afișarea textului
- Adresa I2C **0x27** este definită în cod

---

## 🌡️ Senzor de temperatură DHT11

Senzorul **DHT11** este utilizat pentru măsurarea temperaturii mediului ambiant și transmite date digitale către Arduino.

### Interacțiunea cu codul

- Controlat prin biblioteca `DHT`
- Pinul de date este conectat la **D4**
- Funcția `dht.readTemperature()` citește valoarea temperaturii
- Dacă temperatura depășește **30°C**, LED-ul de avertizare este activat

---

## 🚨 LED de avertizare

LED-ul este utilizat ca element de **semnalizare vizuală**.

### Interacțiunea cu codul

- Conectat la pinul **D5**
- Se aprinde dacă temperatura este **mai mare sau egală cu 30°C**
- Se stinge automat când temperatura scade sub acest prag

---

## 💻 Cod Arduino

```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define TRIG_PIN 2
#define ECHO_PIN 3
#define DHT_PIN 4
#define LED_PIN 5

#define DHTTYPE DHT11
DHT dht(DHT_PIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  dht.begin();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Manusa Smart");
  delay(2000);
  lcd.clear();
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;

  float temperature = dht.readTemperature();

  lcd.setCursor(0, 0);
  lcd.print("Dist: ");
  lcd.print(distance);
  lcd.print(" cm   ");

  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C   ");

  if (temperature >= 30) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  delay(1000);
}
