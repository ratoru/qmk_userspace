#pragma once
#include QMK_KEYBOARD_H

#include "keycodes.h"
#include "features/swapper.h"
#include "features/is_mac.h"

#ifdef NSM_ENABLE
#include "features/nshot_mod.h"
#endif

// gboards combos
#ifdef COMBO_ENABLE
#include "g/keymap_combo.h"
#endif

// Define custom alt repeat keys
#ifdef REPEAT_KEY_ENABLE
#include "features/arcane.h"
#endif
