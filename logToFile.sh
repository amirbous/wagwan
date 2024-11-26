#!/bin/bash

log_file="wagwan_log.txt"
INPUT_DIR="resources/"
OUTPUT_DIR="outputs"

echo "#wagwan test" > "$log_file"

# Loop through files recursively
find "$INPUT_DIR" -type f | while IFS= read -r file; do
  # Get the relative path
  relative_path="${file#$INPUT_DIR/}"
  
  # Compute the new output file path
  output_file="$OUTPUT_DIR/$relative_path"
  
  # Ensure the output folder structure exists
  output_dir="$(dirname "$output_file")"
  
  mkdir -p "$OUTPUT_DIR"
  echo "$file" >> "$log_file"  
  ./wagwan.out "$file" "$output_file" >> "$log_file"
  
  echo "Processed: $file -> $output_file"
done



