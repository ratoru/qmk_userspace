km := "ratoru"

# Keyboard path mappings
kb_corne := "crkbd/rev4_1/standard"
kb_elora := "splitkb/halcyon/elora/rev2"
kb_kyria := "splitkb/kyria/rev3"
kb_sweep := "splitkb/aurora/sweep/rev1"

# Keyboard extra args
extra_corne := ""
extra_elora := "-e HLC_TFT_DISPLAY=1"
extra_kyria := ""
extra_sweep := ""

default: (build "elora")

all: compile-all draw-all

compile-all:
    qmk userspace-compile

draw-all: (draw "corne") (draw "elora") (draw "kyria") (draw "sweep")

# Build with keyboard name as argument
build keyboard: (compile keyboard) (draw keyboard)

# Compiles one keymap and writes compile_commands.json for clangd.
compile keyboard:
    #!/usr/bin/env bash
    set -euo pipefail
    case "{{ keyboard }}" in
        corne) qmk compile -kb {{ kb_corne }} -km {{ km }} {{ extra_corne }} --compiledb ;;
        elora) qmk compile -kb {{ kb_elora }} -km {{ km }} {{ extra_elora }} --compiledb ;;
        kyria) qmk compile -kb {{ kb_kyria }} -km {{ km }} {{ extra_kyria }} --compiledb ;;
        sweep) qmk compile -kb {{ kb_sweep }} -km {{ km }} {{ extra_sweep }} --compiledb ;;
        *) echo "Unknown keyboard: {{ keyboard }}" >&2; exit 1 ;;
    esac

# Draws one keymap to docs/generated with keymap-drawer.
draw keyboard:
    #!/usr/bin/env bash
    set -euo pipefail
    case "{{ keyboard }}" in
        corne) kb={{ kb_corne }}; out=corne_4/corne_qwerty; cols=12; layers="Qwerty Nav Fun Num Sym" ;;
        elora) kb={{ kb_elora }}; out=elora/elora; cols=12; layers="Night Gallium Nav Sym Num Adjust" ;;
        kyria) kb={{ kb_kyria }}; out=kyria/kyria; cols=12; layers="Night Gallium Nav Sym Num Adjust" ;;
        sweep) kb={{ kb_sweep }}; out=sweep/sweep; cols=10; layers="Night Gallium Nav Sym Num" ;;
        *) echo "Unknown keyboard: {{ keyboard }}" >&2; exit 1 ;;
    esac
    dir="keyboards/$kb/keymaps/{{ km }}"
    keymap="$dir/keymap.json"
    if [ "{{ keyboard }}" = elora ]; then
        # keymap-drawer can't draw LAYOUT_elora_hlc, so draw it as
        # LAYOUT_split_4x6_5 without the last 10 keys of each layer.
        keymap=$(mktemp)
        trap 'rm -f "$keymap"' EXIT
        jq '.layout = "LAYOUT_split_4x6_5" | .layers = (.layers | map(.[:-10]))' "$dir/keymap.json" >"$keymap"
    fi
    yaml="docs/generated/$out.yaml"
    mkdir -p "$(dirname "$yaml")"
    uv run keymap -c drawer_config.yaml parse -c "$cols" -l $layers -q "$keymap" >"$yaml"
    uv run keymap -c drawer_config.yaml draw "$yaml" >"docs/generated/$out.svg"
    echo "Drew docs/generated/$out.svg"

format:
    fd --extension c --extension h --search-path keyboards --search-path users --search-path layouts \
        --exec clang-format -i
