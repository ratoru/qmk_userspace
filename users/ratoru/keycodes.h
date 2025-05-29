#pragma once
#include "quantum/keycodes.h"

enum custom_keycodes {
    // OS-specific utilities
    UPDIR = QK_USER,

    // General keycodes
    SRCHSEL, NEXTSEN, CPY_URL, JOINLN,

    // Swapper keys: Windows, tabs, generic Reverse, and browser tabs
    SW_APP, SW_WIN, SW_REV, SW_BTAB,

    // N-Shot Mods: "normal" callum-style n-shot mods (now featuring MULTI MODS!!!)
    OS_LSFT, OS_LCTL, OS_LALT, OS_LGUI, OS_HYPR,

    // RGB codes
    RGBT_NE, RGB_DB_NE, RGB_IB_NE,

    // Custom magic / repeat key
    ARCANE,

    NEW_SAFE_RANGE  // Use for keymap-specific codes
};
