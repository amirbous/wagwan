#!/bin/bash

# Colors
GREEN="\e[32m"
RED="\e[31m"
CYAN="\e[36m"
YELLOW="\e[33m"
RESET="\e[0m"

echo -e "${CYAN}Compiling the C++ code...${RESET}"
if make; then
    echo -e "${GREEN}Compilation successful!${RESET}"
else
    echo -e "${RED}Compilation failed!${RESET}"
    exit 1
fi

# Define folders
INPUT_DIR="resources/small"
OUTPUT_DIR="outputs"
PROGRAM="./wagwan.out"
TIME_LIMIT=10

mkdir -p "$OUTPUT_DIR"

find "$INPUT_DIR" -type f | while IFS= read -r input_file; do
    output_file="$OUTPUT_DIR/$(basename "$input_file")"

    (
        output=$(timeout "$TIME_LIMIT" "$PROGRAM" "$input_file" "$output_file" 2>&1)
        exit_code=$?

        if [[ $exit_code -eq 0 ]]; then
            echo -e "[${CYAN}$(basename "$input_file")${RESET}] ${GREEN}✅ -> $output${RESET}"
        elif [[ $exit_code -eq 124 ]]; then
            echo -e "[${CYAN}$(basename "$input_file")${RESET}] ${RED}❌ -> ${YELLOW}Time limit exceeded${RESET}"
        else
            echo -e "[${CYAN}$(basename "$input_file")${RESET}] ${RED}❌ -> ${YELLOW}$output${RESET}"
        fi
    ) &
done

wait
echo -e "${GREEN}Processing complete!${RESET}"

