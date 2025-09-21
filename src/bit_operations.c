/**
 * @file bit_operations.c
 * @brief Day 3 Task 3: Bit Manipulation Operations (50 minutes)
 *
 * This file implements comprehensive bit manipulation operations for register
 * control and status checking. Students will master bitwise operations,
 * field extraction/insertion, and register manipulation techniques.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "bit_macros.h"
#include "chip_state.h"

/**
 * Task 3: Bit Manipulation Operations (50 minutes)
 *
 * TODO: Implement comprehensive bit manipulation for register operations
 *
 * Learning objectives:
 * - Master SET_BIT, CLEAR_BIT, TOGGLE_BIT, CHECK_BIT macros
 * - Implement register field extraction and insertion
 * - Practice bit masking for multi-bit fields
 * - Develop status flag management systems
 */

/**
 * @brief Demonstrate basic bit manipulation operations
 * @param initial_value Starting register value
 */
void demonstrate_basic_bit_ops(uint32_t initial_value) {
    // TODO: Implement basic bit operation demonstration
    //
    // Steps to complete:
    // 1. Start with the initial_value
    // 2. Use SET_BIT to set various bits (0, 4, 8, 16)
    // 3. Use CLEAR_BIT to clear some bits (1, 5, 9)
    // 4. Use TOGGLE_BIT to toggle bits (2, 6, 10)
    // 5. Use CHECK_BIT to verify bit states
    // 6. Print the register value after each operation
    //
    // Example operations:
    // - Set enable bit (bit 0)
    // - Clear reset bit (bit 1)
    // - Toggle test mode bit (bit 2)
    // - Check status bits

    printf("TODO: Implement demonstrate_basic_bit_ops function\n");
    printf("  - Starting value: 0x%08X\n", initial_value);
    printf("  - Demonstrate SET_BIT, CLEAR_BIT, TOGGLE_BIT, CHECK_BIT\n");
    printf("  - Show register value after each operation\n");

    uint32_t reg = initial_value;

    // Example framework (uncomment and complete):
    // printf("Initial register: 0x%08X\n", reg);
    //
    // SET_BIT(reg, CTRL_ENABLE_BIT);
    // printf("After SET_BIT(ENABLE): 0x%08X\n", reg);
    //
    // CLEAR_BIT(reg, CTRL_RESET_BIT);
    // printf("After CLEAR_BIT(RESET): 0x%08X\n", reg);
    //
    // TOGGLE_BIT(reg, CTRL_TEST_MODE_BIT);
    // printf("After TOGGLE_BIT(TEST_MODE): 0x%08X\n", reg);
    //
    // if (CHECK_BIT(reg, CTRL_ENABLE_BIT)) {
    //     printf("Enable bit is SET\n");
    // } else {
    //     printf("Enable bit is CLEAR\n");
    // }
}

/**
 * @brief Implement register field operations
 * @param reg Pointer to register value
 * @param mode New mode value (3 bits)
 * @param speed New speed value (2 bits)
 * @param voltage New voltage value (4 bits)
 */
void configure_register_fields(uint32_t *reg, uint8_t mode, uint8_t speed, uint8_t voltage) {
    // TODO: Implement multi-bit field operations
    //
    // Steps to complete:
    // 1. Validate input parameters
    // 2. Use INSERT_FIELD macro to set mode field (bits 2:0)
    // 3. Use INSERT_FIELD macro to set speed field (bits 5:4)
    // 4. Use INSERT_FIELD macro to set voltage field (bits 11:8)
    // 5. Verify field values using EXTRACT_FIELD
    // 6. Print before/after register values
    //
    // Field definitions (from bit_macros.h):
    // - CONFIG_MODE_POS = 0, CONFIG_MODE_WIDTH = 3
    // - CONFIG_SPEED_POS = 4, CONFIG_SPEED_WIDTH = 2
    // - CONFIG_VOLTAGE_POS = 8, CONFIG_VOLTAGE_WIDTH = 4

    if (reg == NULL) {
        printf("ERROR: Register pointer is NULL\n");
        return;
    }

    printf("TODO: Implement configure_register_fields function\n");
    printf("  - Set mode field (3 bits): %d\n", mode);
    printf("  - Set speed field (2 bits): %d\n", speed);
    printf("  - Set voltage field (4 bits): %d\n", voltage);
    printf("  - Current register value: 0x%08X\n", *reg);

    // Example framework (uncomment and complete):
    // uint32_t old_value = *reg;
    //
    // INSERT_FIELD(*reg, mode, CONFIG_MODE_POS, CONFIG_MODE_WIDTH);
    // INSERT_FIELD(*reg, speed, CONFIG_SPEED_POS, CONFIG_SPEED_WIDTH);
    // INSERT_FIELD(*reg, voltage, CONFIG_VOLTAGE_POS, CONFIG_VOLTAGE_WIDTH);
    //
    // printf("Register updated: 0x%08X -> 0x%08X\n", old_value, *reg);
    //
    // // Verify the fields were set correctly
    // uint8_t read_mode = EXTRACT_FIELD(*reg, CONFIG_MODE_POS, CONFIG_MODE_WIDTH);
    // uint8_t read_speed = EXTRACT_FIELD(*reg, CONFIG_SPEED_POS, CONFIG_SPEED_WIDTH);
    // uint8_t read_voltage = EXTRACT_FIELD(*reg, CONFIG_VOLTAGE_POS, CONFIG_VOLTAGE_WIDTH);
    //
    // printf("Verification - Mode: %d, Speed: %d, Voltage: %d\n",
    //        read_mode, read_speed, read_voltage);
}

/**
 * @brief Analyze and decode status register
 * @param status_reg Status register value to analyze
 * @return Number of active status flags
 */
int analyze_status_register(uint32_t status_reg) {
    // TODO: Implement status register analysis
    //
    // Steps to complete:
    // 1. Check each status bit using CHECK_BIT or IS_BIT_SET
    // 2. Count the number of active status flags
    // 3. Print human-readable status information
    // 4. Identify critical vs warning conditions
    // 5. Return total count of active flags
    //
    // Status bits to check:
    // - STATUS_READY_BIT: System ready
    // - STATUS_ERROR_BIT: Error condition
    // - STATUS_BUSY_BIT: System busy
    // - STATUS_OVERTEMP_BIT: Over-temperature
    // - STATUS_UNDERVOLT_BIT: Under-voltage

    printf("TODO: Implement analyze_status_register function\n");
    printf("  - Status register: 0x%08X\n", status_reg);
    printf("  - Decode individual status bits\n");
    printf("  - Count active flags\n");

    int active_flags = 0;

    // Example framework (uncomment and complete):
    // printf("Status Analysis:\n");
    //
    // if (IS_BIT_SET(status_reg, STATUS_READY_BIT)) {
    //     printf("  âœ“ READY: System is ready\n");
    //     active_flags++;
    // } else {
    //     printf("  âœ— READY: System not ready\n");
    // }
    //
    // if (IS_BIT_SET(status_reg, STATUS_ERROR_BIT)) {
    //     printf("  âš  ERROR: Error condition detected\n");
    //     active_flags++;
    // }
    //
    // if (IS_BIT_SET(status_reg, STATUS_BUSY_BIT)) {
    //     printf("  â³ BUSY: System is busy\n");
    //     active_flags++;
    // }
    //
    // if (IS_BIT_SET(status_reg, STATUS_OVERTEMP_BIT)) {
    //     printf("  í ¼í¼¡ï¸ OVERTEMP: Temperature warning\n");
    //     active_flags++;
    // }
    //
    // if (IS_BIT_SET(status_reg, STATUS_UNDERVOLT_BIT)) {
    //     printf("  âš¡ UNDERVOLT: Voltage warning\n");
    //     active_flags++;
    // }
    //
    // printf("Total active flags: %d\n", active_flags);

    return active_flags;
}

/**
 * @brief Implement error register management
 * @param error_reg Pointer to error register
 * @param error_type Type of error to set/clear
 * @param set_error True to set error, false to clear
 */
void manage_error_register(uint32_t *error_reg, int error_type, bool set_error) {
    // TODO: Implement error register management
    //
    // Steps to complete:
    // 1. Validate input parameters
    // 2. Use appropriate bit manipulation based on set_error flag
    // 3. Handle different error types (parity, timeout, overflow, etc.)
    // 4. Implement error priority and masking
    // 5. Log error operations for debugging
    //
    // Error types (from bit_macros.h):
    // - ERROR_PARITY_BIT: Parity error
    // - ERROR_TIMEOUT_BIT: Timeout error
    // - ERROR_OVERFLOW_BIT: Buffer overflow
    // - ERROR_UNDERFLOW_BIT: Buffer underflow
    // - ERROR_CHECKSUM_BIT: Checksum error

    if (error_reg == NULL) {
        printf("ERROR: Error register pointer is NULL\n");
        return;
    }

    printf("TODO: Implement manage_error_register function\n");
    printf("  - Error type: %d\n", error_type);
    printf("  - Operation: %s\n", set_error ? "SET" : "CLEAR");
    printf("  - Current error register: 0x%08X\n", *error_reg);

    // Example framework (uncomment and complete):
    // uint32_t old_value = *error_reg;
    //
    // if (set_error) {
    //     SET_BIT(*error_reg, error_type);
    //     printf("Set error bit %d\n", error_type);
    // } else {
    //     CLEAR_BIT(*error_reg, error_type);
    //     printf("Cleared error bit %d\n", error_type);
    // }
    //
    // printf("Error register: 0x%08X -> 0x%08X\n", old_value, *error_reg);
    //
    // // Count total errors
    // int total_errors = COUNT_ONES(*error_reg);
    // printf("Total active errors: %d\n", total_errors);
}

/**
 * @brief Demonstrate advanced bit manipulation techniques
 */
void demonstrate_advanced_bit_ops(void) {
    // TODO: Implement advanced bit manipulation demonstration
    //
    // Steps to complete:
    // 1. Show bit masking with multiple bits
    // 2. Demonstrate byte manipulation within registers
    // 3. Show endianness conversion
    // 4. Practice bit counting and finding operations
    // 5. Implement bit field validation
    //
    // Advanced techniques to demonstrate:
    // - Multi-bit masks and operations
    // - Byte swapping and endianness
    // - Bit counting (population count)
    // - Finding first/last set bits
    // - Bit field validation and bounds checking

    printf("TODO: Implement demonstrate_advanced_bit_ops function\n");
    printf("  - Show multi-bit masking operations\n");
    printf("  - Demonstrate byte manipulation\n");
    printf("  - Show bit counting and finding\n");

    uint32_t test_reg = 0x12345678;

    // Example framework (uncomment and complete):
    // printf("Advanced Bit Operations Demo:\n");
    // printf("Test register: 0x%08X\n", test_reg);
    //
    // // Multi-bit operations
    // uint32_t mask = MASK_4BIT << 8;  // 4-bit mask at position 8
    // printf("4-bit mask at pos 8: 0x%08X\n", mask);
    //
    // // Byte manipulation
    // printf("Byte 0: 0x%02X\n", GET_BYTE_0(test_reg));
    // printf("Byte 1: 0x%02X\n", GET_BYTE_1(test_reg));
    // printf("Byte 2: 0x%02X\n", GET_BYTE_2(test_reg));
    // printf("Byte 3: 0x%02X\n", GET_BYTE_3(test_reg));
    //
    // // Bit counting
    // printf("Number of 1s: %d\n", COUNT_ONES(test_reg));
    // printf("Number of 0s: %d\n", COUNT_ZEROS(test_reg));
    //
    // // Endianness conversion
    // uint32_t swapped = SWAP_BYTES_32(test_reg);
    // printf("Byte swapped: 0x%08X -> 0x%08X\n", test_reg, swapped);
}

/**
 * @brief Test bit manipulation with ChipState structure
 * @param chip Pointer to ChipState structure
 */
void test_chip_bit_operations(ChipState *chip) {
    // TODO: Implement chip-specific bit operations
    //
    // Steps to complete:
    // 1. Validate input parameter
    // 2. Manipulate control register bits
    // 3. Analyze status register
    // 4. Manage error conditions
    // 5. Update chip state based on bit operations
    //
    // Operations to perform:
    // - Enable/disable chip via control register
    // - Set test mode and debug flags
    // - Simulate error conditions
    // - Check and clear status flags

    if (chip == NULL) {
        printf("ERROR: Chip pointer is NULL\n");
        return;
    }

    printf("TODO: Implement test_chip_bit_operations function\n");
    printf("  - Chip ID: %s\n", chip->chip_id);
    printf("  - Manipulate control register bits\n");
    printf("  - Analyze status and error registers\n");

    // Example framework (uncomment and complete):
    // printf("Testing bit operations on chip: %s\n", chip->chip_id);
    //
    // // Enable the chip
    // SET_BIT(chip->control_reg, CTRL_ENABLE_BIT);
    // printf("Chip enabled - Control: 0x%08X\n", chip->control_reg);
    //
    // // Set test mode
    // SET_BIT(chip->control_reg, CTRL_TEST_MODE_BIT);
    // printf("Test mode enabled - Control: 0x%08X\n", chip->control_reg);
    //
    // // Simulate status conditions
    // SET_BIT(chip->status_reg, STATUS_READY_BIT);
    // SET_BIT(chip->status_reg, STATUS_BUSY_BIT);
    //
    // // Analyze status
    // int flags = analyze_status_register(chip->status_reg);
    // printf("Status analysis complete - %d flags active\n", flags);
    //
    // // Test error management
    // manage_error_register(&chip->error_reg, ERROR_PARITY_BIT, true);
    // manage_error_register(&chip->error_reg, ERROR_TIMEOUT_BIT, true);
}

/**
 * @brief Main function demonstrating Task 3
 */
int main(void) {
    printf("=== Day 3: Bit Operations - Task 3 ===\n\n");

    // Demonstrate basic bit manipulation
    printf("--- Basic Bit Manipulation ---\n");
    demonstrate_basic_bit_ops(0x00000000);

    printf("\n--- Register Field Operations ---\n");
    uint32_t config_reg = 0x12345678;
    configure_register_fields(&config_reg, 5, 2, 12);

    printf("\n--- Status Register Analysis ---\n");
    uint32_t status = 0x0000001B;  // Multiple flags set
    int active_flags = analyze_status_register(status);
    printf("Analysis complete: %d active flags\n", active_flags);

    printf("\n--- Error Register Management ---\n");
    uint32_t error_reg = 0x00000000;
    manage_error_register(&error_reg, ERROR_PARITY_BIT, true);
    manage_error_register(&error_reg, ERROR_TIMEOUT_BIT, true);
    manage_error_register(&error_reg, ERROR_PARITY_BIT, false);

    printf("\n--- Advanced Bit Operations ---\n");
    demonstrate_advanced_bit_ops();

    printf("\n--- Chip Bit Operations Test ---\n");
    ChipState test_chip = {0};
    strncpy(test_chip.chip_id, "BIT_TEST_01", sizeof(test_chip.chip_id) - 1);
    test_chip_bit_operations(&test_chip);

    printf("\n=== Task 3 Complete ===\n");
    printf("Next: Implement multi_chip_test.c for Task 4\n");

    return 0;
}

