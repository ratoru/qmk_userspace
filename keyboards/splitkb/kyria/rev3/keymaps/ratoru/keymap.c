// This punctioation slightly deviates from Graphite
// by swapping - and _.
const custom_shift_key_t custom_shift_keys[] = {
    {KC_QUOTE, KC_MINS},        // Shift ' is -
    {KC_UNDS, KC_DOUBLE_QUOTE}, // Shift _ is "
    {KC_COMMA, KC_QUESTION},    // Shift , is ?
};

void keyboard_pre_init_user(void) {
    // Set our LED pin as output
    setPinOutput(24);
    // Turn the LED off
    // (Due to technical reasons, high is off and low is on)
    writePinHigh(24);
}
