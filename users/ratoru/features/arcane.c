#include "arcane.h"

// An enhanced version of SEND_STRING: if Caps Word is active, the Shift key is
// held while sending the string. Additionally, the last key is set such that if
// the Repeat Key is pressed next, it produces `repeat_keycode`. This helper is
// used below.
#define MAGIC_STRING(str, repeat_keycode) magic_send_string_P(PSTR(str), (repeat_keycode))
static void magic_send_string_P(const char* str, uint16_t repeat_keycode) {
    uint8_t saved_mods = 0;
    // If Caps Word is on, save the mods and hold Shift.
    if (is_caps_word_on()) {
        saved_mods = get_mods();
        register_mods(MOD_BIT_LSHIFT);
    }

    send_string_P(str); // Send the string.
    set_last_keycode(repeat_keycode);

    // If Caps Word is on, restore the mods.
    if (is_caps_word_on()) {
        set_mods(saved_mods);
    }
}

bool process_arcane(uint16_t keycode, uint8_t mods, bool is_idle) {
    // If no key has been pressed for a while, default to shift.
    if (is_idle) {
        add_oneshot_mods(MOD_BIT(KC_LSFT));
        return true;
    }
    // Special keycodes
    bool cmd_held   = (mods & MOD_MASK_GUI);
    bool shift_held = (mods & MOD_MASK_SHIFT);
    if (cmd_held && shift_held) {
        switch (keycode) {
            case KC_Z:
                IS_MAC ? SEND_STRING(SS_LGUI("z")) : SEND_STRING(SS_LCTL("z")); // CMD + Shift + Z reverses to CMD + Z.
                return true;
            case KC_T:
                IS_MAC ? SEND_STRING(SS_LGUI("w")) : SEND_STRING(SS_LCTL("w")); // CMD + Shift + T reverses to CMD + W.
                return true;
        }
    }
    if (cmd_held) {
        switch (keycode) {
            case KC_Z:
                IS_MAC ? SEND_STRING(SS_LGUI(SS_LSFT("z"))) : SEND_STRING(SS_LCTL(SS_LSFT("z"))); // CMD + Z reverses to CMD + Shift + Z.
                return true;
            case KC_W:
                IS_MAC ? SEND_STRING(SS_LGUI(SS_LSFT("t"))) : SEND_STRING(SS_LCTL(SS_LSFT("t"))); // CMD + W reverses to CMD + Shift + T.
                return true;
        }
    }
    switch (keycode) {
        case KC_SPACE:
        case KC_ENT:
        case KC_BSPC:
            // Set one-shot mod for shift.
            add_oneshot_mods(MOD_BIT(KC_LSFT));
            return true;
        case KC_DOT:
            MAGIC_STRING(/*.*/ "./", UPDIR);
            return true;
        case KC_GRAVE:
            MAGIC_STRING(/*`*/ "``", KC_GRAVE);
            return true;
        case KC_A:
            MAGIC_STRING(SS_TAP(X_BSPC) SS_LALT(SS_TAP(X_U)) SS_TAP(X_A), KC_A);
            return true;
        case KC_C:
            MAGIC_STRING(/*c*/ "h", KC_H);
            return true;
        case KC_D:
            if (!is_caps_word_on() && shift_held) {
                SEND_STRING(/*D*/ "anke");
            } else {
                MAGIC_STRING(/*d*/ "d", KC_D);
            }
            return true;
        case KC_E:
            MAGIC_STRING(/*e*/ "u", KC_U);
            return true;
        case KC_H:
            MAGIC_STRING(/*h*/ "y", KC_Y);
            return true;
        case KC_I:
            MAGIC_STRING(/*i*/ "on", KC_N);
            return true;
        case KC_O:
            MAGIC_STRING(SS_TAP(X_BSPC) SS_LALT(SS_TAP(X_U)) SS_TAP(X_O), KC_O);
            return true;
        case KC_P:
            MAGIC_STRING(/*p*/ "h", KC_H);
            return true;
        case KC_Q:
            MAGIC_STRING(/*q*/ "u", KC_U);
            return true;
        case KC_S:
            MAGIC_STRING(/*s*/ "ch", KC_H);
            return true;
        case KC_T:
            if (!is_caps_word_on() && shift_held) {
                // Checks for shift mod of previous key
                SEND_STRING(/*T*/ "hank");
                return true;
            }
            break;
        case KC_U:
            MAGIC_STRING(SS_TAP(X_BSPC) SS_LALT(SS_TAP(X_U)) SS_TAP(X_U), KC_U);
            return true;
        case KC_W:
            MAGIC_STRING(/*w*/ "q", KC_W);
            return true;
        case KC_X:
            SEND_STRING(SS_TAP(X_BSPC) SS_RALT(SS_TAP(X_S)));
            return true;
    }
    return false;
}
