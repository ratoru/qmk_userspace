/*
Define combos here.

COMB(name, keycode_shortcut, combo_sequence...)
SSTR(name, "string to send", combo_sequence...)
ACTN(name, function_call(),  combo_sequence...)

Use the COMB macro for simple keycode shortcuts
Example: COMB(vol_up, KC_VOLU, KC_Y, KC_U).

Use the SSTR macro for sending strings
Example: SSTR(which, "which ", KC_W, KC_H).

Use the ACTN macro for function callbacks
Example: ACTN(rgb_tog, rgb_matrix_toggle(), KC_Z, KC_X)
*/

// Macros
SSTR(vi_quit,  ":wq",    KC_Q,    KC_W)
SSTR(vi_save,  ":q!",    KC_Q,    KC_F)

// Simple keycodes
//   name      result           chord keys
COMB(O_TAB,  KC_TAB,   KC_T, KC_G)
COMB(O_ESC,  KC_ESC,   KC_N, KC_M)
COMB(O_CWTG,  CW_TOGG,   KC_T, KC_P)
COMB(O_SLWRD,  SELWORD,   KC_N, KC_L)
//COMB(IDX_ESC,  KC_ESC,          KC_D, KC_H)
//COMB(CMD_SPC,  LGUI(KC_SPC),    KC_G, KC_M)

// Home row combos
COMB(O_LSHFT, OSM(MOD_LSFT), KC_T, KC_D)
COMB(O_LGUI, OSM(MOD_LGUI), KC_S, KC_C)
COMB(O_LCTRL, OSM(MOD_LALT), KC_R, KC_X)
COMB(O_LALT, OSM(MOD_LCTL), KC_A, KC_Z)

COMB(O_RSHFT, OSM(MOD_RSFT), KC_N, KC_H)
COMB(O_RGUI, OSM(MOD_RGUI), KC_E, KC_COMMA)
COMB(O_RCTRL, OSM(MOD_RALT), KC_I, KC_DOT)
COMB(O_RALT, OSM(MOD_RCTL), KC_O, KC_SLASH)
