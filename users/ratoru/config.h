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
