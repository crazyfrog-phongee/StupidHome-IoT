#include "lcd.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

void initLCD()
{
    lcd.backlight(); // turn on lcd backlight
    lcd.init();      // initialize lcd
}

void displayLCD(int value, char *msg)
{
    lcd.clear();
    lcd.setCursor(0, 0); // set the cursor on the first row and column
    lcd.print("Phong Bep");

    lcd.setCursor(0, 1); // set the cursor on the second row and first column
    lcd.print("State: ");
    lcd.print(msg);
}