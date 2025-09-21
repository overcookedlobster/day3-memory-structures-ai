# Bit Operations Guide

## Overview

This guide provides comprehensive documentation for bit manipulation operations used in embedded systems programming, specifically for register control and hardware interface programming.

## Basic Bit Manipulation Macros

### Core Operations

```c
// Basic bit manipulation macros
#define SET_BIT(reg, bit)       ((reg) |= (1U << (bit)))
#define CLEAR_BIT(reg, bit)     ((reg) &= ~(1U << (bit)))
#define TOGGLE_BIT(reg, bit)    ((reg) ^= (1U << (bit)))
#define CHECK_BIT(reg, bit)     (((reg) >> (bit)) & 1U)
```

**Usage Examples:**
```c
uint32_t control_reg = 0x00000000;

SET_BIT(control_reg, 0);     // control_reg = 0x00000001
SET_BIT(control_reg, 4);     // control_reg = 0x00000011
TOGGLE_BIT(control_reg, 4);  // control_reg = 0x00000001
CLEAR_BIT(control_reg, 0);   // control_reg = 0x00000000

bool is_enabled = CHECK_BIT(control_reg, 0);  // false
```

### Advanced Operations

```c
// Advanced bit manipulation macros
#define SET_BITS(reg, mask)     ((reg) |= (mask))
#define CLEAR_BITS(reg, mask)   ((reg) &= ~(mask))
#define GET_FIELD(reg, mask, shift) (((reg) & (mask)) >> (shift))
#define SET_FIELD(reg, mask, shift, value) \
    ((reg) = ((reg) & ~(mask)) | (((value) << (shift)) & (mask)))
```

**Field Manipulation Examples:**
```c
uint32_t config_reg = 0x00000000;

// Set frequency field (bits 7-0) to 100
#define FREQ_MASK  0x000000FF
#define FREQ_SHIFT 0
SET_FIELD(config_reg, FREQ_MASK, FREQ_SHIFT, 100);
// config_reg = 0x00000064

// Set voltage field (bits 11-8) to 5
#define VOLT_MASK  0x00000F00
#define VOLT_SHIFT 8
SET_FIELD(config_reg, VOLT_MASK, VOLT_SHIFT, 5);
// config_reg = 0x00000564

// Read back frequency
uint32_t freq = GET_FIELD(config_reg, FREQ_MASK, FREQ_SHIFT);
// freq = 100
```

## Register Bit Field Definitions

### Control Register Layout

```c
// Control Register (32-bit)
// Bit 0: Enable
// Bit 1: Reset
// Bit 2: Debug Mode
// Bits 7-4: Power Level (0-15)
// Bits 11-8: Operating Mode (0-15)
// Bits 31-16: Reserved

#define CONTROL_ENABLE_BIT      0
#define CONTROL_RESET_BIT       1
#define CONTROL_DEBUG_BIT       2
#define CONTROL_POWER_MASK      0x000000F0
#define CONTROL_POWER_SHIFT     4
#define CONTROL_MODE_MASK       0x00000F00
#define CONTROL_MODE_SHIFT      8
```

### Status Register Layout

```c
// Status Register (32-bit)
// Bit 0: Ready
// Bit 1: Busy
// Bit 2: Error
// Bits 15-8: Temperature Code
// Bit 31: Valid

#define STATUS_READY_BIT        0
#define STATUS_BUSY_BIT         1
#define STATUS_ERROR_BIT        2
#define STATUS_TEMP_MASK        0x0000FF00
#define STATUS_TEMP_SHIFT       8
#define STATUS_VALID_BIT        31
```

### Error Register Layout

```c
// Error Register (32-bit)
// Bit 0: Thermal Error
// Bit 1: Voltage Error
// Bit 2: Timeout Error
// Bit 3: Parity Error
// Bit 4: Overflow Error

#define ERROR_THERMAL_BIT       0
#define ERROR_VOLTAGE_BIT       1
#define ERROR_TIMEOUT_BIT       2
#define ERROR_PARITY_BIT        3
#define ERROR_OVERFLOW_BIT      4
```

## Practical Applications

### 1. Chip Power Control

```c
void enable_chip_power(chip_state_t* chip) {
    if (chip == NULL) return;

    // Set enable bit
    SET_BIT(chip->registers.control_register, CONTROL_ENABLE_BIT);

    // Set power level to maximum (0xF)
    SET_FIELD(chip->registers.control_register, CONTROL_POWER_MASK,
              CONTROL_POWER_SHIFT, 0xF);

    // Clear reset bit if set
    CLEAR_BIT(chip->registers.control_register, CONTROL_RESET_BIT);

    // Update status register
    SET_BIT(chip->registers.status_register, STATUS_READY_BIT);
    CLEAR_BIT(chip->registers.status_register, STATUS_BUSY_BIT);
}
```

### 2. Error Flag Management

```c
uint32_t get_error_flags(const chip_state_t* chip) {
    if (chip == NULL) return 0xFFFFFFFF;

    uint32_t error_flags = chip->registers.error_register;

    if (error_flags != 0) {
        printf("Active errors:\n");
        if (CHECK_BIT(error_flags, ERROR_THERMAL_BIT)) {
            printf("  - Thermal error\n");
        }
        if (CHECK_BIT(error_flags, ERROR_VOLTAGE_BIT)) {
            printf("  - Voltage error\n");
        }
        if (CHECK_BIT(error_flags, ERROR_TIMEOUT_BIT)) {
            printf("  - Timeout error\n");
        }
        // ... check other error bits
    }

    return error_flags;
}

void clear_error_flags(chip_state_t* chip, uint32_t flags_to_clear) {
    if (chip == NULL) return;

    // Clear specified flags
    CLEAR_BITS(chip->registers.error_register, flags_to_clear);

    // Update status register if all errors cleared
    if (chip->registers.error_register == 0) {
        chip->has_errors = false;
        CLEAR_BIT(chip->registers.status_register, STATUS_ERROR_BIT);
    }
}
```

### 3. Operating Mode Control

```c
void set_chip_mode(chip_state_t* chip, uint8_t mode) {
    if (chip == NULL || mode > 15) return;

    uint8_t old_mode = GET_FIELD(chip->registers.control_register,
                                 CONTROL_MODE_MASK, CONTROL_MODE_SHIFT);

    SET_FIELD(chip->registers.control_register, CONTROL_MODE_MASK,
              CONTROL_MODE_SHIFT, mode);

    printf("Mode changed: %d -> %d\n", old_mode, mode);
}
```

## Bit Pattern Generation and Testing

### 1. Test Pattern Generation

```c
uint32_t create_test_pattern(int pattern_type) {
    switch (pattern_type) {
        case 0: // Alternating bits (0x55555555)
            return 0x55555555;

        case 1: // Inverse alternating (0xAAAAAAAA)
            return 0xAAAAAAAA;

        case 2: // Walking ones
            return 0x00000001;

        case 3: // Walking zeros
            return 0xFFFFFFFE;

        case 4: // Checkerboard high nibbles
            return 0xF0F0F0F0;

        case 5: // Checkerboard low nibbles
            return 0x0F0F0F0F;

        default:
            return 0xDEADBEEF;
    }
}
```

### 2. Pattern Validation

```c
int validate_bit_pattern(uint32_t value, uint32_t expected, uint32_t mask) {
    uint32_t masked_value = value & mask;
    uint32_t masked_expected = expected & mask;

    printf("Pattern validation:\n");
    printf("  Value:    0x%08X\n", value);
    printf("  Expected: 0x%08X\n", expected);
    printf("  Mask:     0x%08X\n", mask);
    printf("  Result:   %s\n", (masked_value == masked_expected) ? "PASS" : "FAIL");

    return (masked_value == masked_expected) ? 1 : 0;
}
```

## Advanced Bit Manipulation Techniques

### 1. Bit Counting

```c
// Count set bits using Brian Kernighan's algorithm
int count_set_bits(uint32_t value) {
    int count = 0;
    while (value) {
        value &= (value - 1);  // Clear lowest set bit
        count++;
    }
    return count;
}

// Using GCC builtin (faster)
int count_set_bits_builtin(uint32_t value) {
    return __builtin_popcount(value);
}
```

### 2. Bit Reversal

```c
uint32_t reverse_bits(uint32_t value) {
    uint32_t result = 0;
    for (int i = 0; i < 32; i++) {
        if (value & (1U << i)) {
            result |= (1U << (31 - i));
        }
    }
    return result;
}
```

### 3. Find First Set Bit

```c
int find_first_set_bit(uint32_t value) {
    if (value == 0) return -1;

    int position = 0;
    while ((value & 1) == 0) {
        value >>= 1;
        position++;
    }
    return position;
}

// Using GCC builtin (faster)
int find_first_set_builtin(uint32_t value) {
    return value ? __builtin_ctz(value) : -1;
}
```

## Register Analysis and Debugging

### 1. Register Visualization

```c
void analyze_register_bits(uint32_t register_value, const char* register_name) {
    printf("\n=== Register Analysis: %s ===\n", register_name);
    printf("Value: 0x%08X (%u decimal)\n", register_value, register_value);

    // Binary representation
    printf("Binary: ");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (register_value >> i) & 1);
        if (i % 4 == 0 && i > 0) printf(" ");
    }
    printf("\n");

    // Set bit positions
    printf("Set bits: ");
    bool first = true;
    for (int i = 0; i < 32; i++) {
        if (CHECK_BIT(register_value, i)) {
            if (!first) printf(", ");
            printf("%d", i);
            first = false;
        }
    }
    if (first) printf("None");
    printf("\n");

    // Nibble breakdown
    printf("Nibbles: ");
    for (int i = 7; i >= 0; i--) {
        uint32_t nibble = (register_value >> (i * 4)) & 0xF;
        printf("%X", nibble);
        if (i > 0) printf(" ");
    }
    printf("\n");
}
```

### 2. Register Comparison

```c
void compare_registers(uint32_t old_val, uint32_t new_val, const char* name) {
    printf("Register %s changed:\n", name);
    printf("  Old: 0x%08X\n", old_val);
    printf("  New: 0x%08X\n", new_val);

    uint32_t changed_bits = old_val ^ new_val;
    if (changed_bits != 0) {
        printf("  Changed bits: ");
        for (int i = 0; i < 32; i++) {
            if (CHECK_BIT(changed_bits, i)) {
                printf("%d ", i);
            }
        }
        printf("\n");
    }
}
```

## Performance Considerations

### 1. Compiler Optimizations

```c
// Efficient bit manipulation - compiler optimizes to single instruction
static inline void set_bit_fast(volatile uint32_t* reg, int bit) {
    *reg |= (1U << bit);
}

static inline void clear_bit_fast(volatile uint32_t* reg, int bit) {
    *reg &= ~(1U << bit);
}

static inline bool check_bit_fast(volatile uint32_t reg, int bit) {
    return (reg & (1U << bit)) != 0;
}
```

### 2. Batch Operations

```c
// Process multiple bits efficiently
void update_multiple_bits(uint32_t* reg, uint32_t set_mask, uint32_t clear_mask) {
    *reg = (*reg & ~clear_mask) | set_mask;
}

// Example usage
uint32_t control_reg = 0x00000000;
uint32_t bits_to_set = (1U << 0) | (1U << 4);    // Enable and power bit 0
uint32_t bits_to_clear = (1U << 1) | (1U << 2);  // Clear reset and debug
update_multiple_bits(&control_reg, bits_to_set, bits_to_clear);
```

## Common Pitfalls and Solutions

### 1. Signed vs Unsigned Operations

```c
// WRONG: Signed shift can cause undefined behavior
int32_t signed_val = -1;
int32_t result = signed_val >> 1;  // Implementation-defined behavior

// CORRECT: Use unsigned types for bit operations
uint32_t unsigned_val = 0xFFFFFFFF;
uint32_t result = unsigned_val >> 1;  // Well-defined: 0x7FFFFFFF
```

### 2. Shift Overflow

```c
// WRONG: Shifting by more than type width
uint32_t value = 1;
uint32_t result = value << 32;  // Undefined behavior

// CORRECT: Check shift amount
uint32_t safe_shift(uint32_t value, int shift) {
    if (shift >= 32 || shift < 0) return 0;
    return value << shift;
}
```

### 3. Operator Precedence

```c
// WRONG: Precedence issues
if (reg & MASK == EXPECTED) { ... }  // Wrong! Parsed as: reg & (MASK == EXPECTED)

// CORRECT: Use parentheses
if ((reg & MASK) == EXPECTED) { ... }
```

## Hardware-Specific Considerations

### 1. Volatile Registers

```c
// Hardware registers must be declared volatile
volatile uint32_t* const CONTROL_REG = (volatile uint32_t*)0x40000000;
volatile uint32_t* const STATUS_REG  = (volatile uint32_t*)0x40000004;

// Correct access pattern
void enable_hardware(void) {
    SET_BIT(*CONTROL_REG, 0);  // Enable bit

    // Wait for ready status
    while (!CHECK_BIT(*STATUS_REG, 0)) {
        // Busy wait - volatile ensures register is re-read
    }
}
```

### 2. Atomic Operations

```c
// For multi-threaded environments
#include <stdatomic.h>

atomic_uint32_t shared_flags = ATOMIC_VAR_INIT(0);

void atomic_set_flag(int bit) {
    atomic_fetch_or(&shared_flags, 1U << bit);
}

void atomic_clear_flag(int bit) {
    atomic_fetch_and(&shared_flags, ~(1U << bit));
}

bool atomic_check_flag(int bit) {
    return (atomic_load(&shared_flags) & (1U << bit)) != 0;
}
```

## Testing and Validation

### 1. Comprehensive Bit Tests

```c
void test_bit_operations(void) {
    uint32_t test_reg = 0;

    // Test SET_BIT
    SET_BIT(test_reg, 5);
    assert(test_reg == 0x00000020);
    assert(CHECK_BIT(test_reg, 5) == true);

    // Test CLEAR_BIT
    CLEAR_BIT(test_reg, 5);
    assert(test_reg == 0x00000000);
    assert(CHECK_BIT(test_reg, 5) == false);

    // Test TOGGLE_BIT
    TOGGLE_BIT(test_reg, 10);
    assert(CHECK_BIT(test_reg, 10) == true);
    TOGGLE_BIT(test_reg, 10);
    assert(CHECK_BIT(test_reg, 10) == false);

    printf("All bit operation tests passed!\n");
}
```

### 2. Field Operation Tests

```c
void test_field_operations(void) {
    uint32_t reg = 0;

    // Test field setting
    SET_FIELD(reg, 0x00000F00, 8, 0xA);
    assert(GET_FIELD(reg, 0x00000F00, 8) == 0xA);

    // Test field clearing
    SET_FIELD(reg, 0x00000F00, 8, 0x0);
    assert(GET_FIELD(reg, 0x00000F00, 8) == 0x0);

    printf("All field operation tests passed!\n");
}
```

## Conclusion

Bit manipulation is fundamental to embedded systems programming. This guide provides:

- **Comprehensive macro library** for common operations
- **Register modeling techniques** for hardware interfaces
- **Performance optimization strategies** for efficient code
- **Testing methodologies** for validation
- **Common pitfall avoidance** for robust code

Master these techniques to write efficient, maintainable embedded systems code that directly interfaces with hardware registers and control systems.

