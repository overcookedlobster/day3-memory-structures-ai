#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// Include chip structures for register operations
typedef struct {
    uint32_t control_register;
    uint32_t status_register;
    uint32_t error_register;
    uint32_t config_register;
} register_set_t;

typedef struct {
    char chip_id[16];
    char part_number[32];
    uint32_t serial_number;
    float temperature;
    float voltage;
    register_set_t registers;
    bool is_initialized;
    bool has_errors;
    uint32_t error_count;
    uint64_t uptime_seconds;
} chip_state_t;

// Basic bit manipulation macros
#define SET_BIT(reg, bit)       ((reg) |= (1U << (bit)))
#define CLEAR_BIT(reg, bit)     ((reg) &= ~(1U << (bit)))
#define TOGGLE_BIT(reg, bit)    ((reg) ^= (1U << (bit)))
#define CHECK_BIT(reg, bit)     (((reg) >> (bit)) & 1U)

// Advanced bit manipulation macros
#define SET_BITS(reg, mask)     ((reg) |= (mask))
#define CLEAR_BITS(reg, mask)   ((reg) &= ~(mask))
#define GET_FIELD(reg, mask, shift) (((reg) & (mask)) >> (shift))
#define SET_FIELD(reg, mask, shift, value) \
    ((reg) = ((reg) & ~(mask)) | (((value) << (shift)) & (mask)))

// Register bit field definitions
#define CONTROL_ENABLE_BIT      0
#define CONTROL_RESET_BIT       1
#define CONTROL_DEBUG_BIT       2
#define CONTROL_POWER_MASK      0x000000F0
#define CONTROL_POWER_SHIFT     4
#define CONTROL_MODE_MASK       0x00000F00
#define CONTROL_MODE_SHIFT      8

#define STATUS_READY_BIT        0
#define STATUS_BUSY_BIT         1
#define STATUS_ERROR_BIT        2
#define STATUS_TEMP_MASK        0x0000FF00
#define STATUS_TEMP_SHIFT       8

#define ERROR_THERMAL_BIT       0
#define ERROR_VOLTAGE_BIT       1
#define ERROR_TIMEOUT_BIT       2
#define ERROR_PARITY_BIT        3
#define ERROR_OVERFLOW_BIT      4

/**
 * Demonstrate basic bit operations
 */
void demonstrate_bit_operations(void) {
    printf("\n=== Basic Bit Operations Demo ===\n");

    uint32_t test_reg = 0x00000000;
    printf("Initial register value: 0x%08X\n", test_reg);

    // Set individual bits
    SET_BIT(test_reg, 0);
    printf("After SET_BIT(0):       0x%08X\n", test_reg);

    SET_BIT(test_reg, 4);
    printf("After SET_BIT(4):       0x%08X\n", test_reg);

    SET_BIT(test_reg, 15);
    printf("After SET_BIT(15):      0x%08X\n", test_reg);

    // Toggle bits
    TOGGLE_BIT(test_reg, 4);
    printf("After TOGGLE_BIT(4):    0x%08X\n", test_reg);

    TOGGLE_BIT(test_reg, 8);
    printf("After TOGGLE_BIT(8):    0x%08X\n", test_reg);

    // Clear bits
    CLEAR_BIT(test_reg, 0);
    printf("After CLEAR_BIT(0):     0x%08X\n", test_reg);

    // Check bits
    printf("Bit 8 is %s\n", CHECK_BIT(test_reg, 8) ? "SET" : "CLEAR");
    printf("Bit 0 is %s\n", CHECK_BIT(test_reg, 0) ? "SET" : "CLEAR");

    // Set multiple bits with mask
    SET_BITS(test_reg, 0x0000F000);
    printf("After SET_BITS(0xF000): 0x%08X\n", test_reg);

    // Clear multiple bits with mask
    CLEAR_BITS(test_reg, 0x0000FF00);
    printf("After CLEAR_BITS(0xFF00): 0x%08X\n", test_reg);
}

/**
 * Create test patterns for bit manipulation testing
 * @param pattern_type Type of pattern to create
 * @return Generated test pattern
 */
uint32_t create_test_pattern(int pattern_type) {
    uint32_t pattern = 0;

    switch (pattern_type) {
        case 0: // Alternating bits (0x55555555)
            pattern = 0x55555555;
            printf("Created alternating pattern: 0x%08X\n", pattern);
            break;

        case 1: // Inverse alternating bits (0xAAAAAAAA)
            pattern = 0xAAAAAAAA;
            printf("Created inverse alternating pattern: 0x%08X\n", pattern);
            break;

        case 2: // Walking ones
            pattern = 0x00000001;
            printf("Created walking ones pattern: 0x%08X\n", pattern);
            break;

        case 3: // Walking zeros
            pattern = 0xFFFFFFFE;
            printf("Created walking zeros pattern: 0x%08X\n", pattern);
            break;

        case 4: // Checkerboard high nibbles
            pattern = 0xF0F0F0F0;
            printf("Created checkerboard high pattern: 0x%08X\n", pattern);
            break;

        case 5: // Checkerboard low nibbles
            pattern = 0x0F0F0F0F;
            printf("Created checkerboard low pattern: 0x%08X\n", pattern);
            break;

        default:
            pattern = 0xDEADBEEF;
            printf("Created default test pattern: 0x%08X\n", pattern);
            break;
    }

    return pattern;
}

/**
 * Validate bit pattern against expected value
 * @param value Actual value to check
 * @param expected_pattern Expected pattern
 * @param mask Mask to apply before comparison
 * @return 1 if valid, 0 if invalid
 */
int validate_bit_pattern(uint32_t value, uint32_t expected_pattern, uint32_t mask) {
    uint32_t masked_value = value & mask;
    uint32_t masked_expected = expected_pattern & mask;

    printf("Validating pattern:\n");
    printf("  Value:    0x%08X\n", value);
    printf("  Expected: 0x%08X\n", expected_pattern);
    printf("  Mask:     0x%08X\n", mask);
    printf("  Masked Value:    0x%08X\n", masked_value);
    printf("  Masked Expected: 0x%08X\n", masked_expected);

    if (masked_value == masked_expected) {
        printf("  Result: PASS\n");
        return 1;
    } else {
        printf("  Result: FAIL\n");
        return 0;
    }
}

/**
 * Analyze and display register bit information
 * @param register_value Value to analyze
 * @param register_name Name of the register for display
 */
void analyze_register_bits(uint32_t register_value, const char* register_name) {
    printf("\n=== Register Analysis: %s ===\n", register_name);
    printf("Register Value: 0x%08X (%u decimal)\n", register_value, register_value);

    // Display binary representation
    printf("Binary: ");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (register_value >> i) & 1);
        if (i % 4 == 0 && i > 0) printf(" ");
    }
    printf("\n");

    // Display bit positions that are set
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

    // Display nibble breakdown
    printf("Nibbles: ");
    for (int i = 7; i >= 0; i--) {
        uint32_t nibble = (register_value >> (i * 4)) & 0xF;
        printf("%X", nibble);
        if (i > 0) printf(" ");
    }
    printf("\n");

    // Display byte breakdown
    printf("Bytes: ");
    for (int i = 3; i >= 0; i--) {
        uint32_t byte = (register_value >> (i * 8)) & 0xFF;
        printf("0x%02X", byte);
        if (i > 0) printf(" ");
    }
    printf("\n");
}

/**
 * Enable chip power using bit manipulation
 * @param chip Pointer to chip state
 */
void enable_chip_power(chip_state_t* chip) {
    if (chip == NULL) {
        printf("Error: NULL chip pointer in enable_chip_power\n");
        return;
    }

    printf("Enabling power for chip '%s'\n", chip->chip_id);

    // Set enable bit in control register
    SET_BIT(chip->registers.control_register, CONTROL_ENABLE_BIT);

    // Set power level to maximum (0xF)
    SET_FIELD(chip->registers.control_register, CONTROL_POWER_MASK,
              CONTROL_POWER_SHIFT, 0xF);

    // Clear reset bit if it was set
    CLEAR_BIT(chip->registers.control_register, CONTROL_RESET_BIT);

    // Set ready bit in status register
    SET_BIT(chip->registers.status_register, STATUS_READY_BIT);

    // Clear busy bit
    CLEAR_BIT(chip->registers.status_register, STATUS_BUSY_BIT);

    printf("Power enabled. Control register: 0x%08X\n",
           chip->registers.control_register);
}

/**
 * Disable chip power using bit manipulation
 * @param chip Pointer to chip state
 */
void disable_chip_power(chip_state_t* chip) {
    if (chip == NULL) {
        printf("Error: NULL chip pointer in disable_chip_power\n");
        return;
    }

    printf("Disabling power for chip '%s'\n", chip->chip_id);

    // Clear enable bit in control register
    CLEAR_BIT(chip->registers.control_register, CONTROL_ENABLE_BIT);

    // Set power level to minimum (0x0)
    SET_FIELD(chip->registers.control_register, CONTROL_POWER_MASK,
              CONTROL_POWER_SHIFT, 0x0);

    // Clear ready bit in status register
    CLEAR_BIT(chip->registers.status_register, STATUS_READY_BIT);

    // Set busy bit to indicate shutdown in progress
    SET_BIT(chip->registers.status_register, STATUS_BUSY_BIT);

    printf("Power disabled. Control register: 0x%08X\n",
           chip->registers.control_register);
}

/**
 * Check if chip is ready for operation
 * @param chip Pointer to chip state
 * @return true if ready, false otherwise
 */
bool is_chip_ready(const chip_state_t* chip) {
    if (chip == NULL) {
        printf("Error: NULL chip pointer in is_chip_ready\n");
        return false;
    }

    // Check if enable bit is set
    bool enabled = CHECK_BIT(chip->registers.control_register, CONTROL_ENABLE_BIT);

    // Check if ready bit is set
    bool ready = CHECK_BIT(chip->registers.status_register, STATUS_READY_BIT);

    // Check if busy bit is clear
    bool not_busy = !CHECK_BIT(chip->registers.status_register, STATUS_BUSY_BIT);

    // Check if error bit is clear
    bool no_error = !CHECK_BIT(chip->registers.status_register, STATUS_ERROR_BIT);

    bool chip_ready = enabled && ready && not_busy && no_error;

    printf("Chip '%s' ready status: %s\n", chip->chip_id,
           chip_ready ? "READY" : "NOT READY");
    printf("  Enabled: %s, Ready: %s, Not Busy: %s, No Error: %s\n",
           enabled ? "Yes" : "No", ready ? "Yes" : "No",
           not_busy ? "Yes" : "No", no_error ? "Yes" : "No");

    return chip_ready;
}

/**
 * Get error flags from chip
 * @param chip Pointer to chip state
 * @return Error flags bitmask
 */
uint32_t get_error_flags(const chip_state_t* chip) {
    if (chip == NULL) {
        printf("Error: NULL chip pointer in get_error_flags\n");
        return 0xFFFFFFFF;  // Return all errors for NULL pointer
    }

    uint32_t error_flags = chip->registers.error_register;

    printf("Chip '%s' error flags: 0x%08X\n", chip->chip_id, error_flags);

    if (error_flags != 0) {
        printf("  Active errors:\n");
        if (CHECK_BIT(error_flags, ERROR_THERMAL_BIT)) {
            printf("    - Thermal error\n");
        }
        if (CHECK_BIT(error_flags, ERROR_VOLTAGE_BIT)) {
            printf("    - Voltage error\n");
        }
        if (CHECK_BIT(error_flags, ERROR_TIMEOUT_BIT)) {
            printf("    - Timeout error\n");
        }
        if (CHECK_BIT(error_flags, ERROR_PARITY_BIT)) {
            printf("    - Parity error\n");
        }
        if (CHECK_BIT(error_flags, ERROR_OVERFLOW_BIT)) {
            printf("    - Overflow error\n");
        }
    } else {
        printf("  No errors detected\n");
    }

    return error_flags;
}

/**
 * Clear specific error flags
 * @param chip Pointer to chip state
 * @param flags_to_clear Bitmask of flags to clear
 */
void clear_error_flags(chip_state_t* chip, uint32_t flags_to_clear) {
    if (chip == NULL) {
        printf("Error: NULL chip pointer in clear_error_flags\n");
        return;
    }

    uint32_t old_flags = chip->registers.error_register;

    // Clear specified flags
    CLEAR_BITS(chip->registers.error_register, flags_to_clear);

    printf("Chip '%s' cleared error flags: 0x%08X -> 0x%08X\n",
           chip->chip_id, old_flags, chip->registers.error_register);

    // Update chip error status
    if (chip->registers.error_register == 0) {
        chip->has_errors = false;
        CLEAR_BIT(chip->registers.status_register, STATUS_ERROR_BIT);
        printf("  All errors cleared\n");
    } else {
        printf("  Some errors remain: 0x%08X\n", chip->registers.error_register);
    }
}

/**
 * Set chip operating mode using bit fields
 * @param chip Pointer to chip state
 * @param mode Operating mode (0-15)
 */
void set_chip_mode(chip_state_t* chip, uint8_t mode) {
    if (chip == NULL) {
        printf("Error: NULL chip pointer in set_chip_mode\n");
        return;
    }

    if (mode > 15) {
        printf("Error: Invalid mode %d (must be 0-15)\n", mode);
        return;
    }

    uint8_t old_mode = GET_FIELD(chip->registers.control_register,
                                 CONTROL_MODE_MASK, CONTROL_MODE_SHIFT);

    SET_FIELD(chip->registers.control_register, CONTROL_MODE_MASK,
              CONTROL_MODE_SHIFT, mode);

    printf("Chip '%s' mode changed: %d -> %d\n", chip->chip_id, old_mode, mode);
    printf("  Control register: 0x%08X\n", chip->registers.control_register);
}

/**
 * Update temperature field in status register
 * @param chip Pointer to chip state
 * @param temp_code Temperature code (0-255)
 */
void update_temperature_field(chip_state_t* chip, uint8_t temp_code) {
    if (chip == NULL) {
        printf("Error: NULL chip pointer in update_temperature_field\n");
        return;
    }

    uint8_t old_temp = GET_FIELD(chip->registers.status_register,
                                STATUS_TEMP_MASK, STATUS_TEMP_SHIFT);

    SET_FIELD(chip->registers.status_register, STATUS_TEMP_MASK,
              STATUS_TEMP_SHIFT, temp_code);

    printf("Chip '%s' temperature field updated: %d -> %d\n",
           chip->chip_id, old_temp, temp_code);
    printf("  Status register: 0x%08X\n", chip->registers.status_register);
}

/**
 * Demonstrate comprehensive bit manipulation operations
 */
void demonstrate_comprehensive_bit_ops(void) {
    printf("\n=== Comprehensive Bit Operations Demo ===\n");

    // Create test chip
    chip_state_t test_chip;
    memset(&test_chip, 0, sizeof(chip_state_t));
    strcpy(test_chip.chip_id, "TEST_CHIP");
    test_chip.is_initialized = true;

    // Initialize registers
    test_chip.registers.control_register = 0x00000000;
    test_chip.registers.status_register = 0x00000000;
    test_chip.registers.error_register = 0x00000000;
    test_chip.registers.config_register = 0x12345678;

    printf("\n--- Power Control Operations ---\n");
    enable_chip_power(&test_chip);
    is_chip_ready(&test_chip);

    printf("\n--- Mode Setting Operations ---\n");
    set_chip_mode(&test_chip, 5);
    set_chip_mode(&test_chip, 12);

    printf("\n--- Error Handling Operations ---\n");
    // Simulate some errors
    SET_BIT(test_chip.registers.error_register, ERROR_THERMAL_BIT);
    SET_BIT(test_chip.registers.error_register, ERROR_VOLTAGE_BIT);
    SET_BIT(test_chip.registers.status_register, STATUS_ERROR_BIT);
    test_chip.has_errors = true;

    get_error_flags(&test_chip);
    clear_error_flags(&test_chip, (1U << ERROR_THERMAL_BIT));
    get_error_flags(&test_chip);
    clear_error_flags(&test_chip, 0xFFFFFFFF);  // Clear all

    printf("\n--- Temperature Field Operations ---\n");
    update_temperature_field(&test_chip, 85);  // Hot temperature
    update_temperature_field(&test_chip, 45);  // Normal temperature

    printf("\n--- Register Analysis ---\n");
    analyze_register_bits(test_chip.registers.control_register, "Control Register");
    analyze_register_bits(test_chip.registers.status_register, "Status Register");

    printf("\n--- Power Down Operations ---\n");
    disable_chip_power(&test_chip);
    is_chip_ready(&test_chip);
}

/**
 * Test bit manipulation with various patterns
 */
void test_bit_patterns(void) {
    printf("\n=== Bit Pattern Testing ===\n");

    for (int pattern_type = 0; pattern_type < 6; pattern_type++) {
        printf("\n--- Pattern Type %d ---\n", pattern_type);

        uint32_t pattern = create_test_pattern(pattern_type);
        analyze_register_bits(pattern, "Test Pattern");

        // Test pattern validation
        validate_bit_pattern(pattern, pattern, 0xFFFFFFFF);  // Should pass
        validate_bit_pattern(pattern, ~pattern, 0xFFFFFFFF); // Should fail

        // Test with partial mask
        validate_bit_pattern(pattern, pattern, 0x0000FFFF);  // Should pass
    }
}

/**
 * Demonstrate advanced bit field operations
 */
void demonstrate_advanced_bit_fields(void) {
    printf("\n=== Advanced Bit Field Operations ===\n");

    uint32_t config_reg = 0x00000000;

    // Define some complex bit fields
    #define CONFIG_FREQ_MASK    0x000000FF
    #define CONFIG_FREQ_SHIFT   0
    #define CONFIG_VOLTAGE_MASK 0x00000F00
    #define CONFIG_VOLTAGE_SHIFT 8
    #define CONFIG_MODE_MASK    0x0000F000
    #define CONFIG_MODE_SHIFT   12
    #define CONFIG_FLAGS_MASK   0xFFFF0000
    #define CONFIG_FLAGS_SHIFT  16

    printf("Initial config register: 0x%08X\n", config_reg);

    // Set frequency field (0-255)
    SET_FIELD(config_reg, CONFIG_FREQ_MASK, CONFIG_FREQ_SHIFT, 100);
    printf("After setting frequency to 100: 0x%08X\n", config_reg);

    // Set voltage field (0-15)
    SET_FIELD(config_reg, CONFIG_VOLTAGE_MASK, CONFIG_VOLTAGE_SHIFT, 5);
    printf("After setting voltage to 5: 0x%08X\n", config_reg);

    // Set mode field (0-15)
    SET_FIELD(config_reg, CONFIG_MODE_MASK, CONFIG_MODE_SHIFT, 3);
    printf("After setting mode to 3: 0x%08X\n", config_reg);

    // Set flags field (0-65535)
    SET_FIELD(config_reg, CONFIG_FLAGS_MASK, CONFIG_FLAGS_SHIFT, 0xABCD);
    printf("After setting flags to 0xABCD: 0x%08X\n", config_reg);

    // Read back fields
    uint32_t freq = GET_FIELD(config_reg, CONFIG_FREQ_MASK, CONFIG_FREQ_SHIFT);
    uint32_t voltage = GET_FIELD(config_reg, CONFIG_VOLTAGE_MASK, CONFIG_VOLTAGE_SHIFT);
    uint32_t mode = GET_FIELD(config_reg, CONFIG_MODE_MASK, CONFIG_MODE_SHIFT);
    uint32_t flags = GET_FIELD(config_reg, CONFIG_FLAGS_MASK, CONFIG_FLAGS_SHIFT);

    printf("\nExtracted fields:\n");
    printf("  Frequency: %u\n", freq);
    printf("  Voltage:   %u\n", voltage);
    printf("  Mode:      %u\n", mode);
    printf("  Flags:     0x%04X\n", flags);

    analyze_register_bits(config_reg, "Final Config Register");
}

