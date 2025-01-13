#!/bin/bash

# Set script to stop on errors
set -e

# Clean up previous builds
echo "Cleaning previous builds..."
rm -f driver matmul.o

# Compile matmul.cpp
echo "Compiling matmul.cpp..."
g++ -c matmul.cpp -o matmul.o

# Compile and link driver.cpp with matmul.o
echo "Compiling and linking driver.cpp..."
g++ driver.cpp matmul.o -o driver

# Run the executable
echo "Running the program..."
./driver

echo "Execution completed."
