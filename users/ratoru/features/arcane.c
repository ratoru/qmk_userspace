#include "arcane.h"

// An enhanced version of SEND_STRING: if Caps Word is active, the Shift key is
// held while sending the string. Additionally, the last key is set such that if
// the Repeat Key is pressed next, it produces `repeat_keycode`. This helper is
// used for several macros below in my process_record_user() function.
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

void process_arcane(uint16_t keycode, uint8_t mods, bool is_idle) {
    // If no key has been pressed for a while, default to shift.
    if (is_idle) {
        add_oneshot_mods(MOD_BIT(KC_LSFT));
        return;
    }
    // Special keycodes
    bool cmd_held   = (mods & MOD_MASK_GUI);
    bool shift_held = (mods & MOD_MASK_SHIFT);
    if (cmd_held && shift_held) {
        switch (keycode) {
            case KC_Z:
                IS_MAC ? SEND_STRING(SS_LGUI("z")) : SEND_STRING(SS_LCTL("z")); // CMD + Shift + Z reverses to CMD + Z.
                return;
            case KC_T:
                IS_MAC ? SEND_STRING(SS_LGUI("w")) : SEND_STRING(SS_LCTL("w")); // CMD + Shift + T reverses to CMD + W.
                return;
        }
    }
    if (cmd_held) {
        switch (keycode) {
            case KC_Z:
                IS_MAC ? SEND_STRING(SS_LGUI(SS_LSFT("z"))) : SEND_STRING(SS_LCTL(SS_LSFT("z"))); // CMD + Z reverses to CMD + Shift + Z.
                return;
            case KC_W:
                IS_MAC ? SEND_STRING(SS_LGUI(SS_LSFT("t"))) : SEND_STRING(SS_LCTL(SS_LSFT("t"))); // CMD + W reverses to CMD + Shift + T.
                return;
        }
    }
    switch (keycode) {
        case KC_SPACE:
        case KC_ENT:
            add_oneshot_mods(MOD_BIT(KC_LSFT)); // Set one-shot mod for shift.
            break;
        case KC_BSPC:
            SEND_STRING(SS_LALT(SS_TAP(X_BSPC)));
            break;
        case KC_A:
            MAGIC_STRING(SS_TAP(X_BSPC) SS_LALT(SS_TAP(X_U)) SS_TAP(X_A), KC_A);
            break;
        case KC_B:
            MAGIC_STRING(/*b*/ "b", KC_B);
            break;
        case KC_C:
            MAGIC_STRING(/*c*/ "h", KC_H);
            break;
        case KC_D:
            if (!is_caps_word_on() && (mods & MOD_MASK_SHIFT)) {
                SEND_STRING(/*D*/ "anke");
            } else {
                MAGIC_STRING(/*t*/ "d", KC_D);
            }
            break;
        case KC_E:
            MAGIC_STRING(/*e*/ "u", KC_E);
            break;
        case KC_F:
            MAGIC_STRING(/*f*/ "f", KC_F);
            break;
        case KC_G:
            MAGIC_STRING(/*g*/ "g", KC_G);
            break;
        case KC_H:
            MAGIC_STRING(/*h*/ "y", KC_Y);
            break;
        case KC_I:
            MAGIC_STRING(/*i*/ "on", KC_N);
            break;
        case KC_J:
            MAGIC_STRING(/*j*/ "j", KC_J);
            break;
        case KC_K:
            MAGIC_STRING(/*k*/ "k", KC_K);
            break;
        case KC_L:
            MAGIC_STRING(/*l*/ "l", KC_L);
            break;
        case KC_M:
            MAGIC_STRING(/*m*/ "m", KC_M);
            break;
        case KC_N:
            MAGIC_STRING(/*n*/ "n", KC_N);
            break;
        case KC_O:
            MAGIC_STRING(SS_TAP(X_BSPC) SS_LALT(SS_TAP(X_U)) SS_TAP(X_O), KC_O);
            break;
        case KC_P:
            MAGIC_STRING(/*p*/ "h", KC_H);
            break;
        case KC_Q:
            MAGIC_STRING(/*q*/ "u", KC_U);
            break;
        case KC_R:
            MAGIC_STRING(/*r*/ "r", KC_R);
            break;
        case KC_S:
            MAGIC_STRING(/*s*/ "ch", KC_H);
            break;
        case KC_T:
            if (!is_caps_word_on() && (mods & MOD_MASK_SHIFT)) {
                // Checks for shift mod of previous key
                SEND_STRING(/*T*/ "hank");
            } else {
                MAGIC_STRING(/*t*/ "t", KC_T);
            }
            break;
        case KC_U:
            MAGIC_STRING(SS_TAP(X_BSPC) SS_LALT(SS_TAP(X_U)) SS_TAP(X_U), KC_U);
            break;
        case KC_V:
            MAGIC_STRING(/*v*/ "v", KC_V);
            break;
        case KC_W:
            MAGIC_STRING(/*w*/ "q", KC_W);
            break;
        case KC_X:
            SEND_STRING(SS_TAP(X_BSPC) SS_RALT(SS_TAP(X_S)));
            break;
        case KC_Y:
            MAGIC_STRING(/*y*/ "y", KC_Y);
            break;
        case KC_Z:
            MAGIC_STRING(/*z*/ "z", KC_Z);
            break;
    }
}
