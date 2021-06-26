#include "common.h"
// WiFi & WebServer
#include <DNSServer.h>
#include <ESPAsync_WiFiManager.h>              //https://github.com/khoih-prog/ESPAsync_WiFiManager
#include <AsyncElegantOTA.h>
#include "webserver.h"


const char* CONFIG_FILE = "/ConfigMQTT.json";

// Default configuration values for Adafruit IO MQTT
// This actually works
#define AIO_SERVER              "io.adafruit.com"
#define AIO_SERVERPORT          "1883" //1883, or 8883 for SSL
#define AIO_USERNAME            "private" //Adafruit IO
#define AIO_KEY                 "private"

// Labels for custom parameters in WiFi manager
#define AIO_SERVER_Label             "AIO_SERVER_Label"
#define AIO_SERVERPORT_Label         "AIO_SERVERPORT_Label"
#define AIO_USERNAME_Label           "AIO_USERNAME_Label"
#define AIO_KEY_Label                "AIO_KEY_Label"

// Just dummy topics. To be updated later when got valid data from FS or Config Portal
String MQTT_Pub_Topic   = "private/feeds/Temperature";

// Variables to save custom parameters to...
// I would like to use these instead of #defines
#define custom_AIO_SERVER_LEN       20
#define custom_AIO_PORT_LEN          5
#define custom_AIO_USERNAME_LEN     20
#define custom_AIO_KEY_LEN          40

char custom_AIO_SERVER[custom_AIO_SERVER_LEN];
char custom_AIO_SERVERPORT[custom_AIO_PORT_LEN];
char custom_AIO_USERNAME[custom_AIO_USERNAME_LEN];
char custom_AIO_KEY[custom_AIO_KEY_LEN];

// Function Prototypes
void MQTT_connect();
bool readConfigFile();
bool writeConfigFile();

/*
 * Wi-Fi
 */
AsyncWebServer ota_server(80);
DNSServer dnsServer;

int RESET = 0; 

int alarmcounter=0;
boolean calibrated = false;

void HandleResetModule()
{
  Debugln("Correct password, module resetting");
//  ota_server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request) {
//    request->send(200, "text/html", RESETTING_HTML);
//  });
		Serial.println("Nodemcu Reset/Flash Pressed, deleting WiFi Credentials");
		WiFi.disconnect();
		delay(1000);
    ESP.reset();
}

String processor(const String& var){
  //Serial.println(var);
  if(var == "BUTTONPLACEHOLDER"){
    String buttons = "";
    buttons += "<h4>Reset WiFi</h4><label class=\"switch\"><input type=\"checkbox\" id=\"2\" " + outputState(2) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>Output - GPIO 4</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"4\" " + outputState(4) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>Output - GPIO 33</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"33\" " + outputState(33) + "><span class=\"slider\"></span></label>";
    return buttons;
  }
  return String();
}

void setup(){
  pinMode(PIN_RESET_BUTTON, INPUT);
  LED_init();
  LED(OFF,ON,OFF);

  // initialize serial communication
  Serial.begin(115200);
  delay(1000);
  // Log file naming
  Serial.write(26);
  Serial.write(26);
  Serial.write(26);
  delay(100);
  Serial.print ("rm log*.txt\r");
  delay(1000);
  Serial.print ("append seismo.txt\r");
  delay(1000);
  Serial.write(26);
  delay(100);
  
  ESPAsync_WiFiManager ESPAsync_wifiManager(&ota_server, &dnsServer, "SeismoAP");
  Debug("Starting wifiManager...");  
  //ESPAsync_wifiManager.resetSettings();   //reset saved settings
  //ESPAsync_wifiManager.setAPStaticIPConfig(IPAddress(192,168,186,1), IPAddress(192,168,186,1), IPAddress(255,255,255,0));
  ESPAsync_wifiManager.autoConnect("SeismoAP");

  if (WiFi.status() == WL_CONNECTED) { Serial.print(F("Connected. Local IP: ")); Serial.println(WiFi.localIP()); }
  else { Serial.println(ESPAsync_wifiManager.getStatus(WiFi.status())); }
  
  // Handle Config Page
  ota_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", index_html);
  });

  // Handle Reset WiFi Request
  ota_server.on("/rt", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", "Reset OK");
    HandleResetModule();
  });

  AsyncElegantOTA.begin(&ota_server);    // Start ElegantOTA
  ota_server.begin();
  Serial.println("Async HTTP server started");

  rtcInit();
  seismometerInit();  
}

void loop(){  
  RESET = digitalRead(PIN_RESET_BUTTON);
	if (RESET == LOW) {
		Serial.println("Nodemcu Reset/Flash Pressed, deleting WiFi Credentials");
		WiFi.disconnect();
		delay(1000);
    ESP.reset();
	}

  AsyncElegantOTA.loop();

  if (not calibrated) {
    calibrate_MPU6050();
    calibrated = true;
    LED(ON,OFF,OFF);
  }

  //MPU-6050
  readaccel();
  rtcDateTime();

  Serial.print(printDate);
  Serial.print(" ");
  Serial.print(printTime);
  Serial.print(",");
  Serial.print(xy_vector_mag); 
  Serial.print(",");
  Serial.println(z_vector_mag); 

// EARTHQUAKE!!!
  if (xy_vector_mag >= 1.0 || z_vector_mag >= 1.0) {
    LED_quake();
    alarmcounter = 30;
  } else {
    if (alarmcounter>0) alarmcounter--;
  }
  if (alarmcounter==0) LED_quake_end();
  
  delay(100);                
}

