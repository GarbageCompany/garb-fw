#include <Wire.h>

#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

#include <config.h>

hd44780_I2Cexp lcd;

void lcd_init() {
    lcd.begin(MAX_COLS, MAX_ROWS);
    lcd.print("Garb v1.0.0");
}

void clear_line(int row) {
    row += 1;
    lcd.setCursor(0, row);
    for (int i = 0; i < MAX_COLS; i++) {
        lcd.print(" ");
    }
    lcd.setCursor(0, row);
}
