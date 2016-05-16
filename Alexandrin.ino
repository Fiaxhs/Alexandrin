#include "SoftwareSerialZero.h"
#include <TinyScreen.h>
#include <TinyGPS++.h>

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

unsigned char flappyBirdBitmap[]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};


TinyGPSPlus gps;
TinyScreen display = TinyScreen(TinyScreenPlus);
SoftwareSerial ss(GPS_RXPin, GPS_TXPin);

bool locating = false;

void setup() {
  display.begin();
  display.setFlip(1);
  display.on();
  display.setFont(liberationSans_12ptFontInfo);
  display.setBrightness(10);
  display.fontColor(TS_16b_White,TS_16b_Black);
  // Say Hello
  display.setCursor(0, 0);
  display.print("Ça va ?");  

  display.setFont(liberationSans_8ptFontInfo);
  display.setCursor(0, 37);
  display.println("Waking up GPS...");
  ss.begin(GPSBaud);

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
      display.setFont(liberationSans_16ptFontInfo);
    }
    locating = false;
  }

  
  if (!locating) {
    display.clearScreen();
    display.setCursor(35, 30);
    display.print("Speed: ");
    display.print(gps.speed.kmph());
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

void updateDisplay(){
  display.goTo(0,0);
  display.startData();
  unsigned char z = 0xFF;
  display.writeBuffer(flappyBirdBitmap, 204);
  display.endTransfer();
}
// void updateDisplay(){
//   display.goTo(0,0);
//   display.startData();
//   for(unsigned char y=0;y<groundY;y++){
    
//     for(unsigned char i=16;i<112;i++)
//       lineBuffer[i]=BGcolor;
      
//     int x=cloudOffsetX;
//     char cloud=0;
//     while(x<16+96){
//       if(y>cloudY[cloud] && y<cloudY[cloud]+8){
//         if(y<cloudY[cloud]+2 || y>cloudY[cloud]+6)
//           setBuffer(x+1, cloudWidth-2, cloudColor);
//         else
//           setBuffer(x, cloudWidth, cloudColor);
//       }
//       cloud++;
//       x+=(cloudSpacingX+cloudWidth);
//     }
    
//     x=pipeOffsetX;
//     char pipe=0;
//     while(x<16+96){
//       if(y<PipeHeight[pipe] || y>PipeHeight[pipe]+pipeSpacingY){
//         if(y<PipeHeight[pipe]-4 || y>PipeHeight[pipe]+pipeSpacingY+4)
//           setBuffer(x, pipeWidth, pipeColor);
//         else
//           setBuffer(x-1, pipeWidth+2, pipeColor);
//       }
//       pipe++;
//       x+=(pipeSpacingX+pipeWidth);
//     }
    
//     if(y>=flappyBirdY && y<flappyBirdY+flappyBirdHeight){
//       int offset=(y-flappyBirdY)*flappyBirdWidth;
//       for(int i=0;i<flappyBirdWidth;i++){
//         unsigned char color=flappyBird[offset+i];
//         if(color!=ALPHA)
//           lineBuffer[16+flappyBirdX+i]=color;
//       }
//     }
//     char wingY=flappyBirdY+3;
//     if(y>=wingY && y<wingY+wingHeight){
//       int offset=(y-wingY)*wingWidth;
//       for(int i=0;i<wingWidth;i++){
//         unsigned char color;
//         if(wingPos==0)
//           color=wingUp[offset+i];
//         if(wingPos==1)
//           color=wingMid[offset+i];
//         if(wingPos==2)
//           color=wingDown[offset+i];
//         if(color!=ALPHA)
//           lineBuffer[16+flappyBirdX+i]=color;
//       }
//     }
    
    
//     char highScoreString[3];
//     highScoreString[0]=(highScore/10)+'0';
//     highScoreString[1]=(highScore%10)+'0';
//     highScoreString[2]='\0';
    
//     putString(y,80,1,highScoreString,liberationSans_10ptFontInfo);
    
    
//     char scoreString[3];
//     scoreString[0]=(score/10)+'0';
//     scoreString[1]=(score%10)+'0';
//     scoreString[2]='\0';
    
    
//     putString(y,60,15,scoreString,liberationSans_16ptFontInfo);
    
    
//     display.writeBuffer(lineBuffer+16,96);
//   }
//   for(int z=0;z<21;z++){
//     lineBuffer[16+(z*5)]=GREEN;
//     lineBuffer[17+(z*5)]=GREEN;
//     lineBuffer[18+(z*5)]=GREEN;
//     lineBuffer[19+(z*5)]=DGREEN;
//     lineBuffer[20+(z*5)]=DGREEN;
//   }
//   display.writeBuffer(lineBuffer+16+groundOffsetX,96);
//   display.writeBuffer(lineBuffer+17+groundOffsetX,96);
//   display.writeBuffer(lineBuffer+18+groundOffsetX,96);
//   display.writeBuffer(lineBuffer+19+groundOffsetX,96);
//   display.writeBuffer(lineBuffer+20+groundOffsetX,96);
  
  
//   for(unsigned char i=16;i<112;i++)
//     lineBuffer[i]=BROWN;
//   for(unsigned char i=0;i<7;i++)
//     display.writeBuffer(lineBuffer+16,96);
  
//   display.endTransfer();
// }




//
//
//void setup()
//{
//  Serial.begin(115200);
//  ss.begin(GPSBaud);
//  pinMode(GPS_SYSONPin, INPUT);
//  digitalWrite(GPS_ONOFFPin, LOW);
//  pinMode(GPS_ONOFFPin, OUTPUT);
//  delay(100);
//  Serial.print("Attempting to wake GPS module.. ");
//  while (digitalRead( GPS_SYSONPin ) == LOW )
//  {
//    // Need to wake the module
//    digitalWrite( GPS_ONOFFPin, HIGH );
//    delay(5);
//    digitalWrite( GPS_ONOFFPin, LOW );
//    delay(100);
//  }
//  Serial.println("done.");
//  
//  Serial.print("Testing TinyGPS library v. "); Serial.println(TinyGPS::library_version());
//  Serial.println("by Mikal Hart");
//  Serial.println();
//  Serial.println("Sats HDOP Latitude  Longitude  Fix  Date       Time     Date Alt    Course Speed Card  Distance Course Card  Chars Sentences Checksum");
//  Serial.println("          (deg)     (deg)      Age                      Age  (m)    --- from GPS ----  ---- to London  ----  RX    RX        Fail");
//  Serial.println("-------------------------------------------------------------------------------------------------------------------------------------");
//}
//
//void loop()
//{
//  float flat, flon;
//  unsigned long age, date, time, chars = 0;
//  unsigned short sentences = 0, failed = 0;
//  static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;
//  
//  print_int(gps.satellites(), TinyGPS::GPS_INVALID_SATELLITES, 5);
//  print_int(gps.hdop(), TinyGPS::GPS_INVALID_HDOP, 5);
//  gps.f_get_position(&flat, &flon, &age);
//  print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6);
//  print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);
//  print_int(age, TinyGPS::GPS_INVALID_AGE, 5);
//  print_date(gps);
//  print_float(gps.f_altitude(), TinyGPS::GPS_INVALID_F_ALTITUDE, 7, 2);
//  print_float(gps.f_course(), TinyGPS::GPS_INVALID_F_ANGLE, 7, 2);
//  print_float(gps.f_speed_kmph(), TinyGPS::GPS_INVALID_F_SPEED, 6, 2);
//  print_str(gps.f_course() == TinyGPS::GPS_INVALID_F_ANGLE ? "*** " : TinyGPS::cardinal(gps.f_course()), 6);
//  print_int(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0xFFFFFFFF : (unsigned long)TinyGPS::distance_between(flat, flon, LONDON_LAT, LONDON_LON) / 1000, 0xFFFFFFFF, 9);
//  print_float(flat == TinyGPS::GPS_INVALID_F_ANGLE ? TinyGPS::GPS_INVALID_F_ANGLE : TinyGPS::course_to(flat, flon, LONDON_LAT, LONDON_LON), TinyGPS::GPS_INVALID_F_ANGLE, 7, 2);
//  print_str(flat == TinyGPS::GPS_INVALID_F_ANGLE ? "*** " : TinyGPS::cardinal(TinyGPS::course_to(flat, flon, LONDON_LAT, LONDON_LON)), 6);
//
//  gps.stats(&chars, &sentences, &failed);
//  print_int(chars, 0xFFFFFFFF, 6);
//  print_int(sentences, 0xFFFFFFFF, 10);
//  print_int(failed, 0xFFFFFFFF, 9);
//  Serial.println();
//  
//  smartdelay(1000);
//}

//
//static void print_float(float val, float invalid, int len, int prec)
//{
//  if (val == invalid)
//  {
//    while (len-- > 1)
//      Serial.print('*');
//    Serial.print(' ');
//  }
//  else
//  {
//    Serial.print(val, prec);
//    int vi = abs((int)val);
//    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
//    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
//    for (int i=flen; i<len; ++i)
//      Serial.print(' ');
//  }
//  smartdelay(0);
//}
//
//static void print_int(unsigned long val, unsigned long invalid, int len)
//{
//  char sz[32];
//  if (val == invalid)
//    strcpy(sz, "*******");
//  else
//    sprintf(sz, "%ld", val);
//  sz[len] = 0;
//  for (int i=strlen(sz); i<len; ++i)
//    sz[i] = ' ';
//  if (len > 0) 
//    sz[len-1] = ' ';
//  Serial.print(sz);
//  smartdelay(0);
//}
//
//static void print_date(Tinygps &gps)
//{
//  int year;
//  byte month, day, hour, minute, second, hundredths;
//  unsigned long age;
//  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
//  if (age == TinyGPS::GPS_INVALID_AGE)
//    Serial.print("********** ******** ");
//  else
//  {
//    char sz[32];
//    sprintf(sz, "%02d/%02d/%02d %02d:%02d:%02d ",
//        month, day, year, hour, minute, second);
//    Serial.print(sz);
//  }
//  print_int(age, TinyGPS::GPS_INVALID_AGE, 5);
//  smartdelay(0);
//}
//
//static void print_str(const char *str, int len)
//{
//  int slen = strlen(str);
//  for (int i=0; i<len; ++i)
//    Serial.print(i<slen ? str[i] : ' ');
//  smartdelay(0);
//}
