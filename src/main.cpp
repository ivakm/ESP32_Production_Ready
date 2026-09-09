#include <Arduino.h>
#include "version.h"

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.printf("fw: %s+%s\n", FW_VERSION, FW_GIT_HASH);
  Serial.printf("built: %s\n", FW_BUILD_UTC);
}

void loop() {}