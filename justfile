kb := "crkbd/rev4_1/standard"
km := "ratoru"

default: build

all:
    qmk userspace-compile

build: compile draw

compile:
    qmk compile -kb {{ kb }} -km {{ km }}

draw:
    ./keyboards/{{ kb }}/keymaps/{{ km }}/draw.sh
