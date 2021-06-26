#ifndef __OPENLOG_H
#define __OPENLOG_H

class openlog{
  public:
    openlog(long baudrate);
    void gotoCommandMode();
    void exitCommandMode();
    void cleanLogFile(char *fileName);
    void appendFile(char *fileName);
    void writetoOpenLog(String date, String time, float vector1, float vector2);
};

#endif