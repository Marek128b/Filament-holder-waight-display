#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HX711.h>

const int buttonPin1 = 2; // the number of the pushbutton 1 pin
const int buttonPin2 = 3; // the number of the pushbutton 2 pin

float tara_weight_rolls[] = {150, 120};
String roll_names[] = {"ecoPLA Schwarz", "copymaster3D"};

int current_roll = 0;
int maxRolls = roll_names->length();

byte current_screen = 0;

// HX711 circuit wiring
const int LOADCELL_DT_PIN = 5;
const int LOADCELL_SCK_PIN = 6;

#define CALIBRATION_FACTOR 207.09 // In our case, the reading is 54672. The known weight is 264g, so our calibration factor will be: 54672/264 = 207.09

HX711 scale;
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void screen(byte nr);

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void setup()
{
  /* The code block is initializing various components and setting up the necessary configurations for
  the program to run correctly. */
  Serial.begin(115200);
  scale.begin(LOADCELL_DT_PIN, LOADCELL_SCK_PIN);
  lcd.init(); // initialize the lcd
  lcd.backlight();
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  scale.set_scale(CALIBRATION_FACTOR); // this value is obtained by calibrating the scale with known weights
  scale.tare();                        // reset the scale to 0
}

void loop()
{
  screen(current_screen);
}

void screen(byte nr)
{
  switch (nr)
  {
  case 0:
    /* This code block is checking if the HX711 scale is ready to read data. If it is ready, it
    calculates the weight reading by subtracting the tare weight of the current roll from the units
    obtained from the scale. It then prints the reading on the serial monitor and displays it on the
    LCD screen along with the name of the current roll. If the HX711 scale is not ready, it prints a
    message on the serial monitor and the LCD screen indicating that the scale was not found. */
    if (scale.is_ready())
    {
      long reading = scale.get_units(8) - tara_weight_rolls[current_roll];
      Serial.print("HX711 reading: ");
      Serial.println(reading);
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("Name: ");
      lcd.print(roll_names[current_roll]);

      lcd.setCursor(0, 1);
      lcd.print("Weight: ");
      lcd.print(reading);
      lcd.print("g");
    }
    else
    {
      Serial.println("HX711 not found.");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("HX711 not found.");
    }
    break;
  case 1:
    /* The code block is implementing a loop that allows the user to select a filament roll. */
    int select_roll = 0;
    while (true)
    {
      /* The code block is checking the state of two pushbuttons (`buttonPin1` and `buttonPin2`) and
      updating the `select_roll` variable accordingly. */
      bool lastState1 = true;
      bool lastState2 = true;

      /* This code block is checking the state of the pushbutton connected to `buttonPin1`. */
      if (digitalRead(buttonPin1) != lastState1 && !digitalRead(buttonPin1))
      {
        select_roll++;
        delay(10);
        lcd.clear();
      }

      /* This code block is checking the state of the pushbutton connected to `buttonPin2`. */
      if (digitalRead(buttonPin2) != lastState2 && !digitalRead(buttonPin2))
      {
        select_roll--;
        delay(10);
        lcd.clear();
      }

      /* This code block is checking if both `buttonPin1` and `buttonPin2` are pressed
      simultaneously. If both buttons are pressed, it breaks out of the loop and continues with
      the rest of the code. */
      if (!digitalRead(buttonPin1) && !digitalRead(buttonPin2))
      {
        break;
      }

      /* The code block is checking if the value of `select_roll` is less than or equal to 0. If it is,
      it sets the value of `select_roll` to 0. This ensures that the selected roll index does not go
      below the minimum value (0). */
      if (select_roll <= 0)
      {
        select_roll = 0;
      }
      
      /* The code block is checking if the value of `select_roll` is greater than or equal to the
       maximum number of rolls (`maxRolls`). If it is, it sets the value of `select_roll` to
       `maxRolls`. This ensures that the selected roll index does not exceed the maximum value,
       preventing any out-of-bounds errors when accessing the `roll_names` array. */
      if (select_roll >= maxRolls)
      {
        select_roll = maxRolls;
      }

      lcd.setCursor(0, 0);
      lcd.print("select Filament");

      lcd.setCursor(0, 1);
      lcd.print(roll_names[select_roll]);
    }
    /* The code `current_roll = select_roll;` is assigning the value of the variable `select_roll` to
       the variable `current_roll`. This is done to store the selected filament roll for later use. */
    current_roll = select_roll;
    current_screen = 0;
    break;
  }
}