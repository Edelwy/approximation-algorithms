#!/bin/bash

# Path to the solver binary.
SOLVER=./build/solver.out

# Directory containing .graph files.
TEST_DIR=./tests

# Loop through all .graph files.
for i in $(seq 1 7); do
    (
    # Output file per iteration.
    OUTPUT_FILE="results_${i}.txt"

    for file in "$TEST_DIR"/*.graph; do
        echo "----------------------------------------"
        echo "Running test: $(basename "$file")"
        $SOLVER --path "$file" -n 60 >> "$OUTPUT_FILE"
    done
    ) &
done

wait  # Wait for all background jobs to finish.
echo "All test outputs written to $OUTPUT_FILE"