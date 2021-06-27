#ifndef __MQTT_H
#define __MQTT_H

void initmqtt();
void publish_data(String eq_date, String eq_time, String eq_xy, String eq_z);
void publish_available();
#endif

