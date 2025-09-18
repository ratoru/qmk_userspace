#!/usr/bin/env bash
set -euo pipefail
IFS=$'\n\t'

# ==== CONFIGURE THESE ====
LAYERS=("Night" "Gallium" "Nav" "Sym" "Num") # List of layer names
COLS=10                                      # Number of columns
OUTPUT_NAME="sweep"                          # Output file name (no ext.)

# ==== MAIN ====
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
KEYMAP_FILE="$SCRIPT_DIR/keymap.json"
DRAWINGS_DIR="docs/generated/sweep"
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
# echo -e "draw_config:\n  footer_text: '$FOOTER'" >>"$YAML_FILE"
echo "✅ YAML generated at: $YAML_FILE"

# Generate SVG from YAML
uv run keymap -c "$CONFIG_PATH" draw "$YAML_FILE" "$COMBO_FILE" >"$SVG_FILE"
echo "✅ SVG generated at: $SVG_FILE"

echo "🎉 All done! Your keymap drawing is ready."
