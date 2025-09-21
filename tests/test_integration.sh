#!/bin/bash

# Test script for Integration Testing
# Tests if all components work together properly

set -e

echo "Testing Integration of All Components..."

# Create bin directory if it doesn't exist
mkdir -p bin

integration_passed=0

# Test 1: Build all components successfully
if make clean >/dev/null 2>&1 && make all >/dev/null 2>&1; then
    integration_passed=$((integration_passed + 1))
    echo "✓ All components build successfully"
fi

# Test 2: All executables are created
expected_executables=("chip_monitor" "bit_operations" "multi_chip_test" "ai_enhanced_validation")
executables_found=0

for exe in "${expected_executables[@]}"; do
    if [ -f "bin/$exe" ]; then
        executables_found=$((executables_found + 1))
    fi
done

if [ $executables_found -eq ${#expected_executables[@]} ]; then
    integration_passed=$((integration_passed + 1))
    echo "✓ All expected executables created ($executables_found/${#expected_executables[@]})"
fi

# Test 3: All programs run without crashing
programs_run=0
for exe in "${expected_executables[@]}"; do
    if [ -f "bin/$exe" ]; then
        if timeout 10s ./bin/$exe >/dev/null 2>&1; then
            programs_run=$((programs_run + 1))
        fi
    fi
done

if [ $programs_run -ge 3 ]; then
    integration_passed=$((integration_passed + 1))
    echo "✓ Most programs run successfully ($programs_run/${#expected_executables[@]})"
fi

# Test 4: Check if header files are properly structured
if [ -f "include/chip_state.h" ] && [ -f "include/bit_macros.h" ]; then
    integration_passed=$((integration_passed + 1))
    echo "✓ Header files are present"
fi

# Test 5: Check if source files reference each other properly
cross_references=0
if grep -q "chip_state.h" src/chip_monitor.c; then
    cross_references=$((cross_references + 1))
fi
if grep -q "bit_macros.h" src/bit_operations.c; then
    cross_references=$((cross_references + 1))
fi
if grep -q "chip_state.h" src/multi_chip_test.c; then
    cross_references=$((cross_references + 1))
fi

if [ $cross_references -ge 2 ]; then
    integration_passed=$((integration_passed + 1))
    echo "✓ Source files properly reference headers"
fi

# Require at least 4 out of 5 tests to pass
if [ $integration_passed -ge 4 ]; then
    echo "INTEGRATION_PASSED"
    exit 0
else
    echo "INTEGRATION_FAILED: Only $integration_passed/5 tests passed"
    exit 1
fi

