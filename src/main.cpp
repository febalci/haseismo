#include "common.h"

int RESET = 0; 
int alarmcounter=0;
boolean eqEnd=true;

openlog OpenLog(115200);

char SeismoLog[11] = "seismo.txt";
char RemovableLog[9] = "log*.txt";

rtc_3231 Rtc_3231;

seismometer seismo;

webpage web_page;

void setup(){
  pinMode(PIN_RESET_BUTTON, INPUT); // Flash Button on NodeMCU resets everything
  LED_init();
  LED(OFF,ON,OFF); // Light Yellow LED during whole init phase

  Serial.begin(115200);   // Initialize serial communication
  // Prepare and use OpenLog SD Card Logging
  OpenLog.gotoCommandMode();
  OpenLog.cleanLogFile(RemovableLog);
  OpenLog.appendFile(SeismoLog);
  OpenLog.exitCommandMode();


  web_page.init_webpage(); // Start WifiManager and Config Web Page
  readConfigFile(); // Check if MQTT Log File is there
  
  Rtc_3231.init(); // Init RTC

  initmqtt(); // Connect to MQTT Server
 
  seismo.init(); // Init MPU6050
  seismo.calibrate();
  publish_data(Rtc_3231.Date(),Rtc_3231.Time(),"0.0","0.0"); // Send MQTT EQ Details
  LED(ON,OFF,OFF);
}

void loop(){  
//Check Flash Button Press
  RESET = digitalRead(PIN_RESET_BUTTON);
	if (RESET == LOW) web_page.HandleResetModule();

//Check Config Page RTC Sync Time Button
  if (configPageSyncTime) {
    Debugln("Resyncing Date/Time on RTC...");
    Rtc_3231.syncTime();
    configPageSyncTime = false;
  }

  web_page.asyncloop(); 

  seismo.readaccel();   // Read MPU-6050

  OpenLog.writetoOpenLog(Rtc_3231.Date(),Rtc_3231.Time(),seismo.xy_vector_mag,seismo.z_vector_mag); // Write to OpenLog

// EARTHQUAKE!!!
  if (seismo.xy_vector_mag >= 1.0 || seismo.z_vector_mag >= 1.0) {
    eqEnd = false;
    LED_quake(); // Light Red Led
    publish_data(Rtc_3231.Date(),Rtc_3231.Time(),String(seismo.xy_vector_mag),String(seismo.z_vector_mag)); // Send MQTT EQ Details
    alarmcounter = 30; // Make Led Wait 3 Seconds
  } else {
    if (alarmcounter>0) alarmcounter--;
  }
  if (alarmcounter==0 && not eqEnd) {
    LED_quake_end(); // EQ Ends Red Led Off
    publish_data(Rtc_3231.Date(),Rtc_3231.Time(),"0.0","0.0"); // Reset EQ Values
    eqEnd = true;
  }

  delay(100);                
}

