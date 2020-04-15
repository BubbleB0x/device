/*
 * Library RTC Module: https://github.com/adafruit/RTClib
 * 
 * 
 * Connettere Il modulo RTC all'ESP32
 *    
 *    RTC CLOCK  -   ESP32
 *      VCC           3V
 *      GND          GND
 *      SDA        GPIO 22
 *      SCL        GPIO 21
 */

#include "RTClib.h";

RTC_DS3231 rtc;
String Data;
String DataFile;
String Ora;
String Temp;
int t;
char buffer [25] = "";

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void initRTC()
{
  if (! rtc.begin()) 
  {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) 
  {
    Serial.println("RTC lost power, lets set the time!");
    // If the RTC have lost power it will sets the RTC to the date & time this sketch was compiled in the following line
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}

void setRTC()
{
  rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
}

//------------------ GET DATA RTC CLOCK ----------------------------------
String getData()
{
  DateTime now = rtc.now();
  sprintf(buffer, "%04d/%02d/%02d" , now.year(), now.month(), now.day());
  Data = buffer;
  return Data;
}

//------------------ GET DATA RTC CLOCK ----------------------------------
String getDataFile()
{
  DateTime now = rtc.now();
  sprintf(buffer, "%04d_%02d_%02d" , now.year(), now.month(), now.day());
  DataFile = buffer;
  return DataFile;
}

//----------------- GET ORA RTC CLOCK -------------------------------------
String getOra()
{
  DateTime now = rtc.now();
  sprintf(buffer, "%02d:%02d", now.hour(), now.minute());;
  Ora = buffer;
  return Ora;
}

//---------------- GET TEMPERATURA RTC ------------------------------------
String getTemp()
{
  t = (int)rtc.getTemperature();
  Temp = (String)t;
  Temp = Temp + "°C";
  
  return Temp;
}
