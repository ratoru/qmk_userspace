#include "ratoru.h"

static bool is_linux = false;

#ifdef NSM_ENABLE
/* N-SHOT MOD CONFIGURATION.
 * Reminder of the nshot structure:
 *
 * typedef struct {
 *     uint16_t      trigger;          // Keycode to activate the n-shot mod
 *     uint8_t       modbit;           // Modbit allows for mod combos.
 *     uint8_t       alternate_modbit; // Alternate modbit for Gui-Ctl swapping.
 *     uint8_t       max_count;        // one-shot, two-shot, ..., n-shot.
 *     bool          active_on_rolls;  // Behavior for A down, Mod down, A up = (active_on_rolls == true ? Mod-A : a)
 *     oneshot_state state;            // Direct from users/callum
 *     uint8_t       count;            // N-shot count state
 *     bool          had_keydown;      // keydown state for (active_on_rolls == false)
 * } nshot_state_t;
 */
#define MODBIT_HYPR  MOD_BIT(KC_LCTL) | MOD_BIT(KC_LGUI) | MOD_BIT(KC_LSFT) | MOD_BIT(KC_LALT)
nshot_state_t  nshot_states[] = {
//| trigger  | modbit            | swap-to          | max | roll into | State         | ## | timer | keydown? | //roll-in action |
//|----------|-------------------|------------------|-----|-----------|---------------|----|-------|----------|------------------|
    {OS_LSFT,  MOD_BIT(KC_LSFT),  MOD_BIT(KC_LSFT),   1,   true,       os_up_unqueued,  0,   0,     false},    // S-a
    {OS_LCTL,  MOD_BIT(KC_LCTL),  MOD_BIT(KC_LCTL),   1,   true,       os_up_unqueued,  0,   0,     false},    // C-a
    {OS_LALT,  MOD_BIT(KC_LALT),  MOD_BIT(KC_LALT),   1,   true,       os_up_unqueued,  0,   0,     false},    // A-a
    {OS_LGUI,  MOD_BIT(KC_LGUI),  MOD_BIT(KC_LGUI),   1,   true,       os_up_unqueued,  0,   0,     false},    // G-a
    {OS_HYPR,  MODBIT_HYPR,       MODBIT_HYPR,        1,   true,       os_up_unqueued,  0,   0,     false},    // G-a
};
uint8_t        NUM_NSHOT_STATES = sizeof(nshot_states) / sizeof(nshot_state_t);

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

// These are MacOS specific
swapper_state_t swapper_states[] = {
    {false, KC_LGUI, KC_TAB, S(KC_TAB), SW_APP, SW_REV},
    {false, KC_LGUI, KC_GRAVE, S(KC_GRAVE), SW_WIN, SW_REV},
    {false, KC_LCTL, KC_TAB, S(KC_TAB), SW_WIN, SW_REV}
};
uint8_t         NUM_SWAPPER_STATES = sizeof(swapper_states) / sizeof(swapper_state_t);

bool process_record_user(uint16_t keycode, keyrecord_t* record) {

  #ifdef NSM_ENABLE
  process_nshot_state(keycode, record, !is_linux);
  #endif

  process_swappers(keycode, record);

  switch (keycode) {
    case UPDIR:  // Types ../ to go up a directory on the shell.
      if (record->event.pressed) {
        SEND_STRING("../");
      }
      return false;
    case S_END:  // Sentence end
      if (record->event.pressed) {
        SEND_STRING(". ");
        add_oneshot_mods(MOD_BIT(KC_LSFT));  // Set one-shot mod for shift.
      }
      return false;
    case CPY_URL:  // Copy browser url
      if (record->event.pressed) {
        SEND_STRING(SS_LGUI("lc"));
      }
      return false;
    case SRCHSEL:  // Searches the current selection in a new tab.
      if (record->event.pressed) {
        // Windows users, change LGUI to LCTL.
        SEND_STRING(SS_LGUI("ct") SS_DELAY(100) SS_LGUI("v") SS_TAP(X_ENTER));
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

#ifdef HRM_ENABLE
void matrix_scan_user(void) {
  achordion_task();
}
#endif

// Define custom alt repeat keys
#ifdef REPEAT_KEY_ENABLE
uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods) {
    bool cmd_held = (mods & MOD_MASK_GUI);
    bool shift_held = (mods & MOD_MASK_SHIFT);
    if (cmd_held && shift_held) {
        switch (keycode) {
            case KC_Z: return G(KC_Z);  // CMD + Shift + Z reverses to CMD + Z.
            case KC_T: return G(KC_W);  // CMD + Shift + T reverses to CMD + W.
        }
    }
    if (cmd_held) {
        switch (keycode) {
            case KC_Z: return G(S(KC_Z));  // CMD + Z reverses to CMD + Shift + Z.
            case KC_W: return G(S(KC_T));  // CMD + W reverses to CMD + Shift + T.
        }
    }

    return KC_TRNS;  // Defer to default definitions.
}
#endif

void keyboard_pre_init_user(void) {
  // Set our LED pin as output
  setPinOutput(24);
  // Turn the LED off
  // (Due to technical reasons, high is off and low is on)
  writePinHigh(24);
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
         rgblight_sethsv_noeeprom (HSV_MAGENTA);
         break;
     case 2:
         rgblight_sethsv_noeeprom (HSV_BLUE);
         break;
     case 3:
         rgblight_sethsv_noeeprom (HSV_GOLD);
         break;
     case 4:
         rgblight_sethsv_noeeprom (HSV_GREEN);
         break;
     default: //  for any other layers, or the default layer
         rgblight_sethsv_noeeprom (HSV_PINK);
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
