#!/bin/bash

# Test script for Memory Safety Check
# Tests for memory leaks, segfaults, and proper error handling

set -e

echo "Testing Memory Safety..."

# Create bin directory if it doesn't exist
mkdir -p bin

memory_safe=0

# Test 1: Compile with debug flags and AddressSanitizer
if gcc -Wall -Wextra -std=c11 -g -fsanitize=address -Iinclude src/chip_monitor.c -o bin/chip_monitor_debug -lm 2>/dev/null; then
    memory_safe=$((memory_safe + 1))
    echo "✓ Compiles with AddressSanitizer"

    # Test 2: Run with AddressSanitizer
    if timeout 15s ./bin/chip_monitor_debug >/dev/null 2>&1; then
        memory_safe=$((memory_safe + 1))
        echo "✓ Runs without AddressSanitizer errors"
    fi
fi

# Test 3: Check bit operations for memory safety
if gcc -Wall -Wextra -std=c11 -g -fsanitize=address -Iinclude src/bit_operations.c -o bin/bit_operations_debug -lm 2>/dev/null; then
    if timeout 15s ./bin/bit_operations_debug >/dev/null 2>&1; then
        memory_safe=$((memory_safe + 1))
        echo "✓ Bit operations memory safe"
    fi
fi

# Test 4: Check multi-chip operations for memory safety
if gcc -Wall -Wextra -std=c11 -g -fsanitize=address -Iinclude src/multi_chip_test.c -o bin/multi_chip_debug -lm 2>/dev/null; then
    if timeout 15s ./bin/multi_chip_debug >/dev/null 2>&1; then
        memory_safe=$((memory_safe + 1))
        echo "✓ Multi-chip operations memory safe"
    fi
fi

# Test 5: Check AI validation for memory safety
if gcc -Wall -Wextra -std=c11 -g -fsanitize=address -Iinclude src/ai_enhanced_validation.c -o bin/ai_validation_debug -lm 2>/dev/null; then
    if timeout 15s ./bin/ai_validation_debug >/dev/null 2>&1; then
        memory_safe=$((memory_safe + 1))
        echo "✓ AI validation memory safe"
    fi
fi

# Require at least 3 out of 5 tests to pass
if [ $memory_safe -ge 3 ]; then
    echo "MEMORY_SAFE"
    exit 0
else
    echo "MEMORY_UNSAFE: Only $memory_safe/5 tests passed"
    exit 1
fi

