#include "nshot_mod.h"

void process_nshot_state(uint16_t keycode, keyrecord_t *record, bool use_alternate) {
    nshot_state_t *curr_state = NULL;

    // loop through all mods defined in the nshot_states dictionary
    for (int i = 0; i < NUM_NSHOT_STATES; ++i) {
        // Set the current nshot_state
        curr_state = &nshot_states[i];

        // Get the nshot's under-the-hood config values
        uint8_t max_count = curr_state->max_count * 2;
        uint8_t modbit = use_alternate ? curr_state->alternate_modbit : curr_state->modbit;

        // and let's get processing!

        // First, let's handle the trigger key.
        //
        if (keycode == curr_state->trigger) {
            if (record->event.pressed) {
                // Trigger is down, and we're in unqueued state: register the mod!
                if (curr_state->state == os_up_unqueued) {
                    register_mods(modbit);
                }

                // Update state: key is down, no other keys have been tapped
                curr_state->state = os_down_unused;
                curr_state->count = 0;

                // If we allow active rolls, pretend we've already had one keydown--
                // there are only [max_count] key up/downs permitted total, and the rolled key
                // counts toward that!
                curr_state->had_keydown = curr_state->active_on_rolls;

                // and read the timer. This will let us treat the mod like a normal held mod if needed.
                curr_state->timer = timer_read();
            } else {
                // Trigger is up! What happened during the keypress?
                switch (curr_state->state) {
                    case os_down_unused:
                        // If we didn't use the mod but held it for TAPPING TERM, unregister it.
                        if((timer_elapsed(curr_state->timer)) >= TAPPING_TERM) {
                            curr_state->state = os_up_unqueued;
                            unregister_mods(modbit);
                            break;
                        }
                        // Otherwise, queue it--we've got an n-shot!
                        curr_state->state = os_up_queued;
                        break;
                    case os_down_used:
                        // If we did use the mod while trigger was held, unregister it.
                        curr_state->state = os_up_unqueued;
                        unregister_mods(modbit);
                        break;
                    default:
                        break;
                }
            }
            // and then the trigger key is done processing for this loop!
        } else {
            // Next, let's handle the non-trigger keys, starting with the non-trigger keydown.
            //
            if (record->event.pressed) {
                // If we have a mod in a non-default state and we've hit a cancel key,
                // cancel the mod and reset state.
                if (curr_state->state != os_up_unqueued && is_nshot_cancel_key(keycode)) {
                    curr_state->state = os_up_unqueued;
                    curr_state->count = 0;
                    curr_state->had_keydown = curr_state->active_on_rolls;
                    unregister_mods(modbit);
                }

                // Part 1 of fast-rolling DOuble ACtivation prevention: count the key events.
                // Check for oneshot completion on sequential keys while rolling.
                // curr_state->state will only be os_up_queued after the n-shot has been triggered.
                if (curr_state->state == os_up_queued && !is_nshot_ignored_key(keycode)) {
                    // Increment on sequential key press.
                    curr_state->count = curr_state->count + 1;
                    curr_state->had_keydown = true;

                    // If count > max_count, the previous key hit maxed out the n-shot.
                    // Complete the n-shot; this current keycode will be pressed sans mod.
                    if (curr_state->count == max_count) {
                        curr_state->state = os_up_unqueued;
                        curr_state->count = 0;
                        curr_state->had_keydown = curr_state->active_on_rolls;
                        unregister_mods(modbit);
                    }
                }

                // Part 2 of fast-rolling DOuble ACtivation prevention:
                // record that we've had a keydown if the mod is down. We're probably rolling!
                if (curr_state->state == os_down_unused){
                    curr_state->had_keydown = true;
                }
            } else {
                // Finally, non-trigger key-up!
                //
                if (!is_nshot_ignored_key(keycode)) {
                    // On non-ignored keyup, consider the oneshot used.
                    switch (curr_state->state) {
                        case os_down_unused:
                            // if there's been a keydown, the mod is being used normally. Mark as used.
                            if(curr_state->had_keydown){
                                curr_state->state = os_down_used;
                            }
                            // if there has *not* been a keydown, the mod is being rolled into
                            // and this first keyup is not part of the mod behavior.
                            break;
                        case os_up_queued:
                            // If there's been a keydown, the mod key is being used as an n-shot.
                            // Increment the keycount.
                            if (curr_state->had_keydown){
                                curr_state->count = curr_state->count + 1;
                            }

                            // If the n-shot max has been reached, complete the n-shot.
                            if (curr_state->count == max_count) {
                                curr_state->state = os_up_unqueued;
                                curr_state->count = 0;
                                curr_state->had_keydown = curr_state->active_on_rolls;
                                unregister_mods(modbit);
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }
}

