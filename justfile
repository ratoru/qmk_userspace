km := "ratoru"

# Keyboard path mappings
kb_elora := "splitkb/halcyon/elora/rev2"
kb_kyria := "splitkb/kyria/rev3"
kb_sweep := "splitkb/aurora/sweep/rev1"

# Keyboard extra args
extra_elora := "-e HLC_TFT_DISPLAY=1"
extra_kyria := ""
extra_sweep := ""

default: (build "elora")

all: compile-all draw-all

compile-all:
    qmk userspace-compile

draw-all: _uv
    ./keyboards/crkbd/rev1/keymaps/ratoru/draw.sh
    ./keyboards/crkbd/rev4_1/standard/keymaps/ratoru/draw.sh
    ./keyboards/splitkb/aurora/sweep/rev1/keymaps/ratoru/draw.sh
    ./keyboards/splitkb/kyria/rev3/keymaps/ratoru/draw.sh
    ./keyboards/splitkb/halcyon/elora/rev2/keymaps/ratoru/draw.sh

# Build with keyboard name as argument
build keyboard: (compile keyboard) (draw keyboard)

compile keyboard:
    #!/usr/bin/env bash
    case "{{ keyboard }}" in
        elora) qmk compile -kb {{ kb_elora }} -km {{ km }} {{ extra_elora }} --compiledb ;;
        kyria) qmk compile -kb {{ kb_kyria }} -km {{ km }} {{ extra_kyria }} --compiledb ;;
        sweep) qmk compile -kb {{ kb_sweep }} -km {{ km }} {{ extra_sweep }} --compiledb ;;
        *) echo "Unknown keyboard: {{ keyboard }}"; exit 1 ;;
    esac

draw keyboard: _uv
    #!/usr/bin/env bash
    case "{{ keyboard }}" in
        elora) ./keyboards/{{ kb_elora }}/keymaps/{{ km }}/draw.sh ;;
        kyria) ./keyboards/{{ kb_kyria }}/keymaps/{{ km }}/draw.sh ;;
        sweep) ./keyboards/{{ kb_sweep }}/keymaps/{{ km }}/draw.sh ;;
        *) echo "Unknown keyboard: {{ keyboard }}"; exit 1 ;;
    esac

format:
    fd --extension c --extension h --search-path keyboards --search-path users --search-path layouts \
        --exec clang-format -i

_uv:
    @uv --version || echo 'Please install uv: https://docs.astral.sh/uv/getting-started/installation/'
