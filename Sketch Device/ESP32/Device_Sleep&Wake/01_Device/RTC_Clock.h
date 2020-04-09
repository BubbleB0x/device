/*
 * Library RTC Module: https://github.com/adafruit/RTClib
 * 
 */

#include "RTClib.h";

RTC_DS3231 rtc;
String Data;
String Ora;
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
  Serial.println(Data);
  return Data;
}

//----------------- GET ORA RTC CLOCK -------------------------------------
String getOra()
{
  DateTime now = rtc.now();
  sprintf(buffer, "%02d:%02d", now.hour(), now.minute());;
  Ora = buffer;
  Serial.println(Ora);
  return Ora;
}
