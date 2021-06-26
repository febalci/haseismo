#include "common.h"

char custom_MQTT_SERVER[custom_MQTT_SERVER_LEN];
char custom_MQTT_PORT[custom_MQTT_PORT_LEN];
char custom_MQTT_USERNAME[custom_MQTT_USERNAME_LEN];
char custom_MQTT_PASSWORD[custom_MQTT_PASSWORD_LEN];

const char* CONFIG_FILE = "/ConfigMQTT.json";

#define MQTT_PUB_EVENT "seismoha/event"
#define MQTT_PUB_DATA  "seismoha/data"
 
AsyncMqttClient mqttClient;

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");
//  if (WiFi.isConnected()) {
//    xTimerStart(mqttReconnectTimer, 0);
//  }
}

void onMqttPublish(uint16_t packetId) {
  Serial.print("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void initmqtt() {
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  //mqttClient.onSubscribe(onMqttSubscribe);
  //mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(custom_MQTT_SERVER, atoi(custom_MQTT_PORT));
  // If your broker requires authentication (username and password), set them below
  mqttClient.setCredentials(custom_MQTT_USERNAME, custom_MQTT_PASSWORD);
  connectToMqtt();
}

void publish_event() {
    uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_EVENT, 1, true, String("EQ").c_str());
    Serial.printf("Publishing on topic %s at QoS 1, packetId: %i", MQTT_PUB_EVENT, packetIdPub1);
}

void publish_data(String eq_date, String eq_time, String eq_xy, String eq_z) {
//    String msg = eq_date + "," + eq_time + "," + eq_xy + "," + eq_z;
    String msg = "{\"date\":\""+eq_date + "\",\"time\":\"" + eq_time + "\",\"xy\":\""+ eq_xy + "\",\"z\":\""+ eq_z +"\"}";
    uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_DATA, 1, true, msg.c_str());
    Serial.printf("Publishing on topic %s at QoS 1, packetId: %i", MQTT_PUB_DATA, packetIdPub1);
}