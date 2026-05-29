#include "led.h"

void led_init(void)
{
    pinMode(LED_PIN, OUTPUT);
    LED(HIGH);
}