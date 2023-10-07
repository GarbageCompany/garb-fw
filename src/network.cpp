#include <WiFi.h>
#include <config.h>
#include <lcd.h>

void wifi_connect() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        lcd.setCursor(0, 1);
        lcd.print("Connecting");
        for (int i = 0; i < 3; i++) {
            delay(500);
            lcd.print(".");
        }
        delay(500);
        clear_line(2); 
    }

    clear_line(2);
    lcd.setCursor(0, 1);
    lcd.print("Network Ready!");
}

