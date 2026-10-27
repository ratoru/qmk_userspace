#pragma once
#include QMK_KEYBOARD_H

#include "ratoru_keycodes.h"

// Optional keymap-level hook, called by process_record_user before the
// userspace keycodes. Return false to stop further processing.
bool process_record_keymap(uint16_t keycode, keyrecord_t *record);

// Define custom alt repeat keys
#ifdef ARCANE_ENABLE
#    include "features/arcane.h"
#endif
