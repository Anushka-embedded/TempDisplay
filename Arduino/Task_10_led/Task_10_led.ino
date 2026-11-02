
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Onboard user LEDs
const int LED1 = 33;
const int LED2 = 32;

// Onboard buttons
const int SW1 = 39;  // UP
const int SW2 = 36;  // DOWN
const int SW3 = 35;  // SELECT
const int SW4 = 34;  // BACK

const int MENU_SIZE = 4;
int selectedMenu = 0;

const char* menuItems[MENU_SIZE] = {
  "LED1: ON",
  "LED1: OFF",
  "LED2: ON",
  "LED2: OFF"
};

bool buttonPressed(int pin) {
  if (digitalRead(pin) == LOW) {
    delay(180);

    if (digitalRead(pin) == LOW) {
      while (digitalRead(pin) == LOW) {
        delay(10);
      }
      delay(30);
      return true;
    }
  }

  return false;
}

void showMenu() {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Select Option:");

  lcd.setCursor(0, 1);
  lcd.print(">");
  lcd.print(menuItems[selectedMenu]);
}

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  // GPIO 34-39 are input-only and lack internal pull-ups.
  // Use the PCB's existing button pull-up circuitry.
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(SW3, INPUT);
  pinMode(SW4, INPUT);

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);

  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("ESP32 T10");
  lcd.setCursor(0, 1);
  lcd.print("LED Menu");
  delay(1200);

  showMenu();
}

void loop() {
  // SW1: Move up
  if (buttonPressed(SW1)) {
    selectedMenu--;

    if (selectedMenu < 0) {
      selectedMenu = MENU_SIZE - 1;
    }

    showMenu();
  }

  // SW2: Move down
  if (buttonPressed(SW2)) {
    selectedMenu++;

    if (selectedMenu >= MENU_SIZE) {
      selectedMenu = 0;
    }

    showMenu();
  }

  // SW3: Select and perform action
  if (buttonPressed(SW3)) {
    switch (selectedMenu) {
      case 0:
        digitalWrite(LED1, HIGH);
        break;

      case 1:
        digitalWrite(LED1, LOW);
        break;

      case 2:
        digitalWrite(LED2, HIGH);
        break;

      case 3:
        digitalWrite(LED2, LOW);
        break;
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(menuItems[selectedMenu]);
    lcd.setCursor(0, 1);
    lcd.print("Action Done!");
    delay(800);

    showMenu();
  }

  // SW4: Return to first option
  if (buttonPressed(SW4)) {
    selectedMenu = 0;
    showMenu();
  }
}
