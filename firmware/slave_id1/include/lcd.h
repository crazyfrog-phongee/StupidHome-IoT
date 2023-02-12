#ifndef LCD_H__
#define LCD_H__

#include <LiquidCrystal_I2C.h>

void initLCD(void);
void displayLCD(int value, char* msg);

#endif