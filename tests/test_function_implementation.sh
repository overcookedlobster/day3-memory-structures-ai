#!/bin/bash

# Test script for Function Implementation Check
# Tests if required functions are implemented (not just TODO stubs)

set -e

echo "Testing Function Implementation..."

functions_implemented=0

# Test 1: Check if chip_monitor.c has implemented functions
if [ -f "src/chip_monitor.c" ]; then
    # Look for actual implementation beyond TODO comments
    implemented_funcs=0

    # Check write_register_pointer implementation
    if grep -A 10 "write_register_pointer" src/chip_monitor.c | grep -v "TODO" | grep -q "return.*true\|reg_ptr\|pointer"; then
        implemented_funcs=$((implemented_funcs + 1))
    fi

    # Check chip_init implementation
    if grep -A 10 "chip_init" src/chip_monitor.c | grep -v "TODO" | grep -q "return.*true\|chip->\|strncpy"; then
        implemented_funcs=$((implemented_funcs + 1))
    fi

    if [ $implemented_funcs -ge 1 ]; then
        functions_implemented=$((functions_implemented + 1))
        echo "✓ chip_monitor.c has some implemented functions"
    fi
fi

# Test 2: Check if bit_operations.c has implemented functions
if [ -f "src/bit_operations.c" ]; then
    implemented_funcs=0

    # Check for actual bit manipulation code
    if grep -A 10 "demonstrate_basic_bit_ops\|configure_register_fields" src/bit_operations.c | grep -v "TODO" | grep -q "SET_BIT\|CLEAR_BIT\|reg.*="; then
        implemented_funcs=$((implemented_funcs + 1))
    fi

    if [ $implemented_funcs -ge 1 ]; then
        functions_implemented=$((functions_implemented + 1))
        echo "✓ bit_operations.c has some implemented functions"
    fi
fi

# Test 3: Check if multi_chip_test.c has implemented functions
if [ -f "src/multi_chip_test.c" ]; then
    implemented_funcs=0

    # Check for actual array/system operations
    if grep -A 10 "chip_system_init\|chip_system_update_all" src/multi_chip_test.c | grep -v "TODO" | grep -q "system->\|for.*i\|snprintf"; then
        implemented_funcs=$((implemented_funcs + 1))
    fi

    if [ $implemented_funcs -ge 1 ]; then
        functions_implemented=$((functions_implemented + 1))
        echo "✓ multi_chip_test.c has some implemented functions"
    fi
fi

# Test 4: Check if ai_enhanced_validation.c has some implementation
if [ -f "src/ai_enhanced_validation.c" ]; then
    implemented_funcs=0

    # Check for actual validation logic
    if grep -A 10 "ai_enhanced_chip_validation\|ai_detect_anomalies" src/ai_enhanced_validation.c | grep -v "TODO" | grep -q "score.*=\|return.*[0-9]\|chip->"; then
        implemented_funcs=$((implemented_funcs + 1))
    fi

    if [ $implemented_funcs -ge 1 ]; then
        functions_implemented=$((functions_implemented + 1))
        echo "✓ ai_enhanced_validation.c has some implemented functions"
    fi
fi

# Test 5: Check for removal of TODO comments (indicates implementation)
todo_removal=0
for file in src/*.c; do
    if [ -f "$file" ]; then
        total_lines=$(wc -l < "$file")
        todo_lines=$(grep -c "TODO:" "$file" || true)

        # If TODO lines are less than 50% of total lines, consider it partially implemented
        if [ $total_lines -gt 0 ] && [ $((todo_lines * 2)) -lt $total_lines ]; then
            todo_removal=$((todo_removal + 1))
        fi
    fi
done

if [ $todo_removal -ge 2 ]; then
    functions_implemented=$((functions_implemented + 1))
    echo "✓ Multiple files show implementation progress (reduced TODO density)"
fi

# Test 6: Check for actual code patterns that indicate implementation
code_patterns=0
for file in src/*.c; do
    if [ -f "$file" ]; then
        # Look for common implementation patterns
        if grep -q "if.*!=.*NULL\|for.*int.*i\|return.*true\|printf.*✓\|malloc\|free" "$file"; then
            code_patterns=$((code_patterns + 1))
        fi
    fi
done

if [ $code_patterns -ge 3 ]; then
    functions_implemented=$((functions_implemented + 1))
    echo "✓ Source files contain implementation patterns"
fi

# Require at least 4 out of 6 tests to pass
if [ $functions_implemented -ge 4 ]; then
    echo "FUNCTIONS_IMPLEMENTED"
    exit 0
else
    echo "FUNCTIONS_NOT_IMPLEMENTED: Only $functions_implemented/6 tests passed"
    exit 1
fi

