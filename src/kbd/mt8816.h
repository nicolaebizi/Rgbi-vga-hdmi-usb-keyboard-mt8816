#pragma once

#include <stdbool.h>
#include <stdint.h>

void mt8816_init(void);
void mt8816_set_switch(uint8_t y, uint8_t x, bool state);
void mt8816_reset(void);
