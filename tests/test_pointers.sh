#!/bin/bash

# Test script for Task 1: Pointer Operations
# Tests basic pointer functionality and safety

set -e

echo "Testing Task 1: Pointer Operations..."

# Create bin directory if it doesn't exist
mkdir -p bin

# Compile the chip_monitor program
gcc -Wall -Wextra -std=c11 -Iinclude src/chip_monitor.c -o bin/chip_monitor -lm 2>/dev/null || {
    echo "COMPILATION_FAILED"
    exit 1
}

# Check if the executable was created
if [ ! -f "bin/chip_monitor" ]; then
    echo "EXECUTABLE_NOT_CREATED"
    exit 1
fi

# Run the program and capture output
output=$(timeout 10s ./bin/chip_monitor 2>&1 || true)

# Check for basic pointer operation indicators
pointer_tests_passed=0

# Test 1: Check if write_register_pointer function is called
if echo "$output" | grep -q "write_register_pointer\|register write\|Register write"; then
    pointer_tests_passed=$((pointer_tests_passed + 1))
fi

# Test 2: Check if read_register_pointer function is called
if echo "$output" | grep -q "read_register_pointer\|register read\|Register read"; then
    pointer_tests_passed=$((pointer_tests_passed + 1))
fi

# Test 3: Check if pointer arithmetic is demonstrated
if echo "$output" | grep -q "pointer arithmetic\|Pointer arithmetic\|array\|Array"; then
    pointer_tests_passed=$((pointer_tests_passed + 1))
fi

# Test 4: Check for proper error handling (NULL pointer checks)
if echo "$output" | grep -q "NULL\|null\|ERROR\|Error"; then
    pointer_tests_passed=$((pointer_tests_passed + 1))
fi

# Test 5: Check if program runs without segmentation fault
if ! echo "$output" | grep -q "Segmentation fault\|segmentation fault"; then
    pointer_tests_passed=$((pointer_tests_passed + 1))
fi

# Require at least 3 out of 5 tests to pass
if [ $pointer_tests_passed -ge 3 ]; then
    echo "POINTER_TESTS_PASSED"
    exit 0
else
    echo "POINTER_TESTS_FAILED: Only $pointer_tests_passed/5 tests passed"
    exit 1
fi

