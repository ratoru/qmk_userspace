#pragma once

#include "quantum_keycodes.h"

enum custom_keycodes {
    // OS-specific utilities
    UPDIR = QK_USER,

    // General keycodes
    SRCHSEL,
    CPY_URL,
    JOINLN,

    // RGB codes
    RGBT_NE,
    RGB_DB_NE,
    RGB_IB_NE,

    // Custom magic / repeat key
    ARCANE,

    // snake_case, kebab-case and camelCase
    XCASE,

    NEW_SAFE_RANGE // Use for keymap-specific codes
};
