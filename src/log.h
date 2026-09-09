#pragma once

enum Status
{
    DEBUG,
    INFO,
    WARN,
    ERROR
};

struct Record
{
    Status level;
    char message[64];
    unsigned long timestamp;
};

void log_write(Status inLevel, const char *message);
void log_dump();