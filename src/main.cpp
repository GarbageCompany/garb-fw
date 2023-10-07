#include <Arduino.h>
#include <lcd.h>
#include <network.h>
#include <rfid.h>
#include <config.h>
#include <HTTPClient.h>

struct RFIDInfo
{
  String rfidID;
  String name;
};

RFIDInfo rfidPair[] = {
    {"768490308", "Chanakan"}};

RFIDInfo checkIfRFIDExists(int i);

// FreeRTOS Tasks
void handleCard(void *params);
void handleTrash(void *params);

void clearState();

RFIDInfo currentRFIDInfo = {"", ""};
bool isGarbageIn = false;
bool isMetal = false;

int metalSensorPin = 17;


HTTPClient http;
int httpCode = 0;

void setup()
{
  if(DEBUG) {
    Serial.begin(115200);
  }
  pinMode(metalSensorPin, INPUT);

  lcd_init();
  rfid_init();
  wifi_connect();

  delay(500);
  lcd.clear();

  xTaskCreate(
      handleCard,
      "handleCard",
      4096,
      NULL,
      1,
      NULL);

  xTaskCreate(
      handleTrash,
      "handleTrash",
      2048,
      NULL,
      1,
      NULL);
}

void handleTrash(void *params)
{
  for (;;)
  {
    Serial.print("handleTrash: ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));
    isGarbageIn = !bool(digitalRead(metalSensorPin)); // change to ir sensor
    isMetal = digitalRead(metalSensorPin);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    // TOOD: handle servo and bin control here
  }
}

void handleGoogleSheetPoint(void *params)
{
  http.begin("https://maker.ifttt.com/trigger/student_gc_points_increment/with/key/" + IFTTT_KEY + "?value1=" + currentRFIDInfo.rfidID + "&value2=" + currentRFIDInfo.name + "&value3=" + isMetal);
  http.GET();
  http.end();
  vTaskDelete(NULL);
}

void handleCard(void *params)
{
  bool processed = false;
  for (;;)
  {
    Serial.print("handleCard: ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));
    int uid = read_until_card();
    if (uid == -1)
    {
      vTaskDelay(20 / portTICK_PERIOD_MS);
      continue;
    }

    RFIDInfo rfidInfo = checkIfRFIDExists(uid);

    lcd.clear();
    if (rfidInfo.rfidID != "")
    {
      lcd.print("Hello! " + rfidInfo.name);
      lcd.setCursor(0, 1);
      lcd.print("Put the trash in");

      currentRFIDInfo = rfidInfo;

      for (int i = 0; i < 5; i++)
      {
        if (isGarbageIn)
        {
          xTaskCreate(
              handleGoogleSheetPoint,
              "handleSheet",
              4096,
              NULL,
              1,
              NULL);

          lcd.clear();
          lcd.print("Thank you!");
          lcd.setCursor(0, 1);
          lcd.print("See you again!");
          vTaskDelay(2000 / portTICK_PERIOD_MS);
          lcd.clear();

          processed = true;
          break;
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
      }

      if (processed)
      {
        processed = false;
        clearState();
        continue;
      }

      lcd.clear();
      lcd.print("No trash found");
      lcd.setCursor(0, 1);
      lcd.print("Dismissing...");

      clearState();
      vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    else
    {
      lcd.print("Unknown RFID");
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    lcd.clear();
  }
}

RFIDInfo checkIfRFIDExists(int i)
{
  String str = String(i);

  for (int j = 0; j < sizeof(rfidPair) / sizeof(rfidPair[0]); j++)
  {
    if (rfidPair[j].rfidID == str)
    {
      return rfidPair[j];
    }
  }

  RFIDInfo emptyRFIDInfo = {"", ""};
  return emptyRFIDInfo;
}

void clearState()
{
  currentRFIDInfo = {"", ""};
  isGarbageIn = false;
  isMetal = false;
}

void loop() {}