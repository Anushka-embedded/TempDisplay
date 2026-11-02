
#include <Wire.h>       //enables I2C communication between the Arduino and LCD.
#include <LiquidCrystal_I2C.h>   //controls the LCD through the I2C module.
#include <DHT.h>      //allows to communicate with the DHT22 sensor.

LiquidCrystal_I2C lcd(0x27, 16, 2);    //Initializing the LCD

#define DHTPIN 2           //means the sensor's DATA pin is connected to Arduino digital pin 2.
#define DHTTYPE DHT22      //tells the program which sensor model you are using.

DHT dht(DHTPIN, DHTTYPE);      //This creates an object named dht that the program uses to read the sensor.

void setup() {
  lcd.init();        //initializes the LCD.

  lcd.backlight();   //switches on  backlight.
  
  dht.begin();       //initializes communication with the DHT22.

  lcd.setCursor(0, 0);  //places the cursor at the first column of the first row.
  lcd.print("Temperature:");    //displays the heading
}

void loop() {
  float temp = dht.readTemperature();   //reads the temperature from the DHT22 sensor in degrees Celsius, float is data type, temp is variable in which data stoerd

  lcd.setCursor(0, 1);

  if (isnan(temp)) {         //isnan() means “is not a number.”

    lcd.print("Sensor Error    ");
  } else {
    lcd.print(temp, 1);      //displays the temperature with one digit after the decimal point, such as 28.5
    lcd.write(byte(223));    //displays the degree symbol ° on a compatible character LCD.
    lcd.print("C              ");   //displays Celsius and extra spaces to clear any old characters.
  }

  delay(2000);
}
