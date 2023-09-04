#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HX711.h>

// HX711 circuit wiring
const int LOADCELL_DT_PIN = 5;
const int LOADCELL_SCK_PIN = 6;

HX711 scale;
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  Serial.begin(115200);
  scale.begin(LOADCELL_DT_PIN, LOADCELL_SCK_PIN);
  lcd.init(); // initialize the lcd
  lcd.backlight();
}

void loop()
{

  /* The code is checking if the HX711 scale is ready to read a value. If it is ready, it reads the
  value from the scale using `scale.read()` and stores it in the variable `reading`. Then, it prints
  the reading to the serial monitor using `Serial.print()` and `Serial.println()`. */
  if (scale.is_ready())
  {
    /* The code is reading the average value from the HX711 scale using the `scale.read_average()`
    function. The argument `8` specifies the number of readings to average. */
    long reading = scale.read_average(8);
    Serial.print("HX711 reading: ");
    Serial.println(reading);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(reading);
  }
  else
  {
    Serial.println("HX711 not found.");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("HX711 not found.");
  }

  delay(1000);
}
