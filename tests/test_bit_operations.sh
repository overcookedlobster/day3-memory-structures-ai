#!/bin/bash

# Test script for Task 3: Bit Operations
# Tests bit manipulation macros and operations

set -e

echo "Testing Task 3: Bit Operations..."

# Create bin directory if it doesn't exist
mkdir -p bin

# Compile the bit_operations program
gcc -Wall -Wextra -std=c11 -Iinclude src/bit_operations.c -o bin/bit_operations -lm 2>/dev/null || {
    echo "COMPILATION_FAILED"
    exit 1
}

# Check if the executable was created
if [ ! -f "bin/bit_operations" ]; then
    echo "EXECUTABLE_NOT_CREATED"
    exit 1
fi

# Run the program and capture output
output=$(timeout 10s ./bin/bit_operations 2>&1 || true)

# Check for bit operation indicators
bit_tests_passed=0

# Test 1: Check if basic bit operations are demonstrated
if echo "$output" | grep -q "SET_BIT\|CLEAR_BIT\|TOGGLE_BIT\|CHECK_BIT\|bit operation"; then
    bit_tests_passed=$((bit_tests_passed + 1))
fi

# Test 2: Check if register field operations are implemented
if echo "$output" | grep -q "configure_register_fields\|field\|Field"; then
    bit_tests_passed=$((bit_tests_passed + 1))
fi

# Test 3: Check if status register analysis is implemented
if echo "$output" | grep -q "analyze_status_register\|status\|Status"; then
    bit_tests_passed=$((bit_tests_passed + 1))
fi

# Test 4: Check if error register management is implemented
if echo "$output" | grep -q "manage_error_register\|error\|Error"; then
    bit_tests_passed=$((bit_tests_passed + 1))
fi

# Test 5: Check if advanced bit operations are demonstrated
if echo "$output" | grep -q "advanced_bit_ops\|Advanced\|byte\|Byte"; then
    bit_tests_passed=$((bit_tests_passed + 1))
fi

# Test 6: Check if chip bit operations are tested
if echo "$output" | grep -q "test_chip_bit_operations\|chip.*bit\|Chip.*bit"; then
    bit_tests_passed=$((bit_tests_passed + 1))
fi

# Test 7: Check for hexadecimal output (indicates bit manipulation)
if echo "$output" | grep -q "0x[0-9A-Fa-f]"; then
    bit_tests_passed=$((bit_tests_passed + 1))
fi

# Test 8: Check if program runs without crashing
if ! echo "$output" | grep -q "Segmentation fault\|segmentation fault"; then
    bit_tests_passed=$((bit_tests_passed + 1))
fi

# Require at least 5 out of 8 tests to pass
if [ $bit_tests_passed -ge 5 ]; then
    echo "BIT_TESTS_PASSED"
    exit 0
else
    echo "BIT_TESTS_FAILED: Only $bit_tests_passed/8 tests passed"
    exit 1
fi

