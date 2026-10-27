#include "arcane.h"
#include "is_mac.h"
#include "magic_string.h"
#include "ratoru_keycodes.h"

// Replaces the vowel just typed with its umlaut, using the Option+U dead key of
// the macOS US layout. Pressing Repeat afterwards types the plain vowel.
static void send_umlaut(uint16_t vowel, bool shifted) {
    const uint8_t saved_mods = get_mods();
    clear_mods();
    tap_code(KC_BSPC);
    tap_code16(A(KC_U));
    tap_code16(shifted || is_caps_word_on() ? S(vowel) : vowel);
    set_mods(saved_mods);
    set_last_keycode(vowel);
}

// Reverses the last shortcut. Returns false for shortcuts that should repeat.
static bool reverse_shortcut(uint16_t keycode, bool shifted) {
    switch (keycode) {
        case KC_Z: // Undo <-> redo.
            tap_code16(shifted ? SHORTCUT(KC_Z) : SHORTCUT(S(KC_Z)));
            return true;
        case KC_W: // Close tab or window -> reopen it.
            tap_code16(SHORTCUT(S(KC_T)));
            return true;
        case KC_T: // Reopen tab -> close it.
            if (shifted) {
                tap_code16(SHORTCUT(KC_W));
                return true;
            }
            break;
    }
    return false;
}

bool process_arcane(uint16_t keycode, uint8_t mods, bool is_idle) {
    // After a pause, assume a new sentence starts.
    if (is_idle) {
        add_oneshot_mods(MOD_BIT_LSHIFT);
        return true;
    }

    const bool shifted = mods & MOD_MASK_SHIFT;
    if (mods & SHORTCUT_MOD_MASK) {
        return reverse_shortcut(keycode, shifted);
    }
    if (mods & ~MOD_MASK_SHIFT) {
        return false; // Repeat other shortcuts, like Ctrl+C.
    }

    switch (keycode) {
        case KC_SPACE:
        case KC_ENT:
        case KC_BSPC:
            add_oneshot_mods(MOD_BIT_LSHIFT); // Capitalize the next word.
            return true;

        // German
        case KC_A:
        case KC_O:
        case KC_U:
            send_umlaut(keycode, shifted);
            return true;
        case KC_X: // x -> ß
            tap_code(KC_BSPC);
            tap_code16(RALT(KC_S));
            return true;
        case KC_S:
            MAGIC_STRING(/*s*/ "ch", KC_H);
            return true;
        case KC_D:
            if (shifted && !is_caps_word_on()) {
                SEND_STRING_DELAY(/*D*/ "anke", TAP_CODE_DELAY);
            } else {
                MAGIC_STRING(/*d*/ "d", KC_D);
            }
            return true;

        // English
        case KC_T:
            if (shifted && !is_caps_word_on()) {
                SEND_STRING_DELAY(/*T*/ "hank", TAP_CODE_DELAY);
                return true;
            }
            return false;
        case KC_I:
            MAGIC_STRING(/*i*/ "on", KC_N);
            return true;

        // Other letters
        case KC_C:
            MAGIC_STRING(/*c*/ "h", KC_H);
            return true;
        case KC_P:
            MAGIC_STRING(/*p*/ "h", KC_H);
            return true;
        case KC_E:
            MAGIC_STRING(/*e*/ "u", KC_U);
            return true;
        case KC_H:
            MAGIC_STRING(/*h*/ "y", KC_Y);
            return true;
        case KC_Q:
            MAGIC_STRING(/*q*/ "u", KC_U);
            return true;
        case KC_W:
            MAGIC_STRING(/*w*/ "q", KC_W);
            return true;

        // Code
        case KC_DOT:
            MAGIC_STRING(/*.*/ "./", UPDIR);
            return true;
        case KC_GRAVE:
            MAGIC_STRING(/*`*/ "``", KC_GRAVE);
            return true;
    }
    return false;
}
