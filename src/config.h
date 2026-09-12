#pragma once
#include <Arduino.h>

#define CFG_VERSION 2

struct Config
{
    uint8_t cfg_version;
    int tx_interval;
    uint8_t log_level;
};

extern const Config DEFAULT_CONFIG;
extern Config current_config;

void config_load();
void config_get();

bool config_set(const char *key, const char *value);
void config_reset();
