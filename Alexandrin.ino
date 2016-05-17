#include "SoftwareSerialZero.h"
#include <TinyScreen.h>
#include <TinyGPS++.h>
#include "30pt.h"

// Color definitions
#define  BLACK           0x00
#define BLUE            0xE0
#define RED             0x03
#define GREEN           0x1C
#define DGREEN           0x0C
#define YELLOW          0x1F
#define WHITE           0xFF
#define ALPHA           0xFE
#define BROWN           0x32


// GPS pins
const int GPS_ONOFFPin = A3;
const int GPS_SYSONPin = A2;
const int GPS_RXPin = A1;
const int GPS_TXPin = A0;
const int GPSBaud = 4800;

TinyGPSPlus gps;
TinyScreen display = TinyScreen(TinyScreenPlus);
SoftwareSerial ss(GPS_RXPin, GPS_TXPin);

bool locating = false;

void setup() {
  display.begin();
  display.setFlip(1);
  display.on();
  display.setFont(liberationSans_12ptFontInfo);
  display.setBrightness(15);
  display.fontColor(TS_16b_White, TS_16b_Black);
  // Say Hello
  display.setCursor(0, 0);
  display.print("Ça va ?");

  display.setFont(liberationSans_8ptFontInfo);
  display.setCursor(0, 37);
  display.println("Waking up GPS...");
  ss.begin(GPSBaud);
  SerialUSB.begin(4800);

  // Init the GPS Module to wake mode
  pinMode(GPS_SYSONPin, INPUT);
  digitalWrite(GPS_ONOFFPin, LOW);
  pinMode(GPS_ONOFFPin, OUTPUT);
  delay(100);
  while (digitalRead(GPS_SYSONPin) == LOW) {
    // Need to wake the module
    digitalWrite(GPS_ONOFFPin, HIGH);
    delay(5);
    digitalWrite(GPS_ONOFFPin, LOW);
    delay(100);
  }
  display.setCursor(0, 45);
  display.println("Done!");
  display.setFont(liberationSans_16ptFontInfo);
  delay(1000);
}

void loop() {
  if (!gps.location.isValid()) {
    if (!locating) {
      display.clearScreen();
      display.setFont(liberationSans_8ptFontInfo);
      display.setCursor(0, 0);
      display.print("Need more signal...");
      locating = true;
    }
  } else {
    if (locating) {
      display.clearScreen();
      display.setFont(upheavalTTBRK_30ptFontInfo);
    }
    locating = false;
  }


  if (!locating) {
    display.clearScreen();
    char speedStr[10];
    sprintf(speedStr, "%.1f", gps.speed.kmph());
    display.setCursor(48 - display.getPrintWidth(speedStr) / 2, 30);
    display.print(speedStr);
  }

  smartdelay(1000);
}



static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
