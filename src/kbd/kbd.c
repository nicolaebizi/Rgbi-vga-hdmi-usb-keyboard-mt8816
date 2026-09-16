/**
 * kbd.c - Universal keyboard dispatcher
 *
 * Orchestrates USB keyboard input and MT8816 ZX Spectrum keyboard output
 * through a unified keyboard state pipeline.
 */

#include "g_config.h"
#include "kbd.h"
#include "key_codes.h"
#include "led.h"
#include "zx_kbd.h"

// Hotkey for mouse button swap (override in g_config.h if needed)
#ifndef KBD_HOTKEY_MOUSE_SWAP
#define KBD_HOTKEY_MOUSE_SWAP KEY_F6
#endif
// Hotkeys for NMI and RESET signals (override in g_config.h if needed)
#ifndef KBD_HOTKEY_NMI
#define KBD_HOTKEY_NMI KEY_F11
#endif
#ifndef KBD_HOTKEY_RESET
#define KBD_HOTKEY_RESET KEY_F12
#endif

#include "mt8816.h"

#ifdef PS2_KBD_ENABLE
#include "ps2_kbd.h"
#endif

#ifdef USB_KBD_ENABLE
#include "usb_kbd.h"
#endif

#ifdef OSD_ENABLE
#include "osd_kbd.h"
#else
// When OSD is disabled, keyboard output is never suppressed
#define osd_kbd_active false
#endif


#ifdef KBD_ENABLE

extern settings_t settings;

volatile uint32_t kbd_activity_cnt;

static kbd_unified_state_t kbd_state;
static zx_kbd_state_t kbd_zx_state;
static zx_kbd_state_t kbd_zx_state_old;

static void __not_in_flash_func(kbd_apply_output)(zx_kbd_state_t *zx_new, zx_kbd_state_t *zx_old)
{
    for (int row = 0; row < 8; row++)
    {
        uint8_t changed = zx_new->a[row] ^ zx_old->a[row];
        if (changed)
        {
            for (int col = 0; col < 5; col++)
            {
                if (changed & (1u << col))
                {
                    bool pressed = (zx_new->a[row] & (1u << col)) != 0;
                    mt8816_set_switch(row, col, pressed);
                }
            }
        }
    }
}

static void __not_in_flash_func(kbd_on_event)(void)
{
    kbd_activity_cnt++;
    led_put(LED_B, (kbd_activity_cnt & 1) ? 32 : 0);

    kbd_state_t merged;

    memset(&merged, 0, sizeof(merged));

#ifdef PS2_KBD_ENABLE
    kbd_state_t *ps2 = ps2_kbd_get_state();

    for (int i = 0; i < 4; i++)
        merged.u[i] |= ps2->u[i];
#endif

#ifdef USB_KBD_ENABLE
    kbd_state_t *usb = usb_kbd_get_state();

    for (int i = 0; i < 4; i++)
        merged.u[i] |= usb->u[i];
#endif

    kbd_state.old_state = kbd_state.new_state;
    kbd_state.new_state = merged;


#ifdef OSD_ENABLE
    osd_kbd_intercept(&kbd_state.new_state);
#endif

    if (!osd_kbd_active)
    {
        zx_kbd_set_state(&kbd_zx_state, &kbd_state.new_state);
        kbd_apply_output(&kbd_zx_state, &kbd_zx_state_old);
        kbd_zx_state_old = kbd_zx_state;
    }
}

void kbd_init(void)
{
    memset(&kbd_state, 0, sizeof(kbd_state));
    memset(&kbd_zx_state, 0, sizeof(kbd_zx_state));
    memset(&kbd_zx_state_old, 0, sizeof(kbd_zx_state_old));

    mt8816_init();

#ifdef PS2_KBD_ENABLE
    ps2_kbd_set_event_callback(kbd_on_event);
    ps2_kbd_pio_init();
#endif

#ifdef USB_KBD_ENABLE
    usb_kbd_init();
    usb_kbd_set_event_callback(kbd_on_event);
#endif
}

#endif // KBD_ENABLE
