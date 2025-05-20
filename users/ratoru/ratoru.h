#pragma once
#include QMK_KEYBOARD_H

#include "features/swapper.h"

#ifdef NSM_ENABLE
#include "features/nshot_mod.h"
#endif

// gboards combos
#ifdef COMBO_ENABLE
#include "g/keymap_combo.h"
#endif
