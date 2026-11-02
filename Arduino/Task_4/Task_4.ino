
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  lcd.init();
  lcd.backlight();
  dht.begin();

  lcd.setCursor(0, 0);
  lcd.print("Temperature:");
}

void loop() {
  float temp = dht.readTemperature();

  if (isnan(temp)) {
    lcd.setCursor(0, 1);
    lcd.print("Sensor Error    ");
  }
  else {
    lcd.setCursor(0, 0);
    lcd.print("Temperature:    ");

    lcd.setCursor(0, 1);
    lcd.print(temp, 1);
    lcd.write(byte(223));
    lcd.print("C ");

    if (temp > 50) {
      lcd.print("ON ");
    }
    else {
      lcd.print("OFF");
    }
  }

  delay(2000);
}

