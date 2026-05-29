#ifndef __RELAY_H
#define __RELAY_H

#include "Arduino.h"

#define RELAY_PIN 2
#define RELAY(x) digitalWrite(RELAY_PIN, x)

void relay_init(void);

#endif