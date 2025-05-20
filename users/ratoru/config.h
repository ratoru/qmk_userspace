#pragma once

// Prevent recursive dynamic macros that make the keyboard unresponsive.
#ifdef DYNAMIC_MARCO_ENABLE
#define DYNAMIC_MACRO_NO_NESTING
#endif

// ---- Home row mods settings ----
#ifdef HRM_ENABLE
// Configure the global tapping term (default: 200ms, rec: 170-220ms)
#define TAPPING_TERM 220
#define TAPPING_TERM_PER_KEY
// If you press a dual-role key, tap another key (press and release) and then release the dual-role key,
// the dual-role key will perform its hold action instead.
#define PERMISSIVE_HOLD
#define QUICK_TAP_TERM_PER_KEY
#endif

#ifdef RGBLIGHT_ENABLE
#define WS2812_PIO_USE_PIO1 // Force the usage of PIO1 peripheral, by default the WS2812 implementation uses the PIO0 peripheral
#define RGB_SLEEP
#define RGBLIGHT_DISABLE_KEYCODES
#define RGBLIGHT_LIMIT_VAL 128
#endif

// General features
#define CAPS_WORD_INVERT_ON_SHIFT

// Reduze size of firmware
#define LAYER_STATE_8BIT // Limit the number of layers to 8 to save firmware size.
#define NO_MUSIC_MODE
#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION
#undef LOCKING_SUPPORT_ENABLE
#undef LOCKING_RESYNC_ENABLE

#ifndef __ASSEMBLER__
// Define all of custom keycodes. Needs to be protected.
#include "quantum/keycodes.h"
#define SRCHSEL QK_USER
#define UPDIR QK_USER + 1
#define SW_TAB QK_USER + 2
#define SW_CTAB QK_USER + 3
#define SW_BTICK QK_USER + 4
#define RGBT_NE QK_USER + 5
#define RGB_DB_NE QK_USER + 6
#define RGB_IB_NE QK_USER + 7
#define OS_LSFT QK_USER + 8
#define OS_LCTL QK_USER + 9
#define OS_LALT QK_USER + 10
#define OS_LGUI QK_USER + 11
#define OS_HYPR QK_USER + 12
#define S_END QK_USER + 13
#define CPY_URL QK_USER + 14
#endif
