#include QMK_KEYBOARD_H

uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods) {
    keycode = get_tap_keycode(keycode);

    switch (keycode) {
        case LGUI(KC_TAB):
            return LSG(KC_TAB);
        case LSG(KC_TAB):
            return LGUI(KC_TAB);
        case LGUI(KC_GRV):
            return LSG(KC_GRV);
    }
    return KC_TRNS;
}
