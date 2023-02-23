#define SWITCH_PERIOD 50  //on/off period in ms
#define DUTY_CYCLE 127
#define OUT_PIN_A 0
#define OUT_PIN_B 1

void setup() {
  pinMode(OUT_PIN_A, OUTPUT);
  pinMode(OUT_PIN_B, OUTPUT);
}

void loop() {
  analogWrite(OUTPIN_A, DUTY_CYCLE);
  delay(SWITCH_PERIOD);
  analogWrite(OUTPIN_B, DUTY_CYCLE);
  delay(SWITCH_PERIOD);
}
