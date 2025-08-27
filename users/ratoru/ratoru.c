#include "ratoru.h"
#include "config.h"

#ifdef ARCANE_ENABLE
static uint16_t idle_timer = 0;
static bool     was_arcane = false;

void housekeeping_task_user(void) {
    if (idle_timer && timer_expired(timer_read(), idle_timer)) {
        // If execution reaches here, the keyboard has gone idle.
        idle_timer = 0;
    }
}

bool remember_last_key_user(uint16_t keycode, keyrecord_t* record, uint8_t* remembered_mods) {
    switch (keycode) {
        case ARCANE:
            // Ignore custom repeat keys.
            return false;
    }

    return true; // Other keys can be repeated.
}
#endif

#ifdef NSM_ENABLE
/* N-SHOT MOD CONFIGURATION.
 * Reminder of the nshot structure:
 *
 * typedef struct {
 *     uint16_t      trigger;          // Keycode to activate the n-shot mod
 *     uint8_t       modbit;           // Modbit allows for mod combos.
 *     uint8_t       alternate_modbit; // Alternate modbit for Gui-Ctl swapping.
 *     uint8_t       max_count;        // one-shot, two-shot, ..., n-shot.
 *     bool          active_on_rolls;  // Behavior for A down, Mod down, A up =
 * (active_on_rolls == true ? Mod-A : a) oneshot_state state;            //
 * Direct from users/callum uint8_t       count;            // N-shot count
 * state bool          had_keydown;      // keydown state for (active_on_rolls
 * == false) } nshot_state_t;
 */
#    define MODBIT_HYPR MOD_BIT(KC_LCTL) | MOD_BIT(KC_LGUI) | MOD_BIT(KC_LSFT) | MOD_BIT(KC_LALT)
nshot_state_t nshot_states[] = {
    //| trigger  | modbit            | swap-to          | max | roll into |
    // State         | ## | timer | keydown? | //roll-in action |
    //|----------|-------------------|------------------|-----|-----------|---------------|----|-------|----------|------------------|
    {OS_LSFT, MOD_BIT(KC_LSFT), MOD_BIT(KC_LSFT), 1, true, os_up_unqueued, 0, 0, false}, // S-a
    {OS_LCTL, MOD_BIT(KC_LCTL), MOD_BIT(KC_LGUI), 1, true, os_up_unqueued, 0, 0, false}, // C-a
    {OS_LALT, MOD_BIT(KC_LALT), MOD_BIT(KC_LALT), 1, true, os_up_unqueued, 0, 0, false}, // A-a
    {OS_LGUI, MOD_BIT(KC_LGUI), MOD_BIT(KC_LCTL), 1, true, os_up_unqueued, 0, 0, false}, // G-a
    {OS_HYPR, MODBIT_HYPR, MODBIT_HYPR, 1, true, os_up_unqueued, 0, 0, false},           // G-a
};
uint8_t NUM_NSHOT_STATES = sizeof(nshot_states) / sizeof(nshot_state_t);

bool is_nshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
        case TL_LOWR:
        case TL_UPPR:
        case TT(1):
        case TT(2):
        case TT(3):
        case TT(4):
        case TT(5):
            return true;
        default:
            return false;
    }
}

bool is_nshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
        case TL_LOWR:
        case TL_UPPR:
        case TT(1):
        case TT(2):
        case TT(3):
        case TT(4):
        case TT(5):
        case OS_LSFT:
        case OS_LCTL:
        case OS_LALT:
        case OS_LGUI:
        case OS_HYPR:
            return true;
        default:
            return false;
    }
}
#endif

swapper_state_t swapper_states[]   = {{false, IS_MAC ? KC_LGUI : KC_LALT, KC_TAB, S(KC_TAB), SW_APP, SW_REV}, {false, KC_LGUI, KC_GRAVE, S(KC_GRAVE), SW_WIN, SW_REV}, {false, KC_LCTL, KC_TAB, S(KC_TAB), SW_WIN, SW_REV}};
uint8_t         NUM_SWAPPER_STATES = sizeof(swapper_states) / sizeof(swapper_state_t);

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
#ifdef ARCANE_ENABLE
    bool is_idle = idle_timer == 0;
    idle_timer   = (record->event.time + IDLE_TIMEOUT_MS) | 1;
    if (keycode == ARCANE) {
        if (record->event.pressed) {
            was_arcane = process_arcane(get_last_keycode(), get_last_mods(), is_idle);
        }
        // Default to repeat key if no custom behavior is defined
        if (!was_arcane) {
            return process_repeat_key(QK_REPEAT_KEY, record);
        }
        return false;
    }
#endif

#ifdef NSM_ENABLE
    process_nshot_state(keycode, record, false);
#endif

    process_swappers(keycode, record);

    const uint8_t mods     = get_mods();
    const uint8_t all_mods = (mods | get_weak_mods()
#ifndef NO_ACTION_ONESHOT
                              | get_oneshot_mods()
#endif // NO_ACTION_ONESHOT
    );

    // If alt repeating key A, E, I, O, U, Y with no mods other than Shift, set
    // the last key to KC_N. Above, alternate repeat of KC_N is defined to be
    // again KC_N. This way, either tapping alt repeat and then repeat (or
    // equivalently double tapping alt repeat) is useful to type certain patterns
    // without SFBs:
    //
    //   D <altrep> <rep> -> DYN (as in "dynamic")
    //   O <altrep> <rep> -> OAN (as in "loan")
    if (get_repeat_key_count() < 0 && (all_mods & ~MOD_MASK_SHIFT) == 0 && (keycode == KC_A || keycode == KC_E || keycode == KC_I || keycode == KC_O || keycode == KC_U || keycode == KC_Y)) {
        set_last_keycode(KC_N);
        set_last_mods(0);
    }

    switch (keycode) {
        // When the Repeat key follows Space, it behaves as one-shot shift.
        case KC_SPC:
            if (get_repeat_key_count() > 0) {
                if (record->event.pressed) {
                    add_oneshot_mods(MOD_LSFT);
                    register_mods(MOD_LSFT);
                } else {
                    unregister_mods(MOD_LSFT);
                }
                return false;
            }
            break;
    }

    if (record->event.pressed) {
        switch (keycode) {
            case UPDIR: // Types ../ to go up a directory on the shell.
                SEND_STRING_DELAY("../", TAP_CODE_DELAY);
                return false;
            case NEXTSEN: // Sentence end
                SEND_STRING_DELAY(". ", TAP_CODE_DELAY);
                add_oneshot_mods(MOD_BIT(KC_LSFT)); // Set one-shot mod for shift.
                return false;
            case CPY_URL: // Copy browser url
                if (IS_MAC) {
                    SEND_STRING_DELAY(SS_LGUI("lc"), TAP_CODE_DELAY);
                } else {
                    SEND_STRING_DELAY(SS_LCTL("lc"), TAP_CODE_DELAY);
                }
                return false;
            case SRCHSEL: // Searches the current selection in a new tab.
                if (IS_MAC) {
                    SEND_STRING_DELAY(SS_LGUI("ct") SS_DELAY(100) SS_LGUI("v") SS_TAP(X_ENTER), TAP_CODE_DELAY);
                } else {
                    SEND_STRING_DELAY(SS_LCTL("ct") SS_DELAY(100) SS_LCTL("v") SS_TAP(X_ENTER), TAP_CODE_DELAY);
                }
                return false;
            case JOINLN: // Join lines like Vim's `J` command.
                if (IS_MAC) {
                    SEND_STRING_DELAY( // Go to the end of the line and tap delete.
                        SS_LGUI(SS_TAP(X_RGHT)) SS_TAP(X_DEL)
                        // In case this has joined two words together, insert one space.
                        SS_TAP(X_SPC) SS_LCTL(
                            // Go to the beginning of the next word.
                            SS_TAP(X_RGHT) SS_TAP(X_LEFT)
                            // Select back to the end of the previous word. This should select
                            // all spaces and tabs between the joined lines from indentation
                            // or trailing whitespace, including the space inserted earlier.
                            SS_LSFT(SS_TAP(X_LEFT) SS_TAP(X_RGHT)))
                        // Replace the selection with a single space.
                        SS_TAP(X_SPC),
                        TAP_CODE_DELAY);
                } else {
                    SEND_STRING_DELAY( // Go to the end of the line and tap delete.
                        SS_TAP(X_END) SS_TAP(X_DEL)
                        // In case this has joined two words together, insert one space.
                        SS_TAP(X_SPC) SS_LCTL(
                            // Go to the beginning of the next word.
                            SS_TAP(X_RGHT) SS_TAP(X_LEFT)
                            // Select back to the end of the previous word. This should select
                            // all spaces and tabs between the joined lines from indentation
                            // or trailing whitespace, including the space inserted earlier.
                            SS_LSFT(SS_TAP(X_LEFT) SS_TAP(X_RGHT)))
                        // Replace the selection with a single space.
                        SS_TAP(X_SPC),
                        TAP_CODE_DELAY);
                }
                return false;
#ifdef RGBLIGHT_ENABLE
            case RGBT_NE:
                rgblight_toggle_noeeprom();
                return false;
            case RGB_IB_NE:
                rgblight_increase_val_noeeprom();
                return false;
            case RGB_DB_NE:
                rgblight_decrease_val_noeeprom();
                return false;
#endif
            // Macros invoked through the MAGIC key.
            case M_THE:
                MAGIC_STRING(/* */ "the", KC_N);
                break;
            case M_ION:
                MAGIC_STRING(/*i*/ "on", KC_S);
                break;
            case M_MENT:
                MAGIC_STRING(/*m*/ "ent", KC_S);
                break;
            case M_QUEN:
                MAGIC_STRING(/*q*/ "uen", KC_C);
                break;
            case M_TMENT:
                MAGIC_STRING(/*t*/ "ment", KC_S);
                break;
            case M_UPDIR:
                MAGIC_STRING(/*.*/ "./", UPDIR);
                break;
            case M_INCLUDE:
                SEND_STRING_DELAY(/*#*/ "include ", TAP_CODE_DELAY);
                break;
            case M_EQEQ:
                SEND_STRING_DELAY(/*=*/"==", TAP_CODE_DELAY);
                break;
            case M_NBSP:
                SEND_STRING_DELAY(/*&*/ "nbsp;", TAP_CODE_DELAY);
                break;

            case M_DOCSTR:
                SEND_STRING_DELAY(/*"*/ "\"\"\"\"\"" SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT), TAP_CODE_DELAY);
                break;
            case M_MKGRVS:
                SEND_STRING_DELAY(/*`*/ "``\n\n```" SS_TAP(X_UP), TAP_CODE_DELAY);
                break;
        }
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
};

void suspend_power_down_user(void) {
    // code will run multiple times while keyboard is suspended
    rgblight_disable_noeeprom();
}

void suspend_wakeup_init_user(void) {
    // code will run on keyboard wakeup
    rgblight_enable_noeeprom();
    rgblight_sethsv_noeeprom(HSV_MAGENTA);
    rgblight_mode_noeeprom(1);
}
#endif

///////////////////////////////////////////////////////////////////////////////
// Sentence case (https://getreuer.info/posts/keyboards/sentence-case)
///////////////////////////////////////////////////////////////////////////////
#ifdef COMMUNITY_MODULE_SENTENCE_CASE_ENABLE
char sentence_case_press_user(uint16_t keycode, keyrecord_t* record, uint8_t mods) {
    if ((mods & ~(MOD_MASK_SHIFT | MOD_BIT_RALT)) == 0) {
        const bool shifted = mods & MOD_MASK_SHIFT;
        switch (keycode) {
            case KC_A ... KC_Z:
            case M_THE:
            case M_ION:
            case M_MENT:
            case M_TMENT:
                return 'a'; // Letter key.

            case KC_DOT: // Both . and Shift . (?) punctuate sentence endings.
            case KC_EXLM:
            case KC_QUES:
                return '.';

            case KC_COMM:
                return shifted ? '.' : '#';

            case KC_2 ... KC_0:       // 2 3 4 5 6 7 8 9 0
            case KC_AT ... KC_RPRN:   // @ # $ % ^ & * ( )
            case KC_MINS ... KC_SCLN: // - = [ ] backslash ;
            case KC_UNDS ... KC_COLN: // _ + { } | :
            case KC_GRV:
                return '#'; // Symbol key.

            case KC_SPC:
                return ' '; // Space key.

            case KC_QUOT:
            case KC_DQUO:
                return '\''; // Quote key.
        }
    }

    // Otherwise clear Sentence Case to initial state.
    sentence_case_clear();
    return '\0';
}
#endif // COMMUNITY_MODULE_SENTENCE_CASE_ENABLE
