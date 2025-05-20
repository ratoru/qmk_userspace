# Custom ifdef for timeless homerow mods.
ifeq ($(strip $(HRM_ENABLE)), yes)
    OPT_DEFS += -DHRM_ENABLE
endif

# Custom ifdef for n_shot modifiers (callum style).
ifeq ($(strip $(NSM_ENABLE)), yes)
    OPT_DEFS += -DNSM_ENABLE
endif

SRC += features/nshot_mod.c
SRC += features/swapper.c
SRC += features/oneshot.c

# Enable features on all keymaps
CAPS_WORD_ENABLE = yes
DYNAMIC_MACRO_ENABLE = yes
REPEAT_KEY_ENABLE = yes
TRI_LAYER_ENABLE = yes

ifeq ($(strip $(COMBO_ENABLE)), yes)
	VPATH += keyboards/gboards/ # using gboards combo code: see https://combos.gboards.ca/
    INTROSPECTION_KEYMAP_C = ratoru.c
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
