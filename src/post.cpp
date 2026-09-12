#include "post.h"
#include "log.h"
#include <Preferences.h>
#include <RadioLib.h>

extern SX1276 radio;

#define POST_ADC_PIN 34
#define POST_POWER_MIN_RAW 100

static uint8_t check_power()
{
    int raw = analogRead(POST_ADC_PIN);

    if (raw < POST_POWER_MIN_RAW)
    {
        log_write(ERROR, "POST: Power check FAILED");
        return POST_BIT_POWER;
    }

    log_write(INFO, "POST: Power OK");
    return 0;
}

static uint8_t check_nvs()
{
    Preferences prefs;

    if (!prefs.begin("post_check", false))
    {
        log_write(ERROR, "POST: NVS check FAILED");
        return POST_BIT_NVS;
    }

    prefs.putUChar("test", 42);
    uint8_t val = prefs.getUChar("test", 0);
    prefs.end();

    if (val != 42)
    {
        log_write(ERROR, "POST: NVS read/write FAILED");
        return POST_BIT_NVS;
    }

    log_write(INFO, "POST: NVS OK");
    return 0;
}

static uint8_t check_adc()
{
    int val1 = analogRead(POST_ADC_PIN);
    delay(10);
    int val2 = analogRead(POST_ADC_PIN);

    if (val1 == 0 && val2 == 0)
    {
        log_write(ERROR, "POST: ADC check FAILED");
        return POST_BIT_ADC;
    }

    log_write(INFO, "POST: ADC OK");
    return 0;
}

static uint8_t check_radio()
{
    int16_t state = radio.sleep();
    delay(10);

    ConfigLoRa_t config;
    config.frequency = 868;
    state = radio.begin(config);

    if (state != RADIOLIB_ERR_NONE)
    {
        log_write(ERROR, "POST: Radio check FAILED");
        return POST_BIT_RADIO;
    }

    log_write(INFO, "POST: Radio OK");
    return 0;
}

uint8_t post_run()
{
    uint8_t result = 0;

    result |= check_power();
    result |= check_nvs();
    result |= check_adc();
    result |= check_radio();

    if (result == 0)
    {
        log_write(INFO, "POST: All checks PASSED");
    }
    else
    {
        log_write(WARN, "POST: Some checks FAILED");
    }

    Serial.printf("POST result: 0x%02X\n", result);
    return result;
}