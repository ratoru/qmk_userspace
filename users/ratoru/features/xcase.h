#pragma once

#include QMK_KEYBOARD_H

// Handles the XCASE key and, while xcase is active, replaces spaces with the
// chosen separator. Returns false if the key was handled.
bool process_xcase(uint16_t keycode, keyrecord_t *record);
