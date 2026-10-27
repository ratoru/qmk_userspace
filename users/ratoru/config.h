#pragma once

// Prevent recursive dynamic macros that make the keyboard unresponsive.
#ifdef DYNAMIC_MACRO_ENABLE
#    define DYNAMIC_MACRO_NO_NESTING
#endif

#ifdef RGBLIGHT_ENABLE
#    define WS2812_PIO_USE_PIO1 // Force the usage of PIO1 peripheral, by default the WS2812 implementation uses the PIO0 peripheral
#    define RGBLIGHT_SLEEP
#    define RGBLIGHT_LIMIT_VAL 128
#endif

// Cyclotab: handle Cmd+Tab and Cmd+Grave for Mac app/window switching.
#define CYCLOTAB_KEYS LCMD(KC_TAB), LCMD(KC_GRAVE), LCTL(KC_TAB)
#define CYCLOTAB_TIMEOUT 0 // Disable timeout.

// Home row mods. Keymaps can #undef and redefine the terms.
#define TAPPING_TERM 220
#define FLOW_TAP_TERM 100
#define CHORDAL_HOLD
#define PERMISSIVE_HOLD

// General features
#define CAPS_WORD_INVERT_ON_SHIFT
#define DOUBLE_TAP_SHIFT_TURNS_ON_CAPS_WORD // Left shift or one-shot left shift only.
#define LAYER_LOCK_IDLE_TIMEOUT 60000       // Turn off layer lock after 60 seconds of idle time.
#define IDLE_TIMEOUT_MS 5000                // Idle timeout in milliseconds.

// Unfortunately, some applications drop or misorder fast key events. This is a
// partial fix to slow down the rate at which macros are sent.
#define TAP_CODE_DELAY 5

// Reduce size of firmware
#define LAYER_STATE_8BIT // Limit the number of layers to 8 to save firmware size.
#define NO_MUSIC_MODE
#undef LOCKING_SUPPORT_ENABLE
#undef LOCKING_RESYNC_ENABLE
