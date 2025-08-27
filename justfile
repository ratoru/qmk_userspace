kb := "splitkb/aurora/sweep/rev1"
km := "ratoru"

default: build

all: compile-all draw-all

compile-all:
    qmk userspace-compile

draw-all: _uv
    ./keyboards/crkbd/rev1/keymaps/ratoru/draw.sh
    ./keyboards/crkbd/rev4_1/standard/keymaps/ratoru/draw.sh
    ./keyboards/splitkb/aurora/sweep/rev1/keymaps/ratoru/draw.sh
    ./keyboards/splitkb/kyria/rev3/keymaps/ratoru/draw.sh

build: compile draw

compile:
    qmk compile -kb {{ kb }} -km {{ km }}

draw: _uv
    ./keyboards/{{ kb }}/keymaps/{{ km }}/draw.sh

format:
    fd --extension c --extension h --search-path keyboards --search-path users --search-path layouts \
        --exec clang-format -i

_uv:
    @uv --version || echo 'Please install uv: https://docs.astral.sh/uv/getting-started/installation/'
