 #include <Wire.h>
  #include <LiquidCrystal_I2C.h>
  LiquidCrystal_I2C lcd(0x27,16,2); // set lcd address to 0x27 for a 16 chars and 2 line display
void setup()
{
  lcd.init();  // initialize lcd
  lcd.backlight(); // to on backlight of the display if use I2C
}

void loop() {
  lcd.setCursor(0,0);     // Text start form 0 row , 0 coloum
  lcd.print("Hello World !");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Hello Adyanta");
   delay(2000);
}
