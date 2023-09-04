#include <Arduino.h>
#include <HX711.h>

// HX711 circuit wiring
const int LOADCELL_DT_PIN = 5;
const int LOADCELL_SCK_PIN = 6;

HX711 scale;

void setup()
{
  Serial.begin(115200);
  scale.begin(LOADCELL_DT_PIN, LOADCELL_SCK_PIN);
}

void loop()
{

  /* The code is checking if the HX711 scale is ready to read a value. If it is ready, it reads the
  value from the scale using `scale.read()` and stores it in the variable `reading`. Then, it prints
  the reading to the serial monitor using `Serial.print()` and `Serial.println()`. */
  if (scale.is_ready())
  {
    long reading = scale.read();
    Serial.print("HX711 reading: ");
    Serial.println(reading);
  }
  else
  {
    Serial.println("HX711 not found.");
  }

  delay(1000);
}
