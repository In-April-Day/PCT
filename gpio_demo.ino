#define KEY1  20
#define KEY2  21
#define LIGHT 6
#define FAN   7
#define LED   1

bool powerOn = false;
bool isLight = true;

// KEY1 state
int idle1;
int st1, lr1;
unsigned long tc1;
bool ready1;

// KEY2 state
int idle2;
int st2, lr2;
unsigned long tc2;
bool ready2;

bool isPress(int pin, int &idle, int &st, int &lr, unsigned long &tc, bool &ready) {
  int r = digitalRead(pin);
  if (!ready) {
    idle = r; st = r; lr = r; ready = true;
    return false;
  }
  if (r != lr) { lr = r; tc = millis(); }
  if (millis() - tc >= 50 && lr != st) {
    if (st == idle && lr != idle) {
      st = lr;
      return true;
    }
    st = lr;
  }
  return false;
}

void setup() {
  pinMode(LIGHT, OUTPUT);
  pinMode(FAN, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(KEY1, INPUT);
  pinMode(KEY2, INPUT);
  digitalWrite(LIGHT, LOW);
  digitalWrite(FAN, LOW);
  digitalWrite(LED, LOW);
}

void loop() {
  // LED = system power status (ON = powered, OFF = off)
  digitalWrite(LED, powerOn ? HIGH : LOW);

  if (isPress(KEY1, idle1, st1, lr1, tc1, ready1)) {
    powerOn = !powerOn;
    if (powerOn) {
      isLight = true;
      digitalWrite(LIGHT, HIGH);
      digitalWrite(FAN, LOW);
    } else {
      digitalWrite(LIGHT, LOW);
      digitalWrite(FAN, LOW);
    }
  }

  if (isPress(KEY2, idle2, st2, lr2, tc2, ready2) && powerOn) {
    isLight = !isLight;
    digitalWrite(LIGHT, isLight ? HIGH : LOW);
    digitalWrite(FAN, isLight ? LOW : HIGH);
  }
}