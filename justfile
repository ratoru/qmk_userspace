kb := "crkbd/rev1"
km := "ratoru"

default: all

all:
    qmk userspace-compile

compile:
    qmk compile -kb {{ kb }} -km {{ km }}

draw:
    ./keyboards/{{ kb }}/keymaps/{{ km }}/draw.sh
