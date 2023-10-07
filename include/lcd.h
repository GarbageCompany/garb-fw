#ifndef LCD_H
#define LCD_H

#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

extern hd44780_I2Cexp lcd;
void lcd_init();
void clear_line(int row); 

#endif