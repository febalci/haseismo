#include <FS.h> //this needs to be first, or it all crashes and burns...
#include "common.h"
#include <ArduinoJson.h>          // https://github.com/bblanchon/ArduinoJson

void readConfigFile() {
  if (SPIFFS.begin()) {
    Debugln("Mounted SPIFFS");
    if (SPIFFS.exists(CONFIG_FILE)) {
      //file exists, reading and loading
      Debugln("Reading config file");
      File configFile = SPIFFS.open(CONFIG_FILE, "r");
      if (configFile) {
        Debugln("Opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc,buf.get());
        if (error) {
            Serial.print(F("deserializeJson() failed with code "));
            Serial.println(error.c_str());
            return;
        }
        strcpy(custom_MQTT_SERVER, doc["mqtt_server"]);
        strcpy(custom_MQTT_PORT, doc["mqtt_port"]);
        strcpy(custom_MQTT_USERNAME, doc["mqtt_user"]);
        strcpy(custom_MQTT_PASSWORD, doc["mqtt_pass"]);
        strcpy(custom_MPU6050_SCALE, doc["mpu6050_scale"]);
        custom_LOG_TYPE = doc["custom_logtype"];

        Serial.println(custom_MQTT_SERVER);
        Serial.println(custom_MQTT_PORT);
        Serial.println(custom_MQTT_USERNAME);
        Serial.println(custom_MQTT_PASSWORD);
        Serial.println(custom_MPU6050_SCALE);
        Serial.println(custom_LOG_TYPE);
      }
    } else {
        Serial.println("No MQTT Details Set yet...");
        strcpy(custom_MQTT_SERVER, "");
        strcpy(custom_MQTT_PORT, "1883");
        strcpy(custom_MQTT_USERNAME, "");
        strcpy(custom_MQTT_PASSWORD, "");
        strcpy(custom_MPU6050_SCALE, "64");
        custom_LOG_TYPE = '3';
    }
  } else {
    Serial.println("failed to mount FS");
  }

}

void writeConfigFile() {
    Debugln("Saving Config to SPIFFS");
    DynamicJsonDocument doc(1024);
    doc["mqtt_server"] = custom_MQTT_SERVER;
    doc["mqtt_port"] = custom_MQTT_PORT;
    doc["mqtt_user"] = custom_MQTT_USERNAME;
    doc["mqtt_pass"] = custom_MQTT_PASSWORD;
    doc["mpu6050_scale"] = custom_MPU6050_SCALE;
    doc["custom_logtype"] = custom_LOG_TYPE;

    File configFile = SPIFFS.open(CONFIG_FILE, "w");
    if (!configFile) {
      Serial.println("Failed to open config file for writing");
    }
    serializeJsonPretty(doc, Serial);
    serializeJson(doc, configFile);

    Debugln("Config file written...");

    configFile.close();
}

void removeConfigFile() {
  SPIFFS.remove(CONFIG_FILE); //Remove MQTT config file
}
