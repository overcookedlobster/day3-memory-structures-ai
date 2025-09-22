/**
 * Day 3: Memory Management and Data Structures - Bit Operations
 *
 * Student Name: ___________________
 * Date: ___________________
 *
 * INSTRUCTIONS:
 * This file contains skeleton implementations for Task 3: Bit Manipulation.
 * Complete each TODO section to implement the required functionality.
 *
 * Task 3: Bit Manipulation (15 points)
 *
 * Read the comments carefully and implement each function step by step.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "bit_macros.h"
#include "chip_state.h"

/**
 * Task 3: Bit Manipulation Operations (50 minutes)
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
    //
    // HINT: Use the macros defined in bit_macros.h
    // HINT: Print binary representation to visualize changes

    uint32_t reg = initial_value;
    (void)reg; // Suppress unused variable warning

    printf("TODO: Implement demonstrate_basic_bit_ops function\n");
    printf("  - Initial value: 0x%08X\n", initial_value);
    printf("  - Demonstrate SET_BIT, CLEAR_BIT, TOGGLE_BIT operations\n");
    printf("  - Show register value after each operation\n");

    // Example framework (uncomment and complete):
    // printf("Initial register: 0x%08X\n", reg);
    //
    // SET_BIT(reg, 0);  // Set enable bit
    // printf("After SET_BIT(0): 0x%08X\n", reg);
    //
    // CLEAR_BIT(reg, 1);  // Clear reset bit
    // printf("After CLEAR_BIT(1): 0x%08X\n", reg);
    //
    // Add more operations here...
}

/**
 * @brief Extract a multi-bit field from a register
 * @param reg_value Register value to extract from
 * @param start_bit Starting bit position (0-based)
 * @param num_bits Number of bits in the field
 * @return Extracted field value
 */
uint32_t extract_bit_field(uint32_t reg_value, int start_bit, int num_bits) {
    // TODO: Implement bit field extraction
    //
    // Steps to complete:
    // 1. Validate input parameters (start_bit and num_bits)
    // 2. Create a mask with the appropriate number of bits
    // 3. Shift the register value to align the field
    // 4. Apply the mask to extract only the desired bits
    // 5. Return the extracted value
    //
    // Formula: ((reg_value >> start_bit) & ((1U << num_bits) - 1))
    //
    // HINT: (1U << num_bits) - 1 creates a mask with num_bits set to 1
    // HINT: Always validate that start_bit + num_bits <= 32

    if (start_bit < 0 || num_bits <= 0 || start_bit + num_bits > 32) {
        printf("ERROR: Invalid bit field parameters\n");
        return 0;
    }

    printf("TODO: Implement extract_bit_field function\n");
    printf("  - Register: 0x%08X, Start: %d, Bits: %d\n", reg_value, start_bit, num_bits);

    return 0; // Replace with actual implementation
}

/**
 * @brief Insert a value into a multi-bit field in a register
 * @param reg_value Original register value
 * @param field_value Value to insert
 * @param start_bit Starting bit position (0-based)
 * @param num_bits Number of bits in the field
 * @return Updated register value
 */
uint32_t insert_bit_field(uint32_t reg_value, uint32_t field_value, int start_bit, int num_bits) {
    // TODO: Implement bit field insertion
    //
    // Steps to complete:
    // 1. Validate input parameters
    // 2. Create a mask for the field
    // 3. Clear the target field in the register
    // 4. Mask the field_value to ensure it fits
    // 5. Shift the field_value to the correct position
    // 6. OR the shifted value with the cleared register
    // 7. Return the updated register value
    //
    // Algorithm:
    // - mask = ((1U << num_bits) - 1) << start_bit
    // - reg_cleared = reg_value & ~mask
    // - field_masked = (field_value & ((1U << num_bits) - 1)) << start_bit
    // - result = reg_cleared | field_masked

    if (start_bit < 0 || num_bits <= 0 || start_bit + num_bits > 32) {
        printf("ERROR: Invalid bit field parameters\n");
        return reg_value;
    }

    printf("TODO: Implement insert_bit_field function\n");
    printf("  - Register: 0x%08X, Field: 0x%08X, Start: %d, Bits: %d\n",
           reg_value, field_value, start_bit, num_bits);

    return reg_value; // Replace with actual implementation
}

/**
 * @brief Manage status flags in a register using bit operations
 * @param status_reg Pointer to status register
 * @param flag_bit Bit position of the flag
 * @param set_flag True to set flag, false to clear
 */
void manage_status_flag(uint32_t *status_reg, int flag_bit, bool set_flag) {
    // TODO: Implement status flag management
    //
    // Steps to complete:
    // 1. Validate input parameters (check for NULL pointer)
    // 2. Use appropriate bit operation based on set_flag
    // 3. If set_flag is true, use SET_BIT
    // 4. If set_flag is false, use CLEAR_BIT
    // 5. Print the operation performed
    //
    // HINT: Use the macros from bit_macros.h
    // HINT: Always validate pointers before dereferencing

    if (status_reg == NULL) {
        printf("ERROR: Invalid status register pointer\n");
        return;
    }

    if (flag_bit < 0 || flag_bit >= 32) {
        printf("ERROR: Invalid flag bit position: %d\n", flag_bit);
        return;
    }

    printf("TODO: Implement manage_status_flag function\n");
    printf("  - Register: 0x%08X, Bit: %d, Set: %s\n",
           *status_reg, flag_bit, set_flag ? "true" : "false");

    // Example framework (uncomment and complete):
    // if (set_flag) {
    //     SET_BIT(*status_reg, flag_bit);
    //     printf("Set flag bit %d\n", flag_bit);
    // } else {
    //     CLEAR_BIT(*status_reg, flag_bit);
    //     printf("Cleared flag bit %d\n", flag_bit);
    // }
}

/**
 * @brief Demonstrate advanced bit manipulation techniques
 */
void demonstrate_advanced_bit_ops(void) {
    // TODO: Implement advanced bit operation demonstration
    //
    // Steps to complete:
    // 1. Create a test register with known bit patterns
    // 2. Demonstrate field extraction with different field sizes
    // 3. Show field insertion with various values
    // 4. Practice bit rotation and shifting operations
    // 5. Implement bit counting (population count)
    // 6. Show practical examples like parsing configuration registers
    //
    // Advanced techniques to demonstrate:
    // - Multi-bit field operations
    // - Bit rotation (left and right)
    // - Population count (count set bits)
    // - Find first set bit
    // - Reverse bit order

    uint32_t test_reg = 0x12345678;
    (void)test_reg; // Suppress unused variable warning

    printf("TODO: Implement demonstrate_advanced_bit_ops function\n");
    printf("  - Show field extraction and insertion\n");
    printf("  - Demonstrate bit rotation and shifting\n");
    printf("  - Implement bit counting operations\n");

    // Example framework (uncomment and complete):
    // printf("Test register: 0x%08X\n", test_reg);
    //
    // // Extract 4-bit field starting at bit 8
    // uint32_t field = extract_bit_field(test_reg, 8, 4);
    // printf("Extracted field (bits 8-11): 0x%X\n", field);
    //
    // // Insert new value into field
    // test_reg = insert_bit_field(test_reg, 0xA, 8, 4);
    // printf("After field insertion: 0x%08X\n", test_reg);
}

/**
 * @brief Apply bit operations to ChipState structure
 * @param chip Pointer to ChipState structure
 */
void chip_bit_operations(ChipState *chip) {
    // TODO: Implement chip-specific bit operations
    //
    // Steps to complete:
    // 1. Validate input parameter
    // 2. Use bit operations on chip registers
    // 3. Set/clear specific control bits
    // 4. Extract status information from status register
    // 5. Update error flags using bit manipulation
    // 6. Demonstrate practical register manipulation
    //
    // Example operations:
    // - Set enable bit in control register
    // - Clear reset bit
    // - Extract temperature sensor status
    // - Set error flags based on conditions

    if (chip == NULL) {
        printf("ERROR: Invalid chip pointer\n");
        return;
    }

    printf("TODO: Implement chip_bit_operations function\n");
    printf("  - Chip: %s\n", chip->chip_id);
    printf("  - Apply bit operations to chip registers\n");
    printf("  - Demonstrate practical register manipulation\n");

    // Example framework (uncomment and complete):
    // printf("Before bit operations:\n");
    // printf("  Control: 0x%08X, Status: 0x%08X, Error: 0x%08X\n",
    //        chip->control_reg, chip->status_reg, chip->error_reg);
    //
    // // Set enable bit (bit 0) in control register
    // SET_BIT(chip->control_reg, 0);
    //
    // // Clear reset bit (bit 1) in control register
    // CLEAR_BIT(chip->control_reg, 1);
    //
    // // Extract status field (bits 4-7) from status register
    // uint32_t status_field = extract_bit_field(chip->status_reg, 4, 4);
    // printf("Status field: 0x%X\n", status_field);
    //
    // printf("After bit operations:\n");
    // printf("  Control: 0x%08X, Status: 0x%08X, Error: 0x%08X\n",
    //        chip->control_reg, chip->status_reg, chip->error_reg);
}

/**
 * @brief Main function to demonstrate bit manipulation operations
 */
int main(void) {
    printf("=== Day 3: Bit Operations - Task 3 ===\n\n");

    // Test basic bit operations
    printf("Testing Basic Bit Operations\n");
    printf("----------------------------\n");
    demonstrate_basic_bit_ops(0x12345678);
    printf("\n");

    // Test bit field operations
    printf("Testing Bit Field Operations\n");
    printf("----------------------------\n");
    uint32_t test_reg = 0xABCDEF00;

    printf("Testing field extraction...\n");
    uint32_t extracted = extract_bit_field(test_reg, 8, 8);
    printf("Expected vs Actual: 0xEF vs 0x%02X\n", extracted);

    printf("Testing field insertion...\n");
    uint32_t updated = insert_bit_field(test_reg, 0x42, 8, 8);
    printf("Original: 0x%08X, Updated: 0x%08X\n", test_reg, updated);
    printf("\n");

    // Test status flag management
    printf("Testing Status Flag Management\n");
    printf("------------------------------\n");
    uint32_t status = 0x00000000;
    printf("Initial status: 0x%08X\n", status);

    manage_status_flag(&status, 0, true);   // Set bit 0
    manage_status_flag(&status, 4, true);   // Set bit 4
    manage_status_flag(&status, 0, false);  // Clear bit 0

    printf("Final status: 0x%08X\n", status);
    printf("\n");

    // Test advanced operations
    printf("Testing Advanced Bit Operations\n");
    printf("-------------------------------\n");
    demonstrate_advanced_bit_ops();
    printf("\n");

    // Test with ChipState structure
    printf("Testing Chip Bit Operations\n");
    printf("---------------------------\n");
    ChipState test_chip;
    strncpy(test_chip.chip_id, "BIT_TEST_01", sizeof(test_chip.chip_id) - 1);
    test_chip.chip_id[sizeof(test_chip.chip_id) - 1] = '\0';
    test_chip.control_reg = 0x00000000;
    test_chip.status_reg = 0x12345678;
    test_chip.error_reg = 0x00000000;
    test_chip.temperature = 25.0f;
    test_chip.is_active = true;

    chip_bit_operations(&test_chip);

    printf("\n=== Task 3 Completion Status ===\n");
    printf("Review your implementations above for correctness.\n");
    printf("Next: Continue to multi_chip_test.c for Task 4\n");

    return 0;
}

