kb := "crkbd/rev4_1/standard"
km := "ratoru"

default: build

all: compile-all draw-all

compile-all:
    qmk userspace-compile

draw-all:
    ./keyboards/crkbd/rev1/keymaps/ratoru/draw.sh
    ./keyboards/crkbd/rev4_1/standard/keymaps/ratoru/draw.sh
    ./keyboards/splitkb/aurora/sweep/rev1/keymaps/ratoru/draw.sh
    ./keyboards/splitkb/kyria/rev3/keymaps/ratoru/draw.sh

build: compile draw

compile:
    qmk compile -kb {{ kb }} -km {{ km }}

draw:
    ./keyboards/{{ kb }}/keymaps/{{ km }}/draw.sh
