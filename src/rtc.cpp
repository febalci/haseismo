#include "common.h"

/*
 * DS-3231 RTC
 */

//RTC_DS3231 rtc;
RtcDS3231<TwoWire> Rtc(Wire);

//char printDate[10];
//char printTime[8];

WiFiUDP ntpUDP;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//char printDate[10];
//char printTime[8];
// You can specify the time server pool and the offset, (in seconds)
// additionaly you can specify the update interval (in milliseconds).
NTPClient timeClient(ntpUDP, NTP_SERVER, GMT_TIME_ZONE * 3600 , 60000);

int timeUpdated = 0;

void rtc_3231::syncTime(void) {
  timeClient.update();

  long actualTime = timeClient.getEpochTime();
  Debug("Internet Epoch Time: ");
  Debugln(actualTime);

  RtcDateTime compiled;

  compiled.InitWithEpoch32Time(actualTime);

  if (!Rtc.IsDateTimeValid()) 
  {
      if (Rtc.LastError() != 0)
      {
          // we have a communications error
          // see https://www.arduino.cc/en/Reference/WireEndTransmission for 
          // what the number means
          Serial.print("RTC communications error = ");
          Serial.println(Rtc.LastError());
      }
      else
      {
          // Common Causes:
          //    1) first time you ran and the device wasn't running yet
          //    2) the battery on the device is low or even missing

          Serial.println("RTC lost confidence in the DateTime!");

          // following line sets the RTC to the date & time this sketch was compiled
          // it will also reset the valid flag internally unless the Rtc device is
          // having an issue

          Rtc.SetDateTime(compiled);
      }
  }

  if (!Rtc.GetIsRunning())
  {
      Serial.println("RTC was not actively running, starting now");
      Rtc.SetIsRunning(true);
  }

  Rtc.SetDateTime(compiled);
  Debug("Seismo Date: ");
  Debugln(Date());
  Debug("Seismo Time: ");
  Debugln(Time());
}

void rtc_3231::init() {
  Rtc.Begin();
  Serial.println("RTC Waiting to Start....");
  #ifdef DEBUG
  delay(250);
  #else
  delay(5000);
  #endif
  timeClient.begin();
  syncTime();

  Serial.print("Seismo Date: ");
  Serial.println(Date());
  Serial.print("Seismo Time: ");
  Serial.println(Time());
}

String rtc_3231::Date() {
  RtcDateTime now = Rtc.GetDateTime();  
  char tmpDate[10];
  sprintf (tmpDate, "%02d.%02d.%04d", now.Day(), now.Month(), now.Year());
  return tmpDate; 
}

String rtc_3231::Time() {
  RtcDateTime now = Rtc.GetDateTime();
  char tmpTime[8];
  sprintf (tmpTime, "%02d:%02d:%02d", now.Hour(), now.Minute(), now.Second());
  return tmpTime;
}