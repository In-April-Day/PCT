// 引脚定义（按你硬件修改）
#define LED_PIN     2
#define BUTTON_PIN  3
#define RELAY_PIN   4

// LED 定时（1秒闪烁）
unsigned long ledPrev = 0;
const long ledInterval = 1000;
bool ledState = LOW;

// 继电器定时（2秒切换）
unsigned long relayPrev = 0;
const long relayInterval = 2000;
bool relayState = LOW;

// 按键消抖
unsigned long btnPrev = 0;
const long btnDebounce = 50;
bool lastBtnState = HIGH;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // 上拉输入
  digitalWrite(LED_PIN, ledState);
  digitalWrite(RELAY_PIN, relayState);
  Serial.begin(115200);
}

void loop() {
  unsigned long now = millis();

  // 1. LED 非阻塞闪烁
  if (now - ledPrev >= ledInterval) {
    ledPrev = now;
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    Serial.print("LED: "); Serial.println(ledState ? "ON" : "OFF");
  }

  // 2. 继电器非阻塞定时开关
  if (now - relayPrev >= relayInterval) {
    relayPrev = now;
    relayState = !relayState;
    digitalWrite(RELAY_PIN, relayState);
    Serial.print("RELAY: "); Serial.println(relayState ? "ON" : "OFF");
  }

  // 3. 按键检测（带消抖，不阻塞）
  bool btnNow = digitalRead(BUTTON_PIN);
  if (btnNow != lastBtnState) {
    btnPrev = now;
  }
  if ((now - btnPrev) > btnDebounce) {
    if (btnNow == LOW) { // 按下
      Serial.println("BUTTON PRESSED");
      // 可在这里加：翻转LED或继电器等
      // ledState = !ledState; digitalWrite(LED_PIN, ledState);
    }
  }
  lastBtnState = btnNow;
}