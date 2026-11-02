
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// LED pins
const int led1 = 32;
const int led2 = 33;

// DHT22 sensor pin
const int DHT_PIN = 4;
#define DHTTYPE DHT22

DHT dht(DHT_PIN, DHTTYPE);

// I2C LCD: address 0x27, 16 columns, 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Display menu


void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);

  Serial.begin(115200);

  // Initialize I2C for ESP32
  Wire.begin(21, 22);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // Initialize DHT22
  dht.begin();

  lcd.setCursor(0, 0);
  lcd.print("ESP32 System");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  delay(1000);
  lcd.clear();

  showMenu();
}

void loop() {
  if (Serial.available() > 0) {
    char choice = Serial.read();

    // Ignore Enter characters
    if (choice == '\r' || choice == '\n') {
      return;
    }

    switch (choice) {

      case '1':
        digitalWrite(led1, HIGH);
        Serial.println("LED 1 is ON");
        break;

      case '2':
        digitalWrite(led1, LOW);
        Serial.println("LED 1 is OFF");
        break;

      case '3':
        digitalWrite(led2, HIGH);
        Serial.println("LED 2 is ON");
        break;

      case '4':
        digitalWrite(led2, LOW);
        Serial.println("LED 2 is OFF");
        break;

      case '5':
        digitalWrite(led1, HIGH);
        digitalWrite(led2, HIGH);
        Serial.println("Both LEDs are ON");
        break;

      case '6':
        digitalWrite(led1, LOW);
        digitalWrite(led2, LOW);
        Serial.println("Both LEDs are OFF");
        break;

      case '7': {
        Serial.println("Reading temperature...");

        // DHT22 needs time between readings
        delay(2000);

        float temperature = dht.readTemperature();

        lcd.clear();

        if (isnan(temperature)) {
          Serial.println("DHT22 reading failed!");

          lcd.setCursor(0, 0);
          lcd.print("Sensor Error");
          lcd.setCursor(0, 1);
          lcd.print("Check DHT22");

        } else {
          Serial.print("Temperature: ");
          Serial.print(temperature, 1);
          Serial.println(" C");

          lcd.setCursor(0, 0);
          lcd.print("Temperature:");

          lcd.setCursor(0, 1);
          lcd.print(temperature, 1);
          lcd.write((uint8_t)223);
          lcd.print("C");
        }

        break;
      }

      default:
        Serial.println("Invalid choice! Enter 1 to 7.");
        break;
    }

    showMenu();
  }
}

