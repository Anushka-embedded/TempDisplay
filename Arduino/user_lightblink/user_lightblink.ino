
const int led1 = 32;
const int led2 = 33;

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);


  Serial.begin(115200);

  Serial.println("=== ESP32 LED CONTROL MENU ===");
  Serial.println("1. Turn ON LED 1");
  Serial.println("2. Turn OFF LED 1");
  Serial.println("3. Turn ON LED 2");
  Serial.println("4. Turn OFF LED 2");
  Serial.println("5. Turn ON Both LEDs");
  Serial.println("6. Turn OFF Both LEDs");
  Serial.println("Enter your choice:");
}

void loop() {
  if (Serial.available() > 0) {
    char choice = Serial.read();

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

      case '\r':
      case '\n':
        break;

      default:
        Serial.println("Invalid choice! Enter 1 to 6.");
    }

    Serial.println("Enter your choice:");
  }
}
