#pragma once
#include <Arduino.h>

#define POST_BIT_POWER 0b00000001
#define POST_BIT_NVS 0b00000010
#define POST_BIT_ADC 0b00000100
#define POST_BIT_RADIO 0b00001000

uint8_t post_run();