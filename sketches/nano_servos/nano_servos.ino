// Classic Arduino Nano, ATmega328P, 5 V. Not Nano ESP32 / Nano 33.
#include <Arduino.h>
#include <Servo.h>

const uint8_t CENTER_BUTTON = 2;
const int CENTER_X = 512;
const int CENTER_Y = 512;
const int DEAD_ZONE = 40;
const bool INVERT_X = false;
const bool INVERT_Y = false;
const int MIN_US = 1200;  // Conservative initial range; tune for actual servo/linkage
const int CENTER_US = 1500;
const int MAX_US = 1800;
const int STEP_US = 10;
const uint32_t TICK_MS = 20;
Servo rudder;
Servo auxiliary;
int rudderUs = CENTER_US;
int auxiliaryUs = CENTER_US;
uint32_t lastTick = 0;
uint32_t lastReport = 0;

int axisPulse(int raw, int center, bool invert) {
  long offset = 0;
  if (raw > center + DEAD_ZONE)
    offset = (long)(raw - center - DEAD_ZONE) * 1000 / (1023 - center - DEAD_ZONE);
  else if (raw < center - DEAD_ZONE)
    offset = -(long)(center - DEAD_ZONE - raw) * 1000 / (center - DEAD_ZONE);
  if (invert) offset = -offset;
  return CENTER_US + (offset >= 0 ? offset * (MAX_US - CENTER_US) / 1000
                                : offset * (CENTER_US - MIN_US) / 1000);
}

int approach(int current, int target) {
  if (current < target) return min(current + STEP_US, target);
  return max(current - STEP_US, target);
}

void setup() {
  pinMode(CENTER_BUTTON, INPUT_PULLUP);
  Serial.begin(115200);
  rudder.attach(3, MIN_US, MAX_US);
  auxiliary.attach(4, MIN_US, MAX_US);
  rudder.writeMicroseconds(CENTER_US);
  auxiliary.writeMicroseconds(CENTER_US);
}

void loop() {
  const uint32_t now = millis();
  if (uint32_t(now - lastTick) < TICK_MS) return;
  lastTick = now;
  const int rawX = analogRead(A0);
  const int rawY = analogRead(A1);
  const bool center = digitalRead(CENTER_BUTTON) == LOW;
  rudderUs = approach(rudderUs, center ? CENTER_US : axisPulse(rawX, CENTER_X, INVERT_X));
  auxiliaryUs = approach(auxiliaryUs, center ? CENTER_US : axisPulse(rawY, CENTER_Y, INVERT_Y));
  rudder.writeMicroseconds(rudderUs);
  auxiliary.writeMicroseconds(auxiliaryUs);
  if (uint32_t(now - lastReport) >= 500) {
    lastReport = now;
    Serial.print(F("X=")); Serial.print(rawX);
    Serial.print(F(" Y=")); Serial.print(rawY);
    Serial.print(F(" rudder_us=")); Serial.print(rudderUs);
    Serial.print(F(" auxiliary_us=")); Serial.println(auxiliaryUs);
  }
}
