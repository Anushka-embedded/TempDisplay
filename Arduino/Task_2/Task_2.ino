
#include <Wire.h>                  // enable I2C communication
#include <LiquidCrystal_I2C.h>     // helps to contorl lcd

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Wire.begin();        //starts I2C
  lcd.init();         //initializes the display
  lcd.backlight();    //turn on backlight
  lcd.setCursor(0, 0);  // selects the first row and first column
  lcd.print("Digital Clock"); // display message
}

void loop() {
  unsigned long t = millis() / 1000;  // dividing by 1000 converts them into seconds
  int hours = t / 3600;               //3600= 1hour
  int minutes = (t % 3600) / 60;      // 60 = 1 minutes
  int seconds = t % 60;               // %60 extarcts the remaining seconds or minutes

  char timeStr[9];

  sprintf(timeStr, "%02d:%02d:%02d", hours, minutes, seconds);     // this converts numbers into text in HH:MM:SS and %02 adds leading zero when needed, so 4 becomes 04

  lcd.setCursor(4, 1);        // positions the time on the second row
  lcd.print(timeStr);         // displays the formatted time

  delay(1000);
}c:\Users\jaina\OneDrive\Documents\Arduino\Task_3\Task_3.ino
