#include <Arduino.h>
#include <RadioLib.h>
#include "version.h"
#include "config.h"
#include "post.h"

SX1276 radio = new Module(18, 26, 23, 33);

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.printf("fw: %s+%s\n", FW_VERSION, FW_GIT_HASH);
  Serial.printf("built: %s\n", FW_BUILD_UTC);

  config_load();
  config_get();

  uint8_t post_result = post_run();
}

void loop()
{
  if (Serial.available())
  {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd == "config get")
    {
      config_get();
    }
    else if (cmd.startsWith("config set "))
    {
      String rest = cmd.substring(11);
      int space = rest.indexOf(' ');
      String key = rest.substring(0, space);
      String val = rest.substring(space + 1);
      config_set(key.c_str(), val.c_str());
    }
    else if (cmd == "config reset")
    {
      config_reset();
    }
    else if (cmd == "version")
    {
      Serial.printf("fw: %s+%s\n", FW_VERSION, FW_GIT_HASH);
      Serial.printf("built: %s\n", FW_BUILD_UTC);
    }
  }
}