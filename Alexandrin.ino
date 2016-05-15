//-------------------------------------------------------------------------------
//  TinyCircuits GPS TinyShield Logging Example
//  Last updated 7 April 2016
//  
//  This example uses logs GPS NMEA sentences to SD card. If it doesn't detect an SD
//  card at startup, it will output data to the serial terminal.
//  With the Telit SE868 V2 module with Glonass support, some messages come through
//  as GN** sentences instead of GP**. These are changed back to GP** before logging
//  so that they don't cause problems with programs like Google Earth.
//  The Software Serial library should be modified for a larger buffer- 256 is enough
//  for GGA and RMC sentences at 1Hz. In SoftwareSerial.cpp, the change looks like:
//  #define _SS_MAX_RX_BUFF 256
//
//  Some GPS modules have been shipped with 4800 baud instead of 9600- try this if
//  you see bad data.
//
//  Written by Ben Rose for TinyCircuits, http://TinyCircuits.com
//
//-------------------------------------------------------------------------------

#include "SoftwareSerialZero.h"
#include <TinyScreen.h>
#include <TinyGPS++.h>

// GPS pins
const int GPS_ONOFFPin = A3;
const int GPS_SYSONPin = A2;
const int GPS_RXPin = A1;
const int GPS_TXPin = A0;
const int GPSBaud = 4800;

TinyGPSPlus gpsPlus;
TinyScreen screen = TinyScreen(TinyScreenPlus);
SoftwareSerial gpsSerial(GPS_RXPin, GPS_TXPin);

bool locating = false;

void setup() {
  screen.begin();
  screen.setFlip(1);
  screen.on();
  screen.setFont(liberationSansNarrow_12ptFontInfo);
  screen.fontColor(TS_8b_Blue,TS_8b_Black);
  screen.setBrightness(10);
  screen.fontColor(TS_16b_White,TS_16b_Black);
  // Say Hello
  screen.setCursor(34, 25);
  screen.print("Hello!");  

  screen.setFont(liberationSansNarrow_8ptFontInfo);
  screen.setCursor(0, 37);
  screen.println("Waking up GPS...");
  gpsSerial.begin(GPSBaud);

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
  screen.setCursor(0, 45);
  screen.println("Done!");
  delay(1000);
}

void loop() {
  screen.setFont(liberationSansNarrow_12ptFontInfo);

  while (gpsSerial.available() > 0) {
    gpsPlus.encode(gpsSerial.read());
    int nbGps = gpsPlus.satellites.value();
    if (nbGps < 4) {
      if (!locating) {
        screen.clearScreen();
        screen.setCursor(0, 0);
        screen.print("Looking for satellites...");
        screen.setCursor(0, 12);
        screen.print(nbGps);
        screen.print("/4 connected");
        locating = true;
      }
      screen.setCursor(0, 12);
      screen.print(nbGps);
    } else {
      if (locating) {
        screen.clearScreen();
      }
      locating = false;
    }

    
    if (!locating) {
      screen.setCursor(0, 0);
      screen.print("Speed: ");
      screen.print(gpsPlus.speed.kmph());
      screen.setCursor(0, 50);
      screen.print(nbGps);
    }
  }
  delay(100);
}
