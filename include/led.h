#ifndef __LED_H
#define __LED_H

#define LED_init()            \
pinMode(LED_GREEN,  OUTPUT);  \
pinMode(LED_YELLOW, OUTPUT);  \
pinMode(LED_RED,    OUTPUT)

#define LED(green, yellow, red)    \
digitalWrite(LED_GREEN,  green);   \
digitalWrite(LED_YELLOW, yellow);  \
digitalWrite(LED_RED,    red)

#define LED_quake()            digitalWrite(LED_RED,    ON)
#define LED_quake_end()        digitalWrite(LED_RED,    OFF)

#endif