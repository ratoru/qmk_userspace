#pragma once

#include "quantum_keycodes.h"

enum custom_keycodes {
    // OS-specific utilities
    UPDIR = QK_USER,

    // General keycodes
    SRCHSEL,
    CPY_URL,
    JOINLN,

    // N-Shot Mods: "normal" callum-style n-shot mods (now featuring MULTI MODS!)
    ONE_LSFT,
    ONE_LCTL,
    ONE_LALT,
    ONE_LGUI,
    ONE_HYPR,

    // RGB codes
    RGBT_NE,
    RGB_DB_NE,
    RGB_IB_NE,

    // Custom magic / repeat key
    ARCANE,

    NEW_SAFE_RANGE // Use for keymap-specific codes
};
