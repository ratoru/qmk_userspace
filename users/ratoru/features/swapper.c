#include "swapper.h"
#include "keycodes.h"

// Based on https://github.com/callum-oakley/qmk_firmware/tree/master/users/callum
void process_swappers(uint16_t keycode, keyrecord_t *record) {
    swapper_state_t *curr_state = NULL;

    for (int i = 0; i < NUM_SWAPPER_STATES; ++i) {
        curr_state = &swapper_states[i];

        if (keycode == curr_state->forward_trigger) {
            if (record->event.pressed) {
                if (!curr_state->active) {
                    curr_state->active = true;
                    register_code16(curr_state->mod);
                }
                register_code16(curr_state->forward);
            } else {
                unregister_code16(curr_state->forward);
                // Don't unregister curr_state->mod until some other key is hit or released.
            }
        } else if (curr_state->active && keycode == curr_state->reverse_trigger) {
            if (record->event.pressed) {
                register_code16(curr_state->reverse);
            } else {
                unregister_code16(curr_state->reverse);
            }
        } else if (curr_state->active) {
            unregister_code16(curr_state->mod);
            curr_state->active = false;
        }
    }
}
