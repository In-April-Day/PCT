#include "exit.h"
#include "key.h"
#include "led.h"
#include "relay.h"

#define LONG_PRESS_THRESHOLD 2000

uint8_t auto_mode = 1;

void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.println();
  Serial.println("=== System Starting ===");
  Serial.println("Initializing system...");
  
  exit_init();
  delay(200);
  Serial.println("System initialization complete!");
  Serial.println();
  Serial.println("Waiting for KEY1 to enable system...");
}

void loop() {
  handle_key1();
  handle_key2();
  update_outputs();
  delay(10);
}

void handle_key1() {
  uint8_t current = KEY1;
  
  if (current == 1) {
    if (!system_enabled) {
      system_enabled = 1;
      auto_mode = 1;
      function_mode = 0;
      Serial.println("[KEY1] System ENABLED");
      Serial.println(">>> Mode: AUTO");
    }
  } else {
    if (system_enabled) {
      system_enabled = 0;
      Serial.println("[KEY1] System DISABLED");
    }
  }
}

void handle_key2() {
  static uint8_t key2_last = 0;
  static unsigned long key2_press_time = 0;
  static uint8_t long_press_triggered = 0;
  
  uint8_t current = KEY2;
  
  if (current != key2_last) {
    if (current == 1) {
      key2_press_time = millis();
      long_press_triggered = 0;
    } else {
      if (!long_press_triggered && system_enabled && !auto_mode) {
        function_mode = (function_mode + 1) % 4;
        Serial.print("[KEY2] Short press - Manual State: ");
        Serial.print(function_mode);
        Serial.println(" (incremented)");
      }
    }
    key2_last = current;
  }
  
  if (current == 1 && !long_press_triggered && system_enabled) {
    unsigned long press_duration = millis() - key2_press_time;
    if (press_duration >= LONG_PRESS_THRESHOLD) {
      long_press_triggered = 1;
      auto_mode = !auto_mode;
      Serial.print("[KEY2] Long press detected: ");
      Serial.print(press_duration);
      Serial.println(" ms");
      if (auto_mode) {
        Serial.println(">>> Mode changed to: AUTO");
      } else {
        Serial.println(">>> Mode changed to: MANUAL");
      }
    }
  }
}

void update_outputs() {
  if (system_enabled) {
    if (auto_mode) {
      LED(LOW);
      relay_control(LOW);
    } else {
      switch (function_mode) {
        case 0: {
          LED(LOW);
          relay_control(LOW);
          break;
        }
        case 1: {
          LED(LOW);
          relay_control(HIGH);
          break;
        }
        case 2: {
          LED(HIGH);
          relay_control(LOW);
          break;
        }
        case 3: {
          LED(HIGH);
          relay_control(HIGH);
          break;
        }
      }
    }
  } else {
    LED(LOW);
    relay_control(LOW);
  }
}
