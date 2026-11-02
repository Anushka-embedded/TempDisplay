
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// LCD settings
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Temperature sensor settings
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// LED pins
#define LED1 32
#define LED2 33

// Button pins
#define BTN1 25
#define BTN2 26
#define BTN3 27

// Temperature threshold
float temperatureLimit = 50.0;

// Button states
bool led1State = false;
bool led2State = false;

void setup() {
  Serial.begin(115200);

  // Configure LEDs
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  // Configure buttons
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);

  // Initialize sensor
  dht.begin();

  // Initialize LCD
  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("T9 Temp Display");
  lcd.setCursor(0, 1);
  lcd.print("ESP32 Starting");

  delay(2000);
  lcd.clear();
}

void loop() {
  // Read temperature
  float temp = dht.readTemperature();

  // Display temperature
  lcd.setCursor(0, 0);

  if (isnan(temp)) {
    lcd.print("Sensor Error    ");
    lcd.setCursor(0, 1);
    lcd.print("Check DHT22     ");
  } else {
    lcd.print("Temperature:");
    lcd.setCursor(0, 1);
    lcd.print(temp, 1);
    lcd.write(byte(223));
    lcd.print("C ");

    if (temp > temperatureLimit) {
      lcd.print("HIGH");
    } else {
      lcd.print("NORMAL");
    }

    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" C");
  }

  // Button 1: Toggle LED 1
  if (digitalRead(BTN1) == LOW) {
    led1State = !led1State;
    digitalWrite(LED1, led1State);
    delay(250);
  }

  // Button 2: Toggle LED 2
  if (digitalRead(BTN2) == LOW) {
    led2State = !led2State;
    digitalWrite(LED2, led2State);
    delay(250);
  }

  // Button 3: Display temperature on Serial Monitor
  if (digitalRead(BTN3) == LOW) {
    Serial.println("Temperature display refreshed");
    delay(250);
  }

  delay(1000);
}

