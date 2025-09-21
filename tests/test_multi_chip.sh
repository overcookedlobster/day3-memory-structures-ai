#!/bin/bash

# Test script for Task 4: Multi-Chip Array Operations
# Tests array manipulation and string handling

set -e

echo "Testing Task 4: Multi-Chip Array Operations..."

# Create bin directory if it doesn't exist
mkdir -p bin

# Compile the multi_chip_test program
gcc -Wall -Wextra -std=c11 -Iinclude src/multi_chip_test.c -o bin/multi_chip_test -lm 2>/dev/null || {
    echo "COMPILATION_FAILED"
    exit 1
}

# Check if the executable was created
if [ ! -f "bin/multi_chip_test" ]; then
    echo "EXECUTABLE_NOT_CREATED"
    exit 1
fi

# Run the program and capture output
output=$(timeout 10s ./bin/multi_chip_test 2>&1 || true)

# Check for multi-chip operation indicators
multi_chip_tests_passed=0

# Test 1: Check if chip_system_init function is called
if echo "$output" | grep -q "chip_system_init\|System.*init\|Initialize.*system"; then
    multi_chip_tests_passed=$((multi_chip_tests_passed + 1))
fi

# Test 2: Check if chip_system_update_all function is called
if echo "$output" | grep -q "chip_system_update_all\|Update.*all\|update.*all"; then
    multi_chip_tests_passed=$((multi_chip_tests_passed + 1))
fi

# Test 3: Check if chip_system_find_by_id function is called
if echo "$output" | grep -q "chip_system_find_by_id\|find.*id\|Find.*chip\|search"; then
    multi_chip_tests_passed=$((multi_chip_tests_passed + 1))
fi

# Test 4: Check if chip_system_health_check function is called
if echo "$output" | grep -q "chip_system_health_check\|System.*health\|health.*check"; then
    multi_chip_tests_passed=$((multi_chip_tests_passed + 1))
fi

# Test 5: Check if chip_system_print_summary function is called
if echo "$output" | grep -q "chip_system_print_summary\|System.*summary\|print.*summary"; then
    multi_chip_tests_passed=$((multi_chip_tests_passed + 1))
fi

# Test 6: Check if sorting functionality is implemented
if echo "$output" | grep -q "sort.*temperature\|Sort.*temperature\|sorting"; then
    multi_chip_tests_passed=$((multi_chip_tests_passed + 1))
fi

# Test 7: Check if string operations are demonstrated
if echo "$output" | grep -q "string.*operation\|String.*operation\|demonstrate_string"; then
    multi_chip_tests_passed=$((multi_chip_tests_passed + 1))
fi

# Test 8: Check for chip ID patterns (CHIP_XX format)
if echo "$output" | grep -q "CHIP_[0-9][0-9]\|chip.*id\|Chip.*ID"; then
    multi_chip_tests_passed=$((multi_chip_tests_passed + 1))
fi

# Test 9: Check if program runs without crashing
if ! echo "$output" | grep -q "Segmentation fault\|segmentation fault"; then
    multi_chip_tests_passed=$((multi_chip_tests_passed + 1))
fi

# Require at least 6 out of 9 tests to pass
if [ $multi_chip_tests_passed -ge 6 ]; then
    echo "MULTI_CHIP_TESTS_PASSED"
    exit 0
else
    echo "MULTI_CHIP_TESTS_FAILED: Only $multi_chip_tests_passed/9 tests passed"
    exit 1
fi

