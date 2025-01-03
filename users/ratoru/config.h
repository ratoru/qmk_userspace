#pragma once

// Prevent recursive dynamic macros that make the keyboard unresponsive.
#ifdef DYNAMIC_MARCO_ENABLE
#define DYNAMIC_MACRO_NO_NESTING
#endif

// ---- Home row mods settings ----
// Configure the global tapping term (default: 200ms)
#ifdef THRM_ENABLE
#define TAPPING_TERM 500
// Enable rapid switch from tap to hold, disables double tap hold auto-repeat.
#define QUICK_TAP_TERM 0
// "Timeless" config options
#define TAP_INTERVAL_MS 100 // Instant tap setting
// Bilateral hold options
#define PERMISSIVE_HOLD
#define PERMISSIVE_HOLD_PER_KEY
#define HOLD_ON_OTHER_KEY_PRESS
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY
#endif

#ifdef RGBLIGHT_ENABLE
#define WS2812_PIO_USE_PIO1 // Force the usage of PIO1 peripheral, by default the WS2812 implementation uses the PIO0 peripheral
#define RGB_SLEEP
#define RGBLIGHT_DISABLE_KEYCODES
#define RGBLIGHT_LIMIT_VAL 128
#endif

// Reduze size of firmware
#define LAYER_STATE_8BIT // Limit the number of layers to 8 to save firmware size.
#define NO_MUSIC_MODE
#undef LOCKING_SUPPORT_ENABLE
#undef LOCKING_RESYNC_ENABLE

#ifndef __ASSEMBLER__
// Define all of custom keycodes. Needs to be protected.
#include "quantum/keycodes.h"
#define SELWORD QK_USER
#define UPDIR QK_USER + 1
#define SW_TAB QK_USER + 2
#define SW_CTAB QK_USER + 3
#define SW_BTICK QK_USER + 4
#define RGBT_NE QK_USER + 5
#define RGB_DB_NE QK_USER + 6
#define RGB_IB_NE QK_USER + 7
#define OS_SHFT QK_USER + 8
#define OS_CTRL QK_USER + 9
#define OS_ALT QK_USER + 10
#define OS_CMD QK_USER + 11
#define OS_HYPR QK_USER + 12
#define S_END QK_USER + 13
#define CPY_URL QK_USER + 14
#define SRCHSEL QK_USER + 15
#endif
