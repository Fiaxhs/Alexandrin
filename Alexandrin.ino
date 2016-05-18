#include "SoftwareSerialZero.h"
#include <TinyScreen.h>
#include <TinyGPS++.h>
#include "upheaval30pt.h"

// GPS pins
const int GPS_ONOFFPin = A3;
const int GPS_SYSONPin = A2;
const int GPS_RXPin = A1;
const int GPS_TXPin = A0;
const int GPSBaud = 4800;

// Display dimensions
const int screenWidth = 96;
const int screenHeight = 64;

// GPS vars
TinyGPSPlus gps;
SoftwareSerial ss(GPS_RXPin, GPS_TXPin);

// Display
TinyScreen display = TinyScreen(TinyScreenPlus);

// Currently waiting for a GPS fix?
bool locating = false;

// display Y offset of current "new" line
int nextHeight = 0;


//=======================================
// Utils 
//=======================================

/**
 * Display centered text
 * @param str Text to display
 * @param y   optionnal Y coord on screen, will use current "free" Y
 * if not specified
 */
void centerText(String str, int y = -1)
{
  int size = display.getPrintWidth(&str[0]);
  if (y == -1) {
    y = nextHeight;
  }
  if (size > screenWidth) {
    display.setCursor(0, y);
  } else {
    display.setCursor((screenWidth - size)/2, y);
  }
  display.print(str);
  nextHeight = display.getFontHeight() + y;
}



/**
 * Well, setup.
 */
void setup() {
  // Init display
  initDisplay(15);

  // Say Hello
  splashScreen();

  // Init GPS
  initGPS();
}

/**
 * while true;
 */
void loop() {
  checkLocating();

  if (!locating)
  {
    displaySpeed();
  }

  readGPS(1000);
}


//=======================================
// GPS related Stuff
//=======================================

/**
 * Read and encode GPS data
 * @param ms reading time
 */
static void readGPS(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

/**
 * Handle GPS signal. We won't do anything unless we have a fix.
 */
void checkLocating() {
  if (!gps.location.isValid()) {
    // Service message displayed? 
    if (!locating) {
      display.clearScreen();
      display.setFont(liberationSans_8ptFontInfo);
      centerText("Need more signal...", 0);
      locating = true;
    }
  } else {
    if (locating) {
      display.clearScreen();
      locating = false;
    }
  }
}


/**
 * Init GPS Routine
 */
void initGPS()
{
  display.clearScreen();
  ss.begin(GPSBaud);
  pinMode(GPS_SYSONPin, INPUT);
  digitalWrite(GPS_ONOFFPin, LOW);
  pinMode(GPS_ONOFFPin, OUTPUT);
  delay(100);
  display.setCursor(0,0);
  display.print("Waking up GPS...");
  while (digitalRead(GPS_SYSONPin) == LOW)
  {
    // Need to wake the module
    digitalWrite(GPS_ONOFFPin, HIGH);
    delay(5);
    digitalWrite(GPS_ONOFFPin, LOW);
    delay(100);
  }

  centerText("Done!");
  delay(200);
}


//=======================================
// Display 
//=======================================

/**
 * Init display routine
 * @param brightness desired brightness 0-15
 */
void initDisplay(int brightness)
{
  display.begin();
  display.setFlip(1);
  display.on();
  display.setBrightness(brightness);
  display.fontColor(TS_16b_White, TS_16b_Black);
  display.setFont(liberationSans_12ptFontInfo);
}

/**
 * Welcome text/image/whatever
 */
void splashScreen()
{
  int nextHeight;
  display.clearScreen();
  display.fontColor(TS_16b_White, TS_16b_Black);
  display.setFont(liberationSans_12ptFontInfo);
  centerText("Je suis mon", 0);
  centerText("cher ami,");
  centerText("tres heureux");
  centerText("de te voir.");

  delay(2000);
}

/**
 * Display current speed
 * @return [description]
 */
void  displaySpeed() {
  display.clearScreen();
  display.setFont(upheavalTTBRK_30ptFontInfo);
  String groundSpeed = String(gps.speed.kmph(), 1);
  centerText(groundSpeed, 24);
}