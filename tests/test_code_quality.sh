#!/bin/bash

# Test script for Code Quality Check
# Tests for proper header guards, includes, and basic code structure

set -e

echo "Testing Code Quality..."

quality_checks_passed=0

# Test 1: Check if header files have proper header guards
if [ -f "include/chip_state.h" ]; then
    if grep -q "#ifndef.*_H" include/chip_state.h && grep -q "#define.*_H" include/chip_state.h && grep -q "#endif" include/chip_state.h; then
        quality_checks_passed=$((quality_checks_passed + 1))
        echo "✓ chip_state.h has proper header guards"
    fi
fi

if [ -f "include/bit_macros.h" ]; then
    if grep -q "#ifndef.*_H" include/bit_macros.h && grep -q "#define.*_H" include/bit_macros.h && grep -q "#endif" include/bit_macros.h; then
        quality_checks_passed=$((quality_checks_passed + 1))
        echo "✓ bit_macros.h has proper header guards"
    fi
fi

# Test 2: Check if source files include necessary headers
if [ -f "src/chip_monitor.c" ]; then
    if grep -q "#include.*stdio.h" src/chip_monitor.c && grep -q "#include.*chip_state.h" src/chip_monitor.c; then
        quality_checks_passed=$((quality_checks_passed + 1))
        echo "✓ chip_monitor.c has proper includes"
    fi
fi

# Test 3: Check if bit operations include bit_macros.h
if [ -f "src/bit_operations.c" ]; then
    if grep -q "#include.*bit_macros.h" src/bit_operations.c; then
        quality_checks_passed=$((quality_checks_passed + 1))
        echo "✓ bit_operations.c includes bit_macros.h"
    fi
fi

# Test 4: Check for proper function documentation (/** comments)
doc_count=0
for file in src/*.c; do
    if [ -f "$file" ]; then
        if grep -q "/\*\*" "$file"; then
            doc_count=$((doc_count + 1))
        fi
    fi
done

if [ $doc_count -ge 3 ]; then
    quality_checks_passed=$((quality_checks_passed + 1))
    echo "✓ Source files have documentation comments"
fi

# Test 5: Check if Makefile exists and has basic targets
if [ -f "Makefile" ]; then
    if grep -q "all:" Makefile && grep -q "clean:" Makefile; then
        quality_checks_passed=$((quality_checks_passed + 1))
        echo "✓ Makefile has basic targets"
    fi
fi

# Test 6: Check for proper error handling patterns
error_handling_count=0
for file in src/*.c; do
    if [ -f "$file" ]; then
        if grep -q "NULL" "$file" && grep -q "return.*false\|return.*-1\|return.*0" "$file"; then
            error_handling_count=$((error_handling_count + 1))
        fi
    fi
done

if [ $error_handling_count -ge 2 ]; then
    quality_checks_passed=$((quality_checks_passed + 1))
    echo "✓ Source files have error handling"
fi

# Require at least 4 out of 6 tests to pass
if [ $quality_checks_passed -ge 4 ]; then
    echo "QUALITY_CHECKS_PASSED"
    exit 0
else
    echo "QUALITY_CHECKS_FAILED: Only $quality_checks_passed/6 tests passed"
    exit 1
fi

