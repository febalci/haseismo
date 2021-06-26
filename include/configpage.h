#ifndef __CONFIGPAGE_H
#define __CONFIGPAGE_H

class webpage{
  public:
    void init_webpage();
    void asyncloop();
    static void HandleResetModule();
  private:
    static String config_processor(const String& var);
};

#endif