#include "common.h"
// WiFi & WebServer
#include <DNSServer.h>
#include <AsyncElegantOTA.h>
#include "webserver.h"
#include <ESPAsync_WiFiManager.h>              //https://github.com/khoih-prog/ESPAsync_WiFiManager

AsyncWebServer ota_server(80);
DNSServer dnsServer;
boolean configPageSyncTime = false;

String webpage::config_processor(const String& var){ // MQTT Server Details
  if(var == "MQTTPLACEHOLDER"){
    String server_details = "";
    server_details = "<fieldset><legend><b>&nbsp;MQTT parameters&nbsp;</b></legend>";
    server_details += "<b>Host</b><br/><input id='mh' name='mh' value='"+String(custom_MQTT_SERVER)+"'><br/><br/>";
    server_details += "<b>Port</b> (1883)<br/><input id='ml' name='ml' value='"+String(custom_MQTT_PORT)+"' placeholder='1883'><br/><br/>";
    server_details += "<b>User</b><br/><input id='mu' name='mu' value='"+String(custom_MQTT_USERNAME)+"'><br/><br/>";
    server_details += "<b>Password</b><br/><input id='mp' name='mp' type='password' value='"+String(custom_MQTT_PASSWORD)+"'><br/>";
    server_details += "</fieldset><fieldset><legend><b>&nbsp;Other parameters&nbsp;</b></legend> <b>MPU6050 Scale</b><br/>";
    server_details += "<input id='sc' name='sc' value='"+String(custom_MPU6050_SCALE)+"' placeholder='64'><br/><br/>";
    server_details += "<b>Logging</b><br><select id='lg' name='lg'><option value='0'>None</option><option value='1'>Serial Only</option>";
    server_details += "<option value='2'>Openlog Only</option><option value='3'>Both</option>";
    server_details += "</select></fieldset><script>document.getElementById('lg').value = '"+String(custom_LOG_TYPE)+"';</script>";
    Debug(server_details);
    return server_details;
  }
  return String();
}

void webpage::init_webpage() {
  ESPAsync_WiFiManager ESPAsync_wifiManager(&ota_server, &dnsServer, "SeismoAP");
  Debug("Starting WiFiManager...");  
  //ESPAsync_wifiManager.resetSettings();   //reset saved settings
  //ESPAsync_wifiManager.setAPStaticIPConfig(IPAddress(192,168,186,1), IPAddress(192,168,186,1), IPAddress(255,255,255,0));
  ESPAsync_wifiManager.autoConnect("SeismoAP");

  if (WiFi.status() == WL_CONNECTED) { Serial.print(F("Connected. Local IP: ")); Serial.println(WiFi.localIP()); }
  else { Serial.println(ESPAsync_wifiManager.getStatus(WiFi.status())); }
  
  // Handle Config Page
  ota_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html, webpage::config_processor);
  });

  // Handle Reset WiFi Request
  ota_server.on("/rt", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", "Reset OK");
    webpage::HandleResetModule();
  });

  // Handle Config RTC Time Update
  ota_server.on("/dt", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", "RTC Update OK");
    configPageSyncTime = true;
  });

  // Handle Config Save
  ota_server.on("/sv", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("mh") && request->hasParam("ml")) {
        strcpy(custom_MQTT_SERVER, request->getParam("mh")->value().c_str());
        strcpy(custom_MQTT_PORT, request->getParam("ml")->value().c_str());
        strcpy(custom_MQTT_USERNAME, request->getParam("mu")->value().c_str());
        strcpy(custom_MQTT_PASSWORD, request->getParam("mp")->value().c_str());
        strcpy(custom_MPU6050_SCALE, request->getParam("sc")->value().c_str());
        custom_LOG_TYPE = request->getParam("lg")->value()[0];
    }
    else {
        custom_MQTT_SERVER[0] = '\0';
        custom_MQTT_PORT[0] = '\0';
        custom_MQTT_USERNAME[0] = '\0';
        custom_MQTT_PASSWORD[0] = '\0';
        custom_MPU6050_SCALE[0] = '\0';
        custom_LOG_TYPE = '\0';
    }
    writeConfigFile();
    request->send(200, "text/html", "Saved OK");
  });

  AsyncElegantOTA.begin(&ota_server);    // Start ElegantOTA
  ota_server.begin();
  Debugln("Async HTTP server started");

}

void webpage::HandleResetModule() {
    Serial.println("Nodemcu Reset/Flash Pressed, Reseting Details...");
    removeConfigFile(); //Remove MQTT config file
    WiFi.disconnect(); //Remove WiFi Details
    delay(1000);
    ESP.reset();
}

void webpage::asyncloop() {
    AsyncElegantOTA.loop();
}

