#include "ratoru.h"
#include "features/is_mac.h"

#ifdef ARCANE_ENABLE
static uint32_t last_event_time = 0;
static bool     was_arcane      = false;
#endif

// Layer-taps can only hold basic keycodes, so LT(layer, KC_F23) and
// LT(layer, KC_F24) stand in for Repeat and Alt Repeat as the tap action.
bool remember_last_key_user(uint16_t keycode, keyrecord_t *record, uint8_t *remembered_mods) {
    if (IS_QK_LAYER_TAP(keycode)) {
        keycode = QK_LAYER_TAP_GET_TAP_KEYCODE(keycode);
    }

    switch (keycode) {
#ifdef ARCANE_ENABLE
        case ARCANE:
#endif
#ifdef XCASE_ENABLE
        case XCASE:
#endif
        case KC_F23:
        case KC_F24:
            // Ignore custom repeat keys.
            return false;
    }

    return true; // Other keys can be repeated.
}

__attribute__((weak)) bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef ARCANE_ENABLE
    const bool is_idle = timer_elapsed32(last_event_time) > IDLE_TIMEOUT_MS;
    last_event_time    = timer_read32();
    if (keycode == ARCANE) {
        if (record->event.pressed) {
            // Strip mod-taps so home row mod letters match their plain keycode.
            was_arcane = process_arcane(get_tap_keycode(get_last_keycode()), get_last_mods(), is_idle);
        }
        // Default to repeat key if no custom behavior is defined
        if (!was_arcane) {
            return process_repeat_key(QK_REPEAT_KEY, record);
        }
        return false;
    }
#endif

    if (IS_QK_LAYER_TAP(keycode) && record->tap.count) {
        switch (QK_LAYER_TAP_GET_TAP_KEYCODE(keycode)) {
            case KC_F23:
                repeat_key_invoke(&record->event);
                return false;
            case KC_F24:
                alt_repeat_key_invoke(&record->event);
                return false;
        }
    }

#ifdef XCASE_ENABLE
    if (!process_xcase(keycode, record)) {
        return false;
    }
#endif

    if (!process_record_keymap(keycode, record)) {
        return false;
    }

    switch (keycode) {
        case UPDIR: // Types ../ to go up a directory on the shell.
            if (record->event.pressed) {
                SEND_STRING_DELAY("../", TAP_CODE_DELAY);
            }
            return false;
        case CPY_URL: // Copy browser URL
            if (record->event.pressed) {
                tap_code16(SHORTCUT(KC_L));
                tap_code16(SHORTCUT(KC_C));
            }
            return false;
        case SRCHSEL: // Searches the current selection in a new tab.
            if (record->event.pressed) {
                tap_code16(SHORTCUT(KC_C));
                tap_code16(SHORTCUT(KC_T));
                wait_ms(100);
                tap_code16(SHORTCUT(KC_V));
                tap_code(KC_ENTER);
            }
            return false;
        case JOINLN: // Join lines like Vim's `J` command.
            if (record->event.pressed) {
                // Go to the end of the line and tap delete. In case this has
                // joined two words together, insert one space. Then go to the
                // beginning of the next word and select back to the end of the
                // previous word. This selects all spaces and tabs between the
                // joined lines from indentation or trailing whitespace,
                // including the space inserted earlier. Finally, replace the
                // selection with a single space.
                if (IS_MAC) {
                    SEND_STRING_DELAY(SS_LGUI(SS_TAP(X_RGHT)) SS_TAP(X_DEL) SS_TAP(X_SPC) SS_LALT(SS_TAP(X_RGHT) SS_TAP(X_LEFT) SS_LSFT(SS_TAP(X_LEFT) SS_TAP(X_RGHT))) SS_TAP(X_SPC), TAP_CODE_DELAY);
                } else {
                    SEND_STRING_DELAY(SS_TAP(X_END) SS_TAP(X_DEL) SS_TAP(X_SPC) SS_LCTL(SS_TAP(X_RGHT) SS_TAP(X_LEFT) SS_LSFT(SS_TAP(X_LEFT) SS_TAP(X_RGHT))) SS_TAP(X_SPC), TAP_CODE_DELAY);
                }
            }
            return false;
#ifdef RGBLIGHT_ENABLE
        case RGBT_NE:
            if (record->event.pressed) {
                rgblight_toggle_noeeprom();
            }
            return false;
        case RGB_IB_NE:
            if (record->event.pressed) {
                rgblight_increase_val_noeeprom();
            }
            return false;
        case RGB_DB_NE:
            if (record->event.pressed) {
                rgblight_decrease_val_noeeprom();
            }
            return false;
#endif
    }
    return true;
}

#ifdef RGBLIGHT_ENABLE
void keyboard_post_init_user(void) {
    rgblight_enable_noeeprom(); // Enables RGB, without saving settings
    rgblight_sethsv_noeeprom(HSV_PINK);
    rgblight_mode_noeeprom(1);
}

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case 1:
            rgblight_sethsv_noeeprom(HSV_MAGENTA);
            break;
        case 2:
            rgblight_sethsv_noeeprom(HSV_BLUE);
            break;
        case 3:
            rgblight_sethsv_noeeprom(HSV_GOLD);
            break;
        case 4:
            rgblight_sethsv_noeeprom(HSV_GREEN);
            break;
        default: //  for any other layers, or the default layer
            rgblight_sethsv_noeeprom(HSV_PINK);
            break;
    }
    return state;
}

void suspend_power_down_user(void) {
    // code will run multiple times while keyboard is suspended
    rgblight_disable_noeeprom();
}

void suspend_wakeup_init_user(void) {
    // code will run on keyboard wakeup
    rgblight_enable_noeeprom();
    layer_state_set_user(layer_state); // Restore the current layer's color.
    rgblight_mode_noeeprom(1);
}
#endif

#ifdef CONVERT_TO_LIATRIS
void keyboard_pre_init_user(void) {
    // Turn off the Liatris power LED. It is active low.
    gpio_set_pin_output(24);
    gpio_write_pin_high(24);
}
#endif
