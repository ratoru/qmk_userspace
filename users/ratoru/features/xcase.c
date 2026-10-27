#include "xcase.h"
#include "ratoru_keycodes.h"

// Press XCASE, then a separator key, then type words separated by spaces:
//   XCASE _     hello world -> hello_world
//   XCASE -     hello world -> hello-world
//   XCASE Shift hello world -> helloWorld
// Combine it with Caps Word for SCREAMING_SNAKE_CASE. Any key that is not a
// letter, digit, backspace or the separator ends xcase, and so does a double
// space, which leaves a plain space.

// Stands in for the separator in camelCase, where space shifts the next letter.
#define CAMEL_CASE KC_NO

typedef enum {
    XCASE_OFF,
    XCASE_WAIT, // Waiting for the separator key.
    XCASE_ON,
} xcase_state_t;

static xcase_state_t state          = XCASE_OFF;
static uint16_t      separator      = CAMEL_CASE;
static bool          last_was_space = false;

// Returns the basic keycode of `keycode` if it has at most Shift applied, and
// KC_NO otherwise.
static uint8_t unshifted(uint16_t keycode) {
    if (IS_QK_MODS(keycode)) {
        if (QK_MODS_GET_MODS(keycode) & ~MOD_RSFT) {
            return KC_NO;
        }
        return QK_MODS_GET_BASIC_KEYCODE(keycode);
    }
    return IS_QK_BASIC(keycode) ? keycode : KC_NO;
}

static bool is_shift_key(uint16_t keycode) {
    if (IS_QK_ONE_SHOT_MOD(keycode)) {
        const uint8_t mods = QK_ONE_SHOT_MOD_GET_MODS(keycode);
        return (mods & MOD_LSFT) && !(mods & ~MOD_RSFT);
    }
    return keycode == KC_LSFT || keycode == KC_RSFT;
}

// Keys that only change modifiers or layers, so they neither pick a separator
// nor end xcase. This lets you reach the separator on another layer.
static bool is_mod_or_layer_key(uint16_t keycode, keyrecord_t *record) {
    if ((IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode)) && !record->tap.count) {
        return true; // Held tap-hold key.
    }
    return IS_MODIFIER_KEYCODE(keycode) || IS_QK_ONE_SHOT_MOD(keycode) || IS_QK_MOMENTARY(keycode) || IS_QK_ONE_SHOT_LAYER(keycode) || IS_QK_LAYER_TAP_TOGGLE(keycode) || keycode == QK_LAYER_LOCK || keycode == QK_CAPS_WORD_TOGGLE;
}

static bool choose_separator(uint16_t keycode, keyrecord_t *record) {
    if (is_shift_key(keycode)) {
        separator = CAMEL_CASE;
    } else if (is_mod_or_layer_key(keycode, record)) {
        return true;
    } else {
        keycode             = get_tap_keycode(keycode);
        const uint8_t basic = unshifted(keycode);
        if (basic < KC_MINUS || basic > KC_SLASH) {
            state = XCASE_OFF; // Not a punctuation key, so cancel.
            return true;
        }
        separator = keycode;
    }
    state          = XCASE_ON;
    last_was_space = false;
    return false;
}

bool process_xcase(uint16_t keycode, keyrecord_t *record) {
    if (keycode == XCASE) {
        if (record->event.pressed) {
            state = state == XCASE_OFF ? XCASE_WAIT : XCASE_OFF;
        }
        return false;
    }
    if (state == XCASE_OFF || !record->event.pressed) {
        return true;
    }
    if (state == XCASE_WAIT) {
        return choose_separator(keycode, record);
    }

    if (is_mod_or_layer_key(keycode, record)) {
        return true;
    }
    if ((get_mods() | get_oneshot_mods()) & ~MOD_MASK_SHIFT) {
        state = XCASE_OFF; // A shortcut ends the word.
        return true;
    }

    keycode = get_tap_keycode(keycode);
    if (keycode == KC_SPACE) {
        if (last_was_space) {
            // Double space: undo the separator and type a real space.
            if (separator == CAMEL_CASE) {
                del_oneshot_mods(MOD_BIT_LSHIFT);
            } else {
                tap_code(KC_BSPC);
            }
            state = XCASE_OFF;
            return true;
        }
        if (separator == CAMEL_CASE) {
            add_oneshot_mods(MOD_BIT_LSHIFT);
        } else {
            tap_code16(separator);
            set_last_keycode(separator);
        }
        last_was_space = true;
        return false;
    }

    last_was_space = false;
    if (keycode == separator) {
        return true;
    }
    switch (unshifted(keycode)) {
        case KC_A ... KC_Z:
        case KC_1 ... KC_0:
        case KC_BSPC:
            return true;
    }
    state = XCASE_OFF;
    return true;
}
