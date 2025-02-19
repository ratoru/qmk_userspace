#include "ratoru.h"
#include "features/select_word.h"
#include "features/swapper.h"

#ifdef HRM_ENABLE
#include "features/achordion.h"
#endif

#ifdef COSM_ENABLE
#include "features/oneshot.h"
#endif

bool sw_tab_active = false;
bool sw_control_tab_active = false;
bool sw_backtick_active = false;

#ifdef COSM_ENABLE
oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_cmd_state = os_up_unqueued;
oneshot_state os_hypr_state = os_up_unqueued;

bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
    case TL_LOWR:
    case TL_UPPR:
    case MO(2):
    case MO(3):
    case MO(4):
    case MO(5):
        return true;
    default:
        return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
    case TL_LOWR:
    case TL_UPPR:
    case MO(2):
    case MO(3):
    case MO(4):
    case MO(5):
    case KC_LSFT:
    case OS_SHFT:
    case OS_CTRL:
    case OS_ALT:
    case OS_CMD:
    case OS_HYPR:
        return true;
    default:
        return false;
    }
}
#endif

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  #ifdef HRM_ENABLE
  if (!process_achordion(keycode, record)) { return false; }
  #endif
  if (!process_select_word(keycode, record, SELWORD)) { return false; }

  // Adds functionality to switch apps and windows.
  update_swapper(
      &sw_tab_active, KC_LGUI, KC_TAB, SW_TAB,
      keycode, record
  );
  update_swapper(
      &sw_control_tab_active, KC_LCTL, KC_TAB, SW_CTAB,
      keycode, record
  );
  update_swapper(
      &sw_backtick_active, KC_LGUI, KC_GRAVE, SW_BTICK,
      keycode, record
  );

  #ifdef COSM_ENABLE
  // Process oneshot keys
  update_oneshot(
    &os_shft_state, KC_LSFT, OS_SHFT,
    keycode, record
  );
  update_oneshot(
    &os_ctrl_state, KC_LCTL, OS_CTRL,
    keycode, record
  );
  update_oneshot(
    &os_alt_state, KC_LALT, OS_ALT,
    keycode, record
  );
  update_oneshot(
    &os_cmd_state, KC_LCMD, OS_CMD,
    keycode, record
  );
  update_oneshot(
    &os_hypr_state, KC_HYPR, OS_HYPR,
    keycode, record
  );
  #endif

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

#ifdef RGBLIGHT_ENABLE
void keyboard_pre_init_user(void) {
  // Set our LED pin as output
  setPinOutput(24);
  // Turn the LED off
  // (Due to technical reasons, high is off and low is on)
  writePinHigh(24);
}

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
