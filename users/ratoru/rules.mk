# Custom ifdef for arcane key
ifeq ($(strip $(ARCANE_ENABLE)), yes)
    OPT_DEFS += -DARCANE_ENABLE
    SRC += features/arcane.c
endif

# Custom ifdef for xcase
ifeq ($(strip $(XCASE_ENABLE)), yes)
    OPT_DEFS += -DXCASE_ENABLE
    SRC += features/xcase.c
endif

# Enable features on all keymaps
CAPS_WORD_ENABLE = yes
DYNAMIC_MACRO_ENABLE = yes
REPEAT_KEY_ENABLE = yes

SRC += ratoru.c

# Turn off unused features to reduce size
LTO_ENABLE = yes
CONSOLE_ENABLE = no
COMMAND_ENABLE = no
MOUSEKEY_ENABLE = no
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no
MAGIC_ENABLE = no
MUSIC_ENABLE = no

# halcyon features
ifneq ($(filter $(strip $(KEYBOARD)), splitkb/halcyon/kyria/rev4 \
                                      splitkb/halcyon/ferris/rev1 \
                                      splitkb/halcyon/lily58/rev2 \
                                      splitkb/halcyon/elora/rev2 \
                                      splitkb/halcyon/corne/rev2),)
	include $(USER_PATH)/splitkb/rules.mk
endif
