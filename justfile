kb := "splitkb/kyria/rev3"

default: all

all:
    qmk userspace-compile

compile:
    qmk compile -kb {{ kb }} -km ratoru

draw:
    ./keyboards/{{ kb }}/keymaps/ratoru/draw.sh
