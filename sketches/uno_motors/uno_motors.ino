// Arduino UNO R3 / ATmega328P. Wiring and commissioning: ../../README.md
#include <Arduino.h>
#include "MotorEnable.h"

const uint8_t ENABLE_BUTTON = 2;  // Joystick SW -> GND when pressed
const uint8_t STEERING_PIN = A0;
const uint8_t THROTTLE_PIN = A1;
const int CENTER_X = 512;         // Adjust from Serial Monitor, unloaded joystick
const int CENTER_Y = 512;
const int DEAD_ZONE = 60;
const bool INVERT_X = false;
const bool INVERT_Y = true;
const bool INVERT_LEFT = false;
const bool INVERT_RIGHT = false;
const int MAX_PWM = 100;          // Initial test ceiling; NOT a current limiter
const uint32_t TICK_MS = 20;
const uint32_t REVERSE_PAUSE_MS = 300;
const int RAMP_STEP = 4;

class Motor {
 public:
  Motor(uint8_t en, uint8_t in1, uint8_t in2, bool invert)
      : en_(en), in1_(in1), in2_(in2), invert_(invert) {}
  void begin() {
    digitalWrite(en_, LOW);
    pinMode(en_, OUTPUT);
    pinMode(in1_, OUTPUT);
    pinMode(in2_, OUTPUT);
    stop(millis());
  }
  void stop(uint32_t now) {
    if (pwm_ != 0) zeroSince_ = now;
    pwm_ = 0;
    analogWrite(en_, 0);
    digitalWrite(in1_, LOW);
    digitalWrite(in2_, LOW);
    // Keep last direction/time even across stop/re-arm.
  }
  void update(int target, uint32_t now) {
    target = constrain(target, -MAX_PWM, MAX_PWM);
    if (invert_) target = -target;
    const int direction = (target > 0) - (target < 0);
    if (!direction) { stop(now); return; }
    if (direction != lastDirection_ && lastDirection_ != 0) {
      stop(now);
      if (uint32_t(now - zeroSince_) < REVERSE_PAUSE_MS) return;
    }
    lastDirection_ = direction;
    const int wanted = abs(target);
    if (pwm_ < wanted) pwm_ = min(pwm_ + RAMP_STEP, wanted);
    else pwm_ = max(pwm_ - RAMP_STEP, wanted);
    digitalWrite(in1_, direction > 0 ? HIGH : LOW);
    digitalWrite(in2_, direction > 0 ? LOW : HIGH);
    analogWrite(en_, pwm_);
  }
 private:
  uint8_t en_, in1_, in2_;
  bool invert_;
  int pwm_ = 0;
  int lastDirection_ = 0;
  uint32_t zeroSince_ = 0;
};

Motor leftMotor(5, 7, 8, INVERT_LEFT);
Motor rightMotor(6, 9, 10, INVERT_RIGHT);
MotorEnable motorEnable;
uint32_t lastTick = 0;
uint32_t lastReport = 0;

int axisValue(int raw, int center, bool invert) {
  int value = 0;
  if (raw > center + DEAD_ZONE)
    value = (long)(raw - center - DEAD_ZONE) * 255 / (1023 - center - DEAD_ZONE);
  else if (raw < center - DEAD_ZONE)
    value = -(long)(center - DEAD_ZONE - raw) * 255 / (center - DEAD_ZONE);
  return invert ? -value : value;
}

void setup() {
  leftMotor.begin();
  rightMotor.begin();
  pinMode(ENABLE_BUTTON, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.println(F("Center + click SW: enable. Next click: latched STOP."));
}

void loop() {
  const uint32_t now = millis();
  const bool pressed = digitalRead(ENABLE_BUTTON) == LOW;
  const int rawX = analogRead(STEERING_PIN);
  const int rawY = analogRead(THROTTLE_PIN);
  const int steering = axisValue(rawX, CENTER_X, INVERT_X);
  const int throttle = axisValue(rawY, CENTER_Y, INVERT_Y);
  const bool armed = motorEnable.update(pressed, steering == 0 && throttle == 0, now);
  if (!armed) {
    leftMotor.stop(now);
    rightMotor.stop(now);
    digitalWrite(LED_BUILTIN, LOW);
  }
  if (uint32_t(now - lastTick) < TICK_MS) return;
  lastTick = now;
  if (armed) {
    int left = throttle + steering;
    int right = throttle - steering;
    // Normalize both together to preserve the steering ratio on diagonals.
    const int scale = max(255, max(abs(left), abs(right)));
    leftMotor.update((long)left * MAX_PWM / scale, now);
    rightMotor.update((long)right * MAX_PWM / scale, now);
  } else {
    leftMotor.stop(now);
    rightMotor.stop(now);
  }
  digitalWrite(LED_BUILTIN, armed ? HIGH : LOW);
  if (uint32_t(now - lastReport) >= 500) {
    lastReport = now;
    Serial.print(F("X=")); Serial.print(rawX);
    Serial.print(F(" Y=")); Serial.print(rawY);
    Serial.print(F(" armed=")); Serial.println(armed);
  }
}
