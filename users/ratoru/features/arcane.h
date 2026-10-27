#pragma once

#include QMK_KEYBOARD_H

// Acts on the last key typed. Returns false if ARCANE has nothing special for
// that key, in which case the caller falls back to a plain repeat.
bool process_arcane(uint16_t keycode, uint8_t mods, bool is_idle);
