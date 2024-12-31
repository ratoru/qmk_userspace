# Custom ifdef for timeless homerow mods.
ifeq ($(strip $(THRM_ENABLE)), yes)
    OPT_DEFS += -DTHRM_ENABLE
endif

# Custom ifdef for callum oneshot modifiers
ifeq ($(strip $(COSM_ENABLE)), yes)
    OPT_DEFS += -DCOSM_ENABLE
endif

SRC += ratoru.c \
        features/select_word.c \
        features/swapper.c \
        features/oneshot.c

# Enable features on all keymaps
CAPS_WORD_ENABLE = yes
DYNAMIC_MACRO_ENABLE = yes
REPEAT_KEY_ENABLE = yes
TRI_LAYER_ENABLE = yes
LAYER_LOCK_ENABLE = yes
COMBO_ENABLE = yes

ifeq ($(strip $(COMBO_ENABLE)), yes)
    INTROSPECTION_KEYMAP_C = combos.c
endif

# Turn off unused features to reduce size
LTO_ENABLE = yes
CONSOLE_ENABLE = no
COMMAND_ENABLE = no
MOUSEKEY_ENABLE = no
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no
MAGIC_ENABLE = no
MUSIC_ENABLE = no
