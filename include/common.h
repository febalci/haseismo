#ifndef __COMMON_H
#define __COMMON_H

#include <Arduino.h>


// Uncomment next line for Debug Purposes:
#define DEBUG

// MPU 6050
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
// NTP
#include <NTPClient.h>
#include <WiFiUdp.h>

//#include "RTClib.h"
#include <RtcDS3231.h>
#include <EEPROM.h>

//MQTT
#include <AsyncMqttClient.h>

#include "seismometer.h"
#include "rtc.h"
#include "led.h"
#include "configpage.h"
#include "mqtt.h"
#include "spiffs_config.h"
#include "openlog.h"

// LEDs
#define ON 1
#define OFF 0
#define LED_RED     D7
#define LED_YELLOW  D6
#define LED_GREEN   D5
//  Nodemcu Flash Button Resets WiFi Credentials
#define PIN_RESET_BUTTON 0 
//RTC
#define NTP_SERVER "pool.ntp.org"
//GMT Time Zone with sign
#define GMT_TIME_ZONE +3

#define custom_MQTT_SERVER_LEN       20
#define custom_MQTT_PORT_LEN          5
#define custom_MQTT_USERNAME_LEN     20
#define custom_MQTT_PASSWORD_LEN     20
#define custom_MPU6050_SCALE_LEN     4

extern char custom_MQTT_SERVER[custom_MQTT_SERVER_LEN],custom_MQTT_PORT[custom_MQTT_PORT_LEN],
    custom_MQTT_USERNAME[custom_MQTT_USERNAME_LEN],custom_MQTT_PASSWORD[custom_MQTT_PASSWORD_LEN],
    custom_MPU6050_SCALE[custom_MPU6050_SCALE_LEN],custom_LOG_TYPE;


extern const char* CONFIG_FILE;

extern boolean configPageSyncTime;

// DEBUG PART
#ifdef DEBUG
    #define Debug(x) Serial.print(x)
    #define Debugln(x) Serial.println(x)
    #define Debug64(x) Serial.print(uint32_t(x / 100000)); Serial.print(' '); Serial.print(uint32_t(x % 100000));
    #define Debug64ln(x) Serial.print(uint32_t(x / 100000)); Serial.print(' ');Serial.println(uint32_t(x % 100000));
    #define Debugf(x,y,z) Serial.printf(x,y,z)
#else
    #define Debug(x)
    #define Debugln(x)
    #define Debug64(x)
    #define Debug64ln(x)
#endif


#endif