#ifndef __LED_H
#define __LED_H

#include "Arduino.h"

#define LED_PIN 8
#define LED(x) digitalWrite(LED_PIN, x)

void led_init(void);

#endif