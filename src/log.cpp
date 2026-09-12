#include "log.h"
#include <Arduino.h>
#include <cstring>

Record record[10];
static unsigned int currentIdx = 0;

void log_write(Status inLevel, const char *message)
{
    record[currentIdx].level = inLevel;

    strncpy(record[currentIdx].message, message, 63);
    record[currentIdx].message[63] = '\0';

    record[currentIdx].timestamp = millis();

    currentIdx = (currentIdx + 1) % 10;
}

void log_dump()
{
    for (int i = 0; i < 10; i++)
    {
        Serial.printf("Index: %i\n", i);
        Serial.printf("Level: %i\n", record[i].level);
        Serial.printf("Message: %s\n", record[i].message);
        Serial.printf("Timestamp: %lu\n", record[i].timestamp);
    }
}