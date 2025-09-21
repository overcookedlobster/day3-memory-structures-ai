#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define REGISTER_BANK_SIZE 256
#define BASE_REGISTER_ADDR 0x40000000

// Simulated register bank (in real hardware, this would be memory-mapped)
static uint32_t register_bank[REGISTER_BANK_SIZE];

/**
 * Get a pointer to a register based on its address
 * @param address The register address
 * @return Pointer to the register or NULL if invalid
 */
uint32_t* get_register_pointer(uint32_t address) {
    // Calculate offset from base address
    uint32_t offset = (address - BASE_REGISTER_ADDR) / sizeof(uint32_t);

    // Validate address range
    if (offset >= REGISTER_BANK_SIZE) {
        printf("Error: Register address 0x%08X out of range\n", address);
        return NULL;
    }

    return &register_bank[offset];
}

/**
 * Read a register value using pointer access
 * @param address The register address
 * @return Register value or 0 if invalid address
 */
uint32_t read_register_via_pointer(uint32_t address) {
    uint32_t* reg_ptr = get_register_pointer(address);

    if (reg_ptr == NULL) {
        printf("Error: Cannot read from invalid register address 0x%08X\n", address);
        return 0;
    }

    return *reg_ptr;
}

/**
 * Write a value to a register using pointer access
 * @param address The register address
 * @param value The value to write
 */
void write_register_via_pointer(uint32_t address, uint32_t value) {
    uint32_t* reg_ptr = get_register_pointer(address);

    if (reg_ptr == NULL) {
        printf("Error: Cannot write to invalid register address 0x%08X\n", address);
        return;
    }

    *reg_ptr = value;
    printf("Register 0x%08X = 0x%08X\n", address, value);
}

/**
 * Validate a register pointer
 * @param reg_ptr Pointer to validate
 * @return 1 if valid, 0 if invalid
 */
int validate_register_pointer(uint32_t* reg_ptr) {
    if (reg_ptr == NULL) {
        return 0;
    }

    // Check if pointer is within register bank bounds
    if (reg_ptr < register_bank || reg_ptr >= (register_bank + REGISTER_BANK_SIZE)) {
        return 0;
    }

    return 1;
}

/**
 * Write multiple values to consecutive registers
 * @param start_addr Starting register address
 * @param values Array of values to write
 * @param count Number of values to write
 */
void bulk_register_write(uint32_t start_addr, uint32_t* values, int count) {
    if (values == NULL || count <= 0) {
        printf("Error: Invalid parameters for bulk write\n");
        return;
    }

    uint32_t* start_ptr = get_register_pointer(start_addr);
    if (start_ptr == NULL) {
        return;
    }

    // Check if all registers in range are valid
    uint32_t end_offset = ((start_addr - BASE_REGISTER_ADDR) / sizeof(uint32_t)) + count - 1;
    if (end_offset >= REGISTER_BANK_SIZE) {
        printf("Error: Bulk write would exceed register bank bounds\n");
        return;
    }

    // Perform bulk write using pointer arithmetic
    for (int i = 0; i < count; i++) {
        *(start_ptr + i) = values[i];
        printf("Register 0x%08X = 0x%08X\n",
               start_addr + (i * sizeof(uint32_t)), values[i]);
    }
}

/**
 * Read multiple values from consecutive registers
 * @param start_addr Starting register address
 * @param buffer Buffer to store read values
 * @param count Number of values to read
 */
void bulk_register_read(uint32_t start_addr, uint32_t* buffer, int count) {
    if (buffer == NULL || count <= 0) {
        printf("Error: Invalid parameters for bulk read\n");
        return;
    }

    uint32_t* start_ptr = get_register_pointer(start_addr);
    if (start_ptr == NULL) {
        return;
    }

    // Check if all registers in range are valid
    uint32_t end_offset = ((start_addr - BASE_REGISTER_ADDR) / sizeof(uint32_t)) + count - 1;
    if (end_offset >= REGISTER_BANK_SIZE) {
        printf("Error: Bulk read would exceed register bank bounds\n");
        return;
    }

    // Perform bulk read using pointer arithmetic
    for (int i = 0; i < count; i++) {
        buffer[i] = *(start_ptr + i);
        printf("Read Register 0x%08X = 0x%08X\n",
               start_addr + (i * sizeof(uint32_t)), buffer[i]);
    }
}

/**
 * Find the first register containing a specific value
 * @param value Value to search for
 * @param start_addr Starting address for search
 * @param range Number of registers to search
 * @return Pointer to first matching register or NULL if not found
 */
uint32_t* find_register_by_value(uint32_t value, uint32_t start_addr, int range) {
    if (range <= 0) {
        printf("Error: Invalid search range\n");
        return NULL;
    }

    uint32_t* start_ptr = get_register_pointer(start_addr);
    if (start_ptr == NULL) {
        return NULL;
    }

    // Check if search range is valid
    uint32_t end_offset = ((start_addr - BASE_REGISTER_ADDR) / sizeof(uint32_t)) + range - 1;
    if (end_offset >= REGISTER_BANK_SIZE) {
        printf("Error: Search range would exceed register bank bounds\n");
        return NULL;
    }

    // Search for value using pointer arithmetic
    for (int i = 0; i < range; i++) {
        if (*(start_ptr + i) == value) {
            uint32_t found_addr = start_addr + (i * sizeof(uint32_t));
            printf("Found value 0x%08X at register 0x%08X\n", value, found_addr);
            return start_ptr + i;
        }
    }

    printf("Value 0x%08X not found in specified range\n", value);
    return NULL;
}

/**
 * Initialize register bank with test pattern
 */
void init_register_bank(void) {
    // Initialize with a test pattern
    for (int i = 0; i < REGISTER_BANK_SIZE; i++) {
        register_bank[i] = 0xDEADBEEF + i;
    }
    printf("Register bank initialized with test pattern\n");
}

/**
 * Demonstrate pointer-based register operations
 */
void demonstrate_pointer_operations(void) {
    printf("\n=== Pointer-Based Register Operations Demo ===\n");

    // Initialize register bank
    init_register_bank();

    // Test basic pointer operations
    uint32_t test_addr = BASE_REGISTER_ADDR + 0x10;
    uint32_t test_value = 0x12345678;

    printf("\n1. Basic pointer operations:\n");
    write_register_via_pointer(test_addr, test_value);
    uint32_t read_value = read_register_via_pointer(test_addr);
    printf("Verification: Read back 0x%08X\n", read_value);

    // Test bulk operations
    printf("\n2. Bulk operations:\n");
    uint32_t bulk_values[] = {0xAAAA0000, 0xBBBB1111, 0xCCCC2222, 0xDDDD3333};
    uint32_t bulk_addr = BASE_REGISTER_ADDR + 0x20;

    bulk_register_write(bulk_addr, bulk_values, 4);

    uint32_t read_buffer[4];
    bulk_register_read(bulk_addr, read_buffer, 4);

    // Test search functionality
    printf("\n3. Register search:\n");
    uint32_t* found_ptr = find_register_by_value(0xBBBB1111, bulk_addr, 4);
    if (found_ptr != NULL) {
        printf("Search successful: pointer = %p\n", (void*)found_ptr);
    }

    // Test error conditions
    printf("\n4. Error handling tests:\n");
    write_register_via_pointer(0xFFFFFFFF, 0x12345678);  // Invalid address
    uint32_t invalid_read = read_register_via_pointer(0x00000000);  // Invalid address
    printf("Invalid read returned: 0x%08X\n", invalid_read);
}

/**
 * Test pointer validation functions
 */
void test_pointer_validation(void) {
    printf("\n=== Pointer Validation Tests ===\n");

    // Test valid pointer
    uint32_t* valid_ptr = get_register_pointer(BASE_REGISTER_ADDR);
    printf("Valid pointer test: %s\n",
           validate_register_pointer(valid_ptr) ? "PASS" : "FAIL");

    // Test NULL pointer
    printf("NULL pointer test: %s\n",
           validate_register_pointer(NULL) ? "FAIL" : "PASS");

    // Test out-of-bounds pointer
    uint32_t dummy_value = 0;
    uint32_t* invalid_ptr = &dummy_value;
    printf("Out-of-bounds pointer test: %s\n",
           validate_register_pointer(invalid_ptr) ? "FAIL" : "PASS");
}

