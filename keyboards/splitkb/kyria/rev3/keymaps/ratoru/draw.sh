#!/usr/bin/env bash
set -euo pipefail
IFS=$'\n\t'

# ==== CONFIGURE THESE ====
LAYERS=("Gallium" "Graphite" "Nav" "Sym" "Num" "Fun") # List of layer names
COLS=12                                               # Number of columns
OUTPUT_NAME="kyria"                                   # Output file name (no ext.)

# ==== MAIN ====
# `just` always runs relative to the justfile
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
KEYMAP_FILE="$SCRIPT_DIR/keymap.json"
DRAWINGS_DIR="docs/generated/kyria"
YAML_FILE="$DRAWINGS_DIR/$OUTPUT_NAME.yaml"
COMBO_FILE="$SCRIPT_DIR/combos.yaml"
SVG_FILE="$DRAWINGS_DIR/$OUTPUT_NAME.svg"
CONFIG_PATH="drawer_config.yaml" # Always resolved relative to CWD

# Ensure drawings directory exists
mkdir -p "$DRAWINGS_DIR"

# Convert LAYERS array into a space-separated string
LAYERS_STR="${LAYERS[*]}"

# Generate intermediate YAML
uv run keymap -c "$CONFIG_PATH" parse -c "$COLS" -l $LAYERS_STR -q "$KEYMAP_FILE" >"$YAML_FILE"
echo "✅ YAML generated at: $YAML_FILE"

# Generate SVG from YAML
uv run keymap -c "$CONFIG_PATH" draw "$YAML_FILE" "$COMBO_FILE" >"$SVG_FILE"
echo "✅ SVG generated at: $SVG_FILE"

echo "🎉 All done! Your keymap drawing is ready."
