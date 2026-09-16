#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "g_config.h"
#include "mt8816.h"

static const uint mt8816_control_pins[] = {
    MT8816_AX0,
    MT8816_AX1,
    MT8816_AX2,
    MT8816_AX3,
    MT8816_AY0,
    MT8816_AY1,
    MT8816_AY2,
    MT8816_DATA,
    MT8816_STROBE,
    MT8816_CS,
    MT8816_RESET,
};

static inline void mt8816_write_address(uint8_t y, uint8_t x)
{
    gpio_put(MT8816_AX0, (x >> 0) & 1u);
    gpio_put(MT8816_AX1, (x >> 1) & 1u);
    gpio_put(MT8816_AX2, (x >> 2) & 1u);
    gpio_put(MT8816_AX3, (x >> 3) & 1u);

    gpio_put(MT8816_AY0, (y >> 0) & 1u);
    gpio_put(MT8816_AY1, (y >> 1) & 1u);
    gpio_put(MT8816_AY2, (y >> 2) & 1u);
}

void mt8816_reset(void)
{
    // RESET is active HIGH.
    gpio_put(MT8816_STROBE, 0);
    gpio_put(MT8816_CS, 0);
    gpio_put(MT8816_RESET, 1);
    sleep_us(1);
    gpio_put(MT8816_RESET, 0);
    sleep_us(1);
    gpio_put(MT8816_CS, 1);
}

void mt8816_set_switch(uint8_t y, uint8_t x, bool state)
{
    if (y > 7 || x > 15)
        return;

    // MT8816 writes the addressed memory cell when
    // CS=HIGH and STROBE transitions HIGH -> LOW.
    mt8816_write_address(y, x);
    gpio_put(MT8816_DATA, state ? 1 : 0);

    gpio_put(MT8816_STROBE, 1);
    sleep_us(1);
    gpio_put(MT8816_STROBE, 0);
}

void mt8816_init(void)
{
    for (uint i = 0; i < sizeof(mt8816_control_pins) / sizeof(mt8816_control_pins[0]); i++)
    {
        gpio_init(mt8816_control_pins[i]);
        gpio_set_dir(mt8816_control_pins[i], GPIO_OUT);
        gpio_put(mt8816_control_pins[i], 0);
    }

    // Normal inactive states.
    gpio_put(MT8816_STROBE, 0);
    gpio_put(MT8816_CS, 0);
    gpio_put(MT8816_RESET, 0);
    gpio_put(MT8816_DATA, 0);

    mt8816_reset();
}
