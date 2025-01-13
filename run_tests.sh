#!/bin/bash

# Path to the Unit_test directory (Update this if needed)
UNIT_TEST_DIR="./Unit_test"
RESULTS_FILE="results.csv"

# Check if Unit_test exists
if [ ! -d "$UNIT_TEST_DIR" ]; then
    echo "Error: Unit_test directory not found at $UNIT_TEST_DIR"
    exit 1
fi

# Compile the program
echo "Compiling the code..."
g++ driver.cpp matmul.cpp -o driver -I .
if [ $? -ne 0 ]; then
    echo "Compilation failed! Check driver.cpp and matmul.cpp for errors."
    exit 1
fi

echo "Compilation successful."

# Clear the old results.csv file
echo "Test Case,Status,Success,ijk Time (ms),ikj Time (ms),jik Time (ms),jki Time (ms),kij Time (ms),kji Time (ms)" > $RESULTS_FILE

# Loop through each test case
for TEST_CASE in "$UNIT_TEST_DIR"/*; do
    if [ -d "$TEST_CASE" ]; then
        TEST_NAME=$(basename "$TEST_CASE")  # Extract the test case name
        echo "Running test case: $TEST_NAME"

        # Run the driver for each test case
        ./driver "$TEST_CASE"
        EXIT_STATUS=$?

        # Check if the driver execution succeeded
        if [ $EXIT_STATUS -ne 0 ]; then
            echo "$TEST_NAME,Failed,Driver execution failed,,,,,," >> $RESULTS_FILE
        fi
    fi
done

echo "Test results saved to $RESULTS_FILE"
