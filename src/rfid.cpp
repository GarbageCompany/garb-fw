#include <MFRC522.h>
#include <lcd.h>
#include <config.h>

MFRC522 mfrc522(RFID_SS_PIN, RFID_RST_PIN);

void rfid_init() {
    clear_line(2);
    lcd.setCursor(0, 1);
    lcd.print("Initializing RFID");
    SPI.begin();
    mfrc522.PCD_Init();
    clear_line(2);
    lcd.setCursor(0, 1);
    lcd.print("RFID Ready!");
    clear_line(2);
}

int byteArrayToInt(byte* byteArray) {
    int result = 0;
    
    for (int i = 0; i < 4; i++) {
        result = (result << 8) | byteArray[i];
    }
    
    return result;
}

int read_until_card() {
  if (mfrc522.PICC_IsNewCardPresent()) {
    if (mfrc522.PICC_ReadCardSerial()) {
      byte uidByte[mfrc522.uid.size];

      MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);

      for (int i = 0; i < mfrc522.uid.size; i++) {
        byte temp = mfrc522.uid.uidByte[i];
        uidByte[i] = mfrc522.uid.uidByte[mfrc522.uid.size - i - 1];
        uidByte[mfrc522.uid.size - i - 1] = temp;
      }
      int uid = byteArrayToInt(uidByte);

      mfrc522.PICC_HaltA(); // halt PICC
      mfrc522.PCD_StopCrypto1(); // stop encryption on PCD

      return uid;
    }
  }
  return -1;
}