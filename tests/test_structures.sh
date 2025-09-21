#!/bin/bash

# Test script for Task 2: Structure Operations
# Tests ChipState structure functionality

set -e

echo "Testing Task 2: Structure Operations..."

# Create bin directory if it doesn't exist
mkdir -p bin

# Compile the chip_monitor program
gcc -Wall -Wextra -std=c11 -Iinclude src/chip_monitor.c -o bin/chip_monitor_struct -lm 2>/dev/null || {
    echo "COMPILATION_FAILED"
    exit 1
}

# Check if the executable was created
if [ ! -f "bin/chip_monitor_struct" ]; then
    echo "EXECUTABLE_NOT_CREATED"
    exit 1
fi

# Run the program and capture output
output=$(timeout 10s ./bin/chip_monitor_struct 2>&1 || true)

# Check for structure operation indicators
structure_tests_passed=0

# Test 1: Check if chip_init function is called
if echo "$output" | grep -q "chip_init\|Chip initialization\|Initialize chip"; then
    structure_tests_passed=$((structure_tests_passed + 1))
fi

# Test 2: Check if chip_update_state function is called
if echo "$output" | grep -q "chip_update_state\|Chip state update\|Update chip"; then
    structure_tests_passed=$((structure_tests_passed + 1))
fi

# Test 3: Check if chip_health_check function is called
if echo "$output" | grep -q "chip_health_check\|Health check\|health check"; then
    structure_tests_passed=$((structure_tests_passed + 1))
fi

# Test 4: Check if chip_print_state function is called
if echo "$output" | grep -q "chip_print_state\|print_state\|Display chip"; then
    structure_tests_passed=$((structure_tests_passed + 1))
fi

# Test 5: Check for structure pointer demonstration
if echo "$output" | grep -q "structure pointer\|Structure pointer\|demonstrate_structure"; then
    structure_tests_passed=$((structure_tests_passed + 1))
fi

# Test 6: Check for proper error handling
if echo "$output" | grep -q "NULL\|Error\|ERROR"; then
    structure_tests_passed=$((structure_tests_passed + 1))
fi

# Test 7: Check if program runs without crashing
if ! echo "$output" | grep -q "Segmentation fault\|segmentation fault"; then
    structure_tests_passed=$((structure_tests_passed + 1))
fi

# Require at least 4 out of 7 tests to pass
if [ $structure_tests_passed -ge 4 ]; then
    echo "STRUCTURE_TESTS_PASSED"
    exit 0
else
    echo "STRUCTURE_TESTS_FAILED: Only $structure_tests_passed/7 tests passed"
    exit 1
fi

