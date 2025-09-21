#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

// Simple test framework
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

#define TEST_ASSERT(condition, message) \
    do { \
        tests_run++; \
        if (condition) { \
            tests_passed++; \
            printf("  ✓ %s\n", message); \
        } else { \
            tests_failed++; \
            printf("  ✗ %s\n", message); \
        } \
    } while(0)

#define TEST_ASSERT_EQUAL(expected, actual, message) \
    do { \
        tests_run++; \
        if ((expected) == (actual)) { \
            tests_passed++; \
            printf("  ✓ %s (expected: %d, actual: %d)\n", message, (int)(expected), (int)(actual)); \
        } else { \
            tests_failed++; \
            printf("  ✗ %s (expected: %d, actual: %d)\n", message, (int)(expected), (int)(actual)); \
        } \
    } while(0)

#define TEST_ASSERT_NOT_NULL(ptr, message) \
    do { \
        tests_run++; \
        if ((ptr) != NULL) { \
            tests_passed++; \
            printf("  ✓ %s\n", message); \
        } else { \
            tests_failed++; \
            printf("  ✗ %s (pointer is NULL)\n", message); \
        } \
    } while(0)

#define TEST_ASSERT_NULL(ptr, message) \
    do { \
        tests_run++; \
        if ((ptr) == NULL) { \
            tests_passed++; \
            printf("  ✓ %s\n", message); \
        } else { \
            tests_failed++; \
            printf("  ✗ %s (pointer is not NULL)\n", message); \
        } \
    } while(0)

#define TEST_ASSERT_STRING_EQUAL(expected, actual, message) \
    do { \
        tests_run++; \
        if (strcmp((expected), (actual)) == 0) { \
            tests_passed++; \
            printf("  ✓ %s\n", message); \
        } else { \
            tests_failed++; \
            printf("  ✗ %s (expected: '%s', actual: '%s')\n", message, (expected), (actual)); \
        } \
    } while(0)

#define TEST_ASSERT_FLOAT_EQUAL(expected, actual, tolerance, message) \
    do { \
        tests_run++; \
        float diff = (expected) - (actual); \
        if (diff < 0) diff = -diff; \
        if (diff <= (tolerance)) { \
            tests_passed++; \
            printf("  ✓ %s\n", message); \
        } else { \
            tests_failed++; \
            printf("  ✗ %s (expected: %.2f, actual: %.2f, diff: %.2f)\n", \
                   message, (expected), (actual), diff); \
        } \
    } while(0)

void print_test_summary(void) {
    printf("\n=== Test Summary ===\n");
    printf("Tests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_failed);

    if (tests_failed == 0) {
        printf("Result: ALL TESTS PASSED! ✓\n");
    } else {
        printf("Result: %d TESTS FAILED! ✗\n", tests_failed);
    }

    printf("Success rate: %.1f%%\n",
           tests_run > 0 ? (100.0 * tests_passed / tests_run) : 0.0);
}

int get_test_result(void) {
    return tests_failed == 0 ? 0 : 1;
}

// Include chip state definitions
#include "../include/chip_state.h"

// External function declarations
extern uint32_t* get_register_pointer(uint32_t address);
extern uint32_t read_register_via_pointer(uint32_t address);
extern void write_register_via_pointer(uint32_t address, uint32_t value);
extern int validate_register_pointer(uint32_t* reg_ptr);
extern void init_chip_state(chip_state_t* chip, const char* id, const char* part_num);
extern int validate_chip_state(const chip_state_t* chip);
extern void update_chip_temperature(chip_state_t* chip, float new_temp);
extern uint32_t create_test_pattern(int pattern_type);
extern int validate_bit_pattern(uint32_t value, uint32_t expected_pattern, uint32_t mask);
extern void enable_chip_power(chip_state_t* chip);
extern bool is_chip_ready(const chip_state_t* chip);
extern int run_validation_strategy(chip_state_t* chip, int strategy_index);
extern chip_state_t** create_chip_array(int count);
extern void destroy_chip_array(chip_state_t** chips, int count);
extern void* safe_malloc(size_t size, const char* file, int line);
extern void safe_free(void* ptr, const char* file, int line);
extern void init_memory_safety(void);
extern void check_memory_leaks(void);
extern uint32_t calculate_crc32_optimized(const uint8_t* data, size_t length);
extern uint32_t calculate_crc32_naive(const uint8_t* data, size_t length);

// Remove duplicate globals since they're now static in the macros section

/**
 * Test pointer operations
 */
void test_pointer_operations(void) {
    printf("\n--- Testing Pointer Operations ---\n");

    // Test register pointer retrieval
    uint32_t test_addr = 0x40000000;
    uint32_t* reg_ptr = get_register_pointer(test_addr);
    TEST_ASSERT_NOT_NULL(reg_ptr, "Get register pointer returns valid pointer");

    // Test pointer validation
    int valid = validate_register_pointer(reg_ptr);
    TEST_ASSERT_EQUAL(1, valid, "Valid register pointer passes validation");

    // Test NULL pointer validation
    int null_valid = validate_register_pointer(NULL);
    TEST_ASSERT_EQUAL(0, null_valid, "NULL pointer fails validation");

    // Test register write/read
    uint32_t test_value = 0x12345678;
    write_register_via_pointer(test_addr, test_value);
    uint32_t read_value = read_register_via_pointer(test_addr);
    TEST_ASSERT_EQUAL(test_value, read_value, "Register write/read consistency");

    // Test invalid address handling
    uint32_t invalid_addr = 0xFFFFFFFF;
    uint32_t* invalid_ptr = get_register_pointer(invalid_addr);
    TEST_ASSERT_NULL(invalid_ptr, "Invalid address returns NULL pointer");
}

/**
 * Test chip structure operations
 */
void test_chip_structures(void) {
    printf("\n--- Testing Chip Structures ---\n");

    chip_state_t test_chip;
    memset(&test_chip, 0, sizeof(chip_state_t));

    // Test chip initialization
    init_chip_state(&test_chip, "TEST_CHIP", "TEST_PART");
    TEST_ASSERT(test_chip.is_initialized, "Chip initialization sets initialized flag");
    TEST_ASSERT_STRING_EQUAL("TEST_CHIP", test_chip.chip_id, "Chip ID set correctly");
    TEST_ASSERT_STRING_EQUAL("TEST_PART", test_chip.part_number, "Part number set correctly");

    // Test chip validation
    int validation_result = validate_chip_state(&test_chip);
    TEST_ASSERT_EQUAL(1, validation_result, "Valid chip passes validation");

    // Test temperature update
    float original_temp = test_chip.temperature;
    float new_temp = 55.0f;
    update_chip_temperature(&test_chip, new_temp);
    TEST_ASSERT_FLOAT_EQUAL(new_temp, test_chip.temperature, 0.1f, "Temperature update works correctly");

    // Test error conditions
    test_chip.temperature = 95.0f;  // Overheating
    update_chip_temperature(&test_chip, test_chip.temperature);
    TEST_ASSERT(test_chip.has_errors, "Overheating sets error flag");
    TEST_ASSERT(test_chip.error_count > 0, "Overheating increments error count");

    // Test NULL pointer handling
    init_chip_state(NULL, "TEST", "TEST");  // Should not crash
    int null_validation = validate_chip_state(NULL);
    TEST_ASSERT_EQUAL(0, null_validation, "NULL chip fails validation");
}

/**
 * Test bit manipulation operations
 */
void test_bit_operations(void) {
    printf("\n--- Testing Bit Operations ---\n");

    // Test pattern creation
    uint32_t pattern = create_test_pattern(0);  // Alternating pattern
    TEST_ASSERT_EQUAL(0x55555555, pattern, "Alternating pattern created correctly");

    pattern = create_test_pattern(1);  // Inverse alternating
    TEST_ASSERT_EQUAL(0xAAAAAAAA, pattern, "Inverse alternating pattern created correctly");

    // Test pattern validation
    uint32_t test_value = 0x55555555;
    uint32_t expected = 0x55555555;
    uint32_t mask = 0xFFFFFFFF;
    int valid = validate_bit_pattern(test_value, expected, mask);
    TEST_ASSERT_EQUAL(1, valid, "Matching pattern validates correctly");

    // Test pattern mismatch
    valid = validate_bit_pattern(test_value, ~expected, mask);
    TEST_ASSERT_EQUAL(0, valid, "Non-matching pattern fails validation");

    // Test chip power operations
    chip_state_t test_chip;
    memset(&test_chip, 0, sizeof(chip_state_t));
    strcpy(test_chip.chip_id, "BIT_TEST");
    test_chip.is_initialized = true;

    enable_chip_power(&test_chip);
    bool ready = is_chip_ready(&test_chip);
    TEST_ASSERT(ready, "Chip is ready after power enable");

    // Check control register enable bit
    bool enabled = (test_chip.registers.control_register & 0x1) != 0;
    TEST_ASSERT(enabled, "Enable bit set in control register");
}

/**
 * Test advanced pointer techniques
 */
void test_advanced_pointers(void) {
    printf("\n--- Testing Advanced Pointers ---\n");

    // Test chip array creation
    int chip_count = 3;
    chip_state_t** chip_array = create_chip_array(chip_count);
    TEST_ASSERT_NOT_NULL(chip_array, "Chip array creation succeeds");

    if (chip_array != NULL) {
        // Test array contents
        for (int i = 0; i < chip_count; i++) {
            TEST_ASSERT_NOT_NULL(chip_array[i], "Chip array element is not NULL");
            if (chip_array[i] != NULL) {
                TEST_ASSERT(chip_array[i]->is_initialized, "Chip array element is initialized");
            }
        }

        // Test validation strategies
        if (chip_array[0] != NULL) {
            int validation_result = run_validation_strategy(chip_array[0], 0);
            TEST_ASSERT(validation_result >= 0, "Validation strategy runs without error");
        }

        // Clean up
        destroy_chip_array(chip_array, chip_count);
    }

    // Test invalid parameters
    chip_state_t** null_array = create_chip_array(0);
    TEST_ASSERT_NULL(null_array, "Zero count returns NULL array");

    chip_state_t** negative_array = create_chip_array(-1);
    TEST_ASSERT_NULL(negative_array, "Negative count returns NULL array");
}

/**
 * Test memory safety features
 */
void test_memory_safety(void) {
    printf("\n--- Testing Memory Safety ---\n");

    // Initialize memory safety system
    init_memory_safety();

    // Test safe malloc
    void* ptr1 = safe_malloc(100, __FILE__, __LINE__);
    TEST_ASSERT_NOT_NULL(ptr1, "Safe malloc returns valid pointer");

    void* ptr2 = safe_malloc(200, __FILE__, __LINE__);
    TEST_ASSERT_NOT_NULL(ptr2, "Second safe malloc returns valid pointer");

    // Test safe free
    safe_free(ptr1, __FILE__, __LINE__);
    safe_free(ptr2, __FILE__, __LINE__);

    // Test zero size allocation
    void* zero_ptr = safe_malloc(0, __FILE__, __LINE__);
    TEST_ASSERT_NULL(zero_ptr, "Zero size allocation returns NULL");

    // Test NULL free (should not crash)
    safe_free(NULL, __FILE__, __LINE__);

    // Check for leaks
    check_memory_leaks();
}

/**
 * Test AI optimization features
 */
void test_ai_optimizations(void) {
    printf("\n--- Testing AI Optimizations ---\n");

    // Test CRC32 calculation
    uint8_t test_data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    size_t data_size = sizeof(test_data);

    uint32_t crc_optimized = calculate_crc32_optimized(test_data, data_size);
    uint32_t crc_naive = calculate_crc32_naive(test_data, data_size);

    TEST_ASSERT_EQUAL(crc_optimized, crc_naive, "Optimized and naive CRC32 produce same result");

    // Test with NULL data
    uint32_t crc_null = calculate_crc32_optimized(NULL, data_size);
    TEST_ASSERT_EQUAL(0, crc_null, "NULL data returns zero CRC");

    // Test with zero length
    uint32_t crc_zero = calculate_crc32_optimized(test_data, 0);
    TEST_ASSERT_EQUAL(0, crc_zero, "Zero length returns zero CRC");

    // Test with larger data set
    uint8_t large_data[1000];
    for (int i = 0; i < 1000; i++) {
        large_data[i] = i & 0xFF;
    }

    uint32_t crc_large_opt = calculate_crc32_optimized(large_data, 1000);
    uint32_t crc_large_naive = calculate_crc32_naive(large_data, 1000);
    TEST_ASSERT_EQUAL(crc_large_opt, crc_large_naive, "Large data CRC consistency");
}

/**
 * Test error handling and edge cases
 */
void test_error_handling(void) {
    printf("\n--- Testing Error Handling ---\n");

    // Test NULL pointer handling in various functions
    chip_state_t* null_chip = NULL;

    // These should not crash
    update_chip_temperature(null_chip, 50.0f);
    enable_chip_power(null_chip);
    bool ready = is_chip_ready(null_chip);
    TEST_ASSERT(!ready, "NULL chip is not ready");

    // Test invalid validation strategy
    chip_state_t test_chip;
    memset(&test_chip, 0, sizeof(chip_state_t));
    init_chip_state(&test_chip, "ERROR_TEST", "ERROR_PART");

    int invalid_strategy = run_validation_strategy(&test_chip, 999);
    TEST_ASSERT_EQUAL(-1, invalid_strategy, "Invalid strategy index returns error");

    // Test boundary conditions
    uint32_t boundary_pattern = create_test_pattern(999);  // Invalid pattern type
    TEST_ASSERT_EQUAL(0xDEADBEEF, boundary_pattern, "Invalid pattern type returns default");
}

/**
 * Test integration scenarios
 */
void test_integration(void) {
    printf("\n--- Testing Integration Scenarios ---\n");

    // Create a complete chip system
    chip_state_t test_chip;
    memset(&test_chip, 0, sizeof(chip_state_t));

    // Initialize chip
    init_chip_state(&test_chip, "INTEGRATION", "INT_PART");
    TEST_ASSERT(test_chip.is_initialized, "Integration chip initialized");

    // Enable power
    enable_chip_power(&test_chip);
    TEST_ASSERT(is_chip_ready(&test_chip), "Integration chip ready after power on");

    // Update temperature
    update_chip_temperature(&test_chip, 45.0f);
    TEST_ASSERT_FLOAT_EQUAL(45.0f, test_chip.temperature, 0.1f, "Integration temperature update");

    // Validate chip state
    int validation = validate_chip_state(&test_chip);
    TEST_ASSERT_EQUAL(1, validation, "Integration chip passes validation");

    // Test with memory allocation
    init_memory_safety();
    void* test_buffer = safe_malloc(256, __FILE__, __LINE__);
    TEST_ASSERT_NOT_NULL(test_buffer, "Integration memory allocation succeeds");

    if (test_buffer != NULL) {
        // Use the buffer
        memset(test_buffer, 0xAA, 256);
        safe_free(test_buffer, __FILE__, __LINE__);
    }

    check_memory_leaks();
}

/**
 * Main test runner
 */
int main(void) {
    printf("=== Day 3 Reference Solution Test Suite ===\n");
    printf("Testing Memory Management and Data Structures\n");

    // Initialize random seed for consistent testing
    srand(12345);

    // Run all test suites
    test_pointer_operations();
    test_chip_structures();
    test_bit_operations();
    test_advanced_pointers();
    test_memory_safety();
    test_ai_optimizations();
    test_error_handling();
    test_integration();

    // Print summary and return result
    print_test_summary();
    return get_test_result();
}

