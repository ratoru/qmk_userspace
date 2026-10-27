#include QMK_KEYBOARD_H
#include "ratoru.h"
#include "features/magic_string.h"

// Keycodes for the magic key sequences, typed in process_record_keymap().
enum sweep_magic_keycodes {
    M_THE = NEW_SAFE_RANGE,
    M_ION,
    M_MENT,
    M_QUEN,
    M_TMENT,
    M_UPDIR,
    M_NOOP,
    M_INCLUDE,
    M_NBSP,
    M_EQEQ,
    M_DOCSTR,
    M_MKGRVS,
};

// The following describes the magic key functionality, where * represents the
// magic key and @ the repeat key. For example, tapping A and then the magic key
// types "ao". Most of this is coded in my `get_alt_repeat_key_keycode_user()`
// definition below.
//
// SFB removal and common n-grams:
//
//     A * -> AO     L * -> LK      S * -> SK
//     C * -> CY     M * -> MENT    T * -> TMENT
//     D * -> DY     O * -> OA      U * -> UE
//     E * -> EU     P * -> PY      Y * -> YP
//     G * -> GY     Q * -> QUEN    spc * -> THE
//     I * -> ION    R * -> RL
//
// When the magic key types a letter, following it with the repeat key produces
// "n". This is useful to type certain patterns without SFBs.
//
//     A * @ -> AON             (like "kaon")
//     D * @ -> DYN             (like "dynamic")
//     E * @ -> EUN             (like "reunite")
//     O * @ -> OAN             (like "loan")
//
// Other patterns:
//
//     spc * @ -> THEN
//     I * @ -> IONS            (like "nations")
//     M * @ -> MENTS           (like "moments")
//     Q * @ -> QUENC           (like "frequency")
//     T * @ -> TMENTS          (like "adjustments")
//     = *   -> ===             (JS code)
//     ! *   -> !==             (JS code)
//     " *   -> """<cursor>"""  (Python code)
//     ` *   -> ```<cursor>```  (Markdown code)
//     # *   -> #include        (C code)
//     & *   -> &nbsp;          (HTML code)
//     . *   -> ../             (shell)
//     . * @ -> ../../
uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods) {
    keycode = get_tap_keycode(keycode);

    if ((mods & ~MOD_MASK_SHIFT) == 0) {
        // This is where most of the "magic" for the MAGIC key is implemented.
        switch (keycode) {
            case KC_SPC: // spc -> THE
            case KC_ENT:
            case KC_TAB:
                return M_THE;

            // For navigating next/previous search results in Vim:
            // N -> Shift + N, Shift + N -> N.
            case KC_N:
                if ((mods & MOD_MASK_SHIFT) == 0) {
                    return S(KC_N);
                }
                return KC_N;

            // Fix SFBs and awkward strokes.
            case KC_A:
                return KC_O; // A -> O
            case KC_O:
                return KC_A; // O -> A
            case KC_E:
                return KC_U; // E -> U
            case KC_U:
                return KC_E; // U -> E
            case KC_I:
                if ((mods & MOD_MASK_SHIFT) == 0) {
                    return M_ION; // I -> ON
                } else {
                    return KC_QUOT; // Shift I -> '
                }
            case KC_M:
                return M_MENT; // M -> ENT
            case KC_Q:
                return M_QUEN; // Q -> UEN
            case KC_T:
                return M_TMENT; // T -> TMENT

            case KC_C:
                return KC_Y; // C -> Y
            case KC_D:
                return KC_Y; // D -> Y
            case KC_G:
                return KC_Y; // G -> Y
            case KC_P:
                return KC_Y; // P -> Y
            case KC_Y:
                return KC_P; // Y -> P

            case KC_L:
                return KC_K; // L -> K
            case KC_S:
                return KC_K; // S -> K

            case KC_R:
                return KC_L; // R -> L
            case KC_DOT:
                if ((mods & MOD_MASK_SHIFT) == 0) {
                    return M_UPDIR; // . -> ./
                }
                return M_NOOP;
            case KC_HASH:
                return M_INCLUDE; // # -> include
            case KC_AMPR:
                return M_NBSP; // & -> nbsp;
            case KC_EQL:
                return M_EQEQ; // = -> ==
            case KC_RBRC:
                return KC_SCLN; // ] -> ;

            case KC_COMM:
                if ((mods & MOD_MASK_SHIFT) != 0) {
                    return KC_EQL; // ! -> =
                }
                return M_NOOP;
            case KC_QUOT:
                if ((mods & MOD_MASK_SHIFT) != 0) {
                    return M_DOCSTR; // " -> ""<cursor>"""
                }
                return M_NOOP;
            case KC_GRV: // ` -> ``<cursor>``` (for Markdown code)
                return M_MKGRVS;
            case KC_LABK: // < -> - (for Haskell)
                return KC_MINS;
            case KC_SLSH:
                return KC_SLSH; // / -> / (easier reach than Repeat)

            case KC_PLUS:
            case KC_MINS:
            case KC_ASTR:
            case KC_PERC:
            case KC_PIPE:
            case KC_CIRC:
            case KC_TILD:
            case KC_EXLM:
            case KC_DLR:
            case KC_RABK:
            case KC_LPRN:
            case KC_RPRN:
            case KC_UNDS:
            case KC_COLN:
                return KC_EQL;

            case KC_F:
            case KC_V:
            case KC_X:
            case KC_SCLN:
            case KC_1 ... KC_0:
                return M_NOOP;
        }
    }

    switch (keycode) {
        case SELWBAK:
            return SELWORD;
        case SELWORD:
            return SELWBAK;
        case LGUI(KC_TAB):
            return LSG(KC_TAB);
        case LSG(KC_TAB):
            return LGUI(KC_TAB);
        case LGUI(KC_GRV):
            return LSG(KC_GRV);
    }
    return KC_TRNS;
}

bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        return true;
    }

    switch (keycode) {
        case M_THE:
            MAGIC_STRING(/* */ "the", KC_N);
            return false;
        case M_ION:
            MAGIC_STRING(/*i*/ "on", KC_S);
            return false;
        case M_MENT:
            MAGIC_STRING(/*m*/ "ent", KC_S);
            return false;
        case M_QUEN:
            MAGIC_STRING(/*q*/ "uen", KC_C);
            return false;
        case M_TMENT:
            MAGIC_STRING(/*t*/ "ment", KC_S);
            return false;
        case M_UPDIR:
            MAGIC_STRING(/*.*/ "./", UPDIR);
            return false;
        case M_INCLUDE:
            SEND_STRING_DELAY(/*#*/ "include ", TAP_CODE_DELAY);
            return false;
        case M_NBSP:
            SEND_STRING_DELAY(/*&*/ "nbsp;", TAP_CODE_DELAY);
            return false;
        case M_EQEQ:
            SEND_STRING_DELAY(/*=*/"==", TAP_CODE_DELAY);
            return false;
        case M_DOCSTR:
            SEND_STRING_DELAY(/*"*/ "\"\"\"\"\"" SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT), TAP_CODE_DELAY);
            return false;
        case M_MKGRVS:
            SEND_STRING_DELAY(/*`*/ "`````" SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT), TAP_CODE_DELAY);
            return false;
        case M_NOOP:
            return false;
    }
    return true;
}
