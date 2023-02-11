#include "lcd.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

void initLCD()
{
    lcd.backlight(); // turn on lcd backlight
    lcd.init();      // initialize lcd
}

void displayLCD(float h, float t)
{
    lcd.clear();
    lcd.setCursor(0, 0); // set the cursor on the first row and column
    lcd.print("Humidity: ");
    lcd.print(h, 1); // print the humidity
    lcd.print("%");
    lcd.setCursor(0, 1); // set the cursor on the second row and first column
    lcd.print("Temp:     ");
    lcd.print(t, 1); // print the temperature
    lcd.print(char(223));
    lcd.print("C");
}