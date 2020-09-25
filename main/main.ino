#include "sensor.h"

bool ok = false;
void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  Serial.begin(115200);
  Serial.println("Start");

  Serial.println();

  ok = init_sensors();
}

void loop() {
  Main();
}
