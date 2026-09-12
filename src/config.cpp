#include "config.h"
#include "log.h"
#include <Preferences.h>

Preferences prefs;

const Config DEFAULT_CONFIG = {
    .cfg_version = CFG_VERSION,
    .tx_interval = 1000,
    .log_level = INFO};

Config current_config;

static void migrate_v1_to_v2(Config &cfg)
{
    cfg.log_level = DEFAULT_CONFIG.log_level;
    cfg.cfg_version = CFG_VERSION;
    log_write(INFO, "Config migrated v1->v2");
}

void config_load()
{
    prefs.begin("config", true);

    if (!prefs.isKey("cfg_version"))
    {
        prefs.end();
        current_config = DEFAULT_CONFIG;
        log_write(INFO, "Config: NVS empty, using defaults");
        return;
    }

    current_config.cfg_version = prefs.getUChar("cfg_version", CFG_VERSION);
    current_config.tx_interval = prefs.getInt("tx_interval", DEFAULT_CONFIG.tx_interval);
    current_config.log_level = prefs.getUChar("log_level", DEFAULT_CONFIG.log_level);
    prefs.end();

    if (current_config.cfg_version == 1)
    {
        migrate_v1_to_v2(current_config);
        prefs.begin("config", false);
        prefs.putUChar("cfg_version", current_config.cfg_version);
        prefs.putUChar("log_level", current_config.log_level);
        prefs.end();
    }

    log_write(INFO, "Config loaded from NVS");
}

void config_get()
{
    Serial.printf("cfg_version: %d\n", current_config.cfg_version);
    Serial.printf("tx_interval: %d ms\n", current_config.tx_interval);
    Serial.printf("log_level:   %d\n", current_config.log_level);
}

bool config_set(const char *key, const char *value)
{
    if (strcmp(key, "tx_interval") == 0)
    {
        int val = atoi(value);

        if (val < 100 || val > 60000)
        {
            Serial.println("Error: tx_interval must be 100..60000");
            return false;
        }
        current_config.tx_interval = val;
        prefs.begin("config", false);
        prefs.putInt("tx_interval", val);
        prefs.end();
    }
    else if (strcmp(key, "log_level") == 0)
    {
        int val = atoi(value);

        if (val < DEBUG || val > ERROR)
        {
            Serial.println("Error: log_level must be 0..3");
            return false;
        }
        current_config.log_level = val;
        prefs.begin("config", false);
        prefs.putUChar("log_level", val);
        prefs.end();
    }
    else
    {
        Serial.printf("Error: unknown key '%s'\n", key);
        return false;
    }

    log_write(INFO, "Config updated");
    return true;
}

void config_reset()
{
    current_config = DEFAULT_CONFIG;
    prefs.begin("config", false);
    prefs.putUChar("cfg_version", current_config.cfg_version);
    prefs.putInt("tx_interval", current_config.tx_interval);
    prefs.putUChar("log_level", current_config.log_level);
    prefs.end();
    log_write(INFO, "Config reset to defaults");
    Serial.println("Config reset to defaults");
}