#!/bin/bash

# Path to the solver binary.
SOLVER=./build/solver.out

# Directory containing .graph files.
TEST_DIR=./tests

# Output file
OUTPUT_FILE=results.txt

# Loop through all .graph files.
for file in "$TEST_DIR"/*.graph; do
    echo "----------------------------------------"
    echo "Running test: $(basename "$file")"
    $SOLVER --path "$file" >> "$OUTPUT_FILE"
done

echo "All test outputs written to $OUTPUT_FILE"