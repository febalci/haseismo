#include "common.h"

char custom_LOG_TYPE;

openlog::openlog(long baudrate)
{
  Serial1.begin(baudrate);
}

void openlog::gotoCommandMode(){
  //Send three control z to enter OpenLog command mode
  Debugln("Set OpenLog Command Mode...");
  Serial1.write(26);
  Serial1.write(26);
  Serial1.write(26);
/*  // Use this when RX works, can remove delay(100) below then...
  while (1) {
    if (OpenLog.available())
      if (OpenLog.read() == '>') break;
  }
*/
  delay(100);
}

void openlog::exitCommandMode(){
  //Send three control z to enter OpenLog command mode
  Debugln("Exit OpenLog Command Mode...");
  Serial1.write(26);
  delay(100);
}

void openlog::appendFile(char *fileName){
  Debug("Append File: ");
  Debug(fileName);
  Debugln(" to OpenLog");
  Serial1.print("append ");
  Serial1.print(fileName);
  Serial1.write(13); //This is \r
  delay(100);
}

void openlog::cleanLogFile(char *fileName){
  Debug("Remove File: ");
  Debug(fileName);
  Debugln(" from OpenLog");
  Serial1.print("rm ");
  Serial1.print(fileName);
  Serial1.write(13); //This is \r
  delay(100);
}

void openlog::writetoOpenLog(String date, String time,float vector1, float vector2){  // Write DateTime and accel to OpenLog
  String temptxt = String(date)+","+String(time)+","+String(vector1)+","+String(vector2);
  if (custom_LOG_TYPE == '1') {
    Serial.println(temptxt);
  } else if (custom_LOG_TYPE == '2') {
    Serial1.println(temptxt);
  } else if (custom_LOG_TYPE == '3') {
    Serial.println(temptxt);
    Serial1.println(temptxt);
  }

}