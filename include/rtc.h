#ifndef __RTC_H
#define __RTC_H

class rtc_3231{
  public:
    void syncTime(void);
    void init();
    String Date();
    String Time();
};

#endif