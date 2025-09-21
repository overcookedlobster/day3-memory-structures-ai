#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Include chip state structure
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

// Function pointer types for validation strategies
typedef int (*validation_func_t)(const chip_state_t* chip);

// Callback function type for chip events
typedef void (*chip_event_callback_t)(chip_state_t* chip, int event_type);

// Event types
#define EVENT_POWER_ON      1
#define EVENT_POWER_OFF     2
#define EVENT_ERROR         3
#define EVENT_TEMPERATURE   4
#define EVENT_VOLTAGE       5

// Maximum number of callbacks per chip
#define MAX_CALLBACKS 5

// Callback registry structure
typedef struct {
    chip_event_callback_t callbacks[MAX_CALLBACKS];
    int callback_count;
} callback_registry_t;

// Global callback registry
static callback_registry_t g_callback_registry = {0};

/**
 * Validation function: Check power levels
 * @param chip Pointer to chip state
 * @return 1 if valid, 0 if invalid
 */
int validate_power_levels(const chip_state_t* chip) {
    if (chip == NULL) return 0;

    printf("Validating power levels for chip '%s'\n", chip->chip_id);

    // Check voltage range
    if (chip->voltage < 3.0f || chip->voltage > 3.6f) {
        printf("  FAIL: Voltage %.2fV out of range (3.0V - 3.6V)\n", chip->voltage);
        return 0;
    }

    // Check if power enable bit is set when voltage is present
    bool power_enabled = (chip->registers.control_register & (1U << 0)) != 0;
    if (chip->voltage > 2.5f && !power_enabled) {
        printf("  FAIL: Voltage present but power not enabled\n");
        return 0;
    }

    printf("  PASS: Power levels valid\n");
    return 1;
}

/**
 * Validation function: Check temperature range
 * @param chip Pointer to chip state
 * @return 1 if valid, 0 if invalid
 */
int validate_temperature_range(const chip_state_t* chip) {
    if (chip == NULL) return 0;

    printf("Validating temperature range for chip '%s'\n", chip->chip_id);

    // Check operating temperature range
    if (chip->temperature < -40.0f || chip->temperature > 125.0f) {
        printf("  FAIL: Temperature %.1f°C out of absolute range (-40°C to 125°C)\n",
               chip->temperature);
        return 0;
    }

    // Check for thermal warnings
    if (chip->temperature > 85.0f) {
        printf("  WARNING: Temperature %.1f°C above recommended maximum (85°C)\n",
               chip->temperature);
        // Still valid but with warning
    }

    printf("  PASS: Temperature range valid\n");
    return 1;
}

/**
 * Validation function: Check register consistency
 * @param chip Pointer to chip state
 * @return 1 if valid, 0 if invalid
 */
int validate_register_consistency(const chip_state_t* chip) {
    if (chip == NULL) return 0;

    printf("Validating register consistency for chip '%s'\n", chip->chip_id);

    // Check enable/ready bit consistency
    bool enabled = (chip->registers.control_register & (1U << 0)) != 0;
    bool ready = (chip->registers.status_register & (1U << 0)) != 0;
    bool busy = (chip->registers.status_register & (1U << 1)) != 0;

    if (enabled && busy && ready) {
        printf("  FAIL: Cannot be both ready and busy simultaneously\n");
        return 0;
    }

    if (!enabled && ready) {
        printf("  FAIL: Cannot be ready when not enabled\n");
        return 0;
    }

    // Check error register consistency
    bool has_error_bit = (chip->registers.status_register & (1U << 2)) != 0;
    bool has_error_reg = (chip->registers.error_register != 0);

    if (has_error_bit != has_error_reg) {
        printf("  FAIL: Error bit and error register inconsistent\n");
        return 0;
    }

    printf("  PASS: Register consistency valid\n");
    return 1;
}

/**
 * Validation function: Check error states
 * @param chip Pointer to chip state
 * @return 1 if valid, 0 if invalid
 */
int validate_error_states(const chip_state_t* chip) {
    if (chip == NULL) return 0;

    printf("Validating error states for chip '%s'\n", chip->chip_id);

    // Check if error count matches has_errors flag
    if (chip->error_count > 0 && !chip->has_errors) {
        printf("  FAIL: Error count %u but has_errors flag is false\n", chip->error_count);
        return 0;
    }

    if (chip->error_count == 0 && chip->has_errors) {
        printf("  FAIL: No error count but has_errors flag is true\n");
        return 0;
    }

    // Check if error register matches has_errors flag
    if (chip->registers.error_register != 0 && !chip->has_errors) {
        printf("  FAIL: Error register 0x%08X but has_errors flag is false\n",
               chip->registers.error_register);
        return 0;
    }

    printf("  PASS: Error states valid\n");
    return 1;
}

// Array of validation function pointers
validation_func_t validation_strategies[] = {
    validate_power_levels,
    validate_temperature_range,
    validate_register_consistency,
    validate_error_states
};

#define NUM_VALIDATION_STRATEGIES (sizeof(validation_strategies) / sizeof(validation_strategies[0]))

/**
 * Run a specific validation strategy on a chip
 * @param chip Pointer to chip state
 * @param strategy_index Index of validation strategy to run
 * @return 1 if valid, 0 if invalid, -1 if error
 */
int run_validation_strategy(chip_state_t* chip, int strategy_index) {
    if (chip == NULL) {
        printf("Error: NULL chip pointer\n");
        return -1;
    }

    if (strategy_index < 0 || strategy_index >= NUM_VALIDATION_STRATEGIES) {
        printf("Error: Invalid strategy index %d (0-%d)\n",
               strategy_index, (int)NUM_VALIDATION_STRATEGIES - 1);
        return -1;
    }

    printf("\n--- Running Validation Strategy %d ---\n", strategy_index);

    // Call the validation function using function pointer
    validation_func_t validator = validation_strategies[strategy_index];
    int result = validator(chip);

    printf("Strategy %d result: %s\n", strategy_index,
           result ? "PASS" : "FAIL");

    return result;
}

/**
 * Run all validation strategies on a chip
 * @param chip Pointer to chip state
 * @return Number of passed validations
 */
int run_all_validations(chip_state_t* chip) {
    if (chip == NULL) {
        printf("Error: NULL chip pointer\n");
        return -1;
    }

    printf("\n=== Running All Validation Strategies ===\n");
    printf("Chip: %s\n", chip->chip_id);

    int passed_count = 0;

    for (int i = 0; i < NUM_VALIDATION_STRATEGIES; i++) {
        int result = run_validation_strategy(chip, i);
        if (result == 1) {
            passed_count++;
        }
    }

    printf("\n=== Validation Summary ===\n");
    printf("Passed: %d/%d strategies\n", passed_count, (int)NUM_VALIDATION_STRATEGIES);
    printf("Overall result: %s\n",
           (passed_count == NUM_VALIDATION_STRATEGIES) ? "PASS" : "FAIL");

    return passed_count;
}

/**
 * Callback function: Power event handler
 * @param chip Pointer to chip that triggered event
 * @param event_type Type of event
 */
void power_event_callback(chip_state_t* chip, int event_type) {
    if (chip == NULL) return;

    switch (event_type) {
        case EVENT_POWER_ON:
            printf("CALLBACK: Power ON event for chip '%s'\n", chip->chip_id);
            chip->registers.status_register |= (1U << 0);  // Set ready bit
            break;

        case EVENT_POWER_OFF:
            printf("CALLBACK: Power OFF event for chip '%s'\n", chip->chip_id);
            chip->registers.status_register &= ~(1U << 0);  // Clear ready bit
            chip->registers.status_register |= (1U << 1);   // Set busy bit
            break;

        default:
            printf("CALLBACK: Unknown power event %d for chip '%s'\n",
                   event_type, chip->chip_id);
            break;
    }
}

/**
 * Callback function: Error event handler
 * @param chip Pointer to chip that triggered event
 * @param event_type Type of event
 */
void error_event_callback(chip_state_t* chip, int event_type) {
    if (chip == NULL) return;

    if (event_type == EVENT_ERROR) {
        printf("CALLBACK: Error event for chip '%s'\n", chip->chip_id);
        chip->has_errors = true;
        chip->error_count++;
        chip->registers.status_register |= (1U << 2);  // Set error bit
    }
}

/**
 * Callback function: Temperature monitoring
 * @param chip Pointer to chip that triggered event
 * @param event_type Type of event
 */
void temperature_monitor_callback(chip_state_t* chip, int event_type) {
    if (chip == NULL) return;

    if (event_type == EVENT_TEMPERATURE) {
        printf("CALLBACK: Temperature event for chip '%s' (%.1f°C)\n",
               chip->chip_id, chip->temperature);

        if (chip->temperature > 85.0f) {
            printf("  Temperature critical! Initiating thermal protection\n");
            chip->registers.error_register |= (1U << 0);  // Set thermal error bit
            chip->has_errors = true;
            chip->error_count++;
        }
    }
}

/**
 * Register a callback function for chip events
 * @param chip Pointer to chip (not used in this simple implementation)
 * @param callback Callback function to register
 * @return 1 if successful, 0 if failed
 */
int register_chip_callback(chip_state_t* chip, chip_event_callback_t callback) {
    if (callback == NULL) {
        printf("Error: Cannot register NULL callback\n");
        return 0;
    }

    if (g_callback_registry.callback_count >= MAX_CALLBACKS) {
        printf("Error: Callback registry full (%d callbacks)\n", MAX_CALLBACKS);
        return 0;
    }

    g_callback_registry.callbacks[g_callback_registry.callback_count] = callback;
    g_callback_registry.callback_count++;

    printf("Registered callback (Total: %d)\n", g_callback_registry.callback_count);
    return 1;
}

/**
 * Trigger callbacks for a specific event
 * @param chip Pointer to chip that triggered event
 * @param event_type Type of event
 */
void trigger_chip_callbacks(chip_state_t* chip, int event_type) {
    if (chip == NULL) return;

    printf("Triggering callbacks for event %d on chip '%s'\n",
           event_type, chip->chip_id);

    for (int i = 0; i < g_callback_registry.callback_count; i++) {
        if (g_callback_registry.callbacks[i] != NULL) {
            g_callback_registry.callbacks[i](chip, event_type);
        }
    }
}

/**
 * Create an array of chip pointers dynamically
 * @param count Number of chips to create
 * @return Array of chip pointers or NULL if failed
 */
chip_state_t** create_chip_array(int count) {
    if (count <= 0) {
        printf("Error: Invalid chip count %d\n", count);
        return NULL;
    }

    printf("Creating array of %d chip pointers\n", count);

    // Allocate array of pointers
    chip_state_t** chip_array = malloc(count * sizeof(chip_state_t*));
    if (chip_array == NULL) {
        printf("Error: Failed to allocate chip pointer array\n");
        return NULL;
    }

    // Allocate individual chips
    for (int i = 0; i < count; i++) {
        chip_array[i] = malloc(sizeof(chip_state_t));
        if (chip_array[i] == NULL) {
            printf("Error: Failed to allocate chip %d\n", i);

            // Clean up previously allocated chips
            for (int j = 0; j < i; j++) {
                free(chip_array[j]);
            }
            free(chip_array);
            return NULL;
        }

        // Initialize chip
        memset(chip_array[i], 0, sizeof(chip_state_t));
        snprintf(chip_array[i]->chip_id, sizeof(chip_array[i]->chip_id),
                 "CHIP_%d", i);
        snprintf(chip_array[i]->part_number, sizeof(chip_array[i]->part_number),
                 "PART_%d", i + 1000);
        chip_array[i]->serial_number = 100000 + i;
        chip_array[i]->temperature = 25.0f + (i * 5.0f);
        chip_array[i]->voltage = 3.3f;
        chip_array[i]->is_initialized = true;
        chip_array[i]->registers.control_register = 0x00000001;
        chip_array[i]->registers.status_register = 0x80000001;

        printf("  Initialized chip[%d]: %s\n", i, chip_array[i]->chip_id);
    }

    printf("Successfully created chip array\n");
    return chip_array;
}

/**
 * Destroy dynamically allocated chip array
 * @param chips Array of chip pointers
 * @param count Number of chips in array
 */
void destroy_chip_array(chip_state_t** chips, int count) {
    if (chips == NULL) {
        printf("Warning: Attempting to destroy NULL chip array\n");
        return;
    }

    printf("Destroying chip array of %d chips\n", count);

    // Free individual chips
    for (int i = 0; i < count; i++) {
        if (chips[i] != NULL) {
            printf("  Freeing chip[%d]: %s\n", i, chips[i]->chip_id);
            free(chips[i]);
            chips[i] = NULL;
        }
    }

    // Free the array itself
    free(chips);
    printf("Chip array destroyed\n");
}

/**
 * Process chip array using pointer arithmetic
 * @param chips Array of chip pointers
 * @param count Number of chips
 */
void process_chip_array(chip_state_t** chips, int count) {
    if (chips == NULL || count <= 0) {
        printf("Error: Invalid chip array parameters\n");
        return;
    }

    printf("\n=== Processing Chip Array ===\n");

    // Process using array indexing
    printf("Method 1: Array indexing\n");
    for (int i = 0; i < count; i++) {
        if (chips[i] != NULL) {
            printf("  chips[%d]: %s, Temp: %.1f°C\n",
                   i, chips[i]->chip_id, chips[i]->temperature);
        }
    }

    // Process using pointer arithmetic
    printf("\nMethod 2: Pointer arithmetic\n");
    chip_state_t** chip_ptr = chips;
    for (int i = 0; i < count; i++) {
        if (*(chip_ptr + i) != NULL) {
            printf("  *(chip_ptr + %d): %s, Temp: %.1f°C\n",
                   i, (*(chip_ptr + i))->chip_id, (*(chip_ptr + i))->temperature);
        }
    }

    // Find hottest chip using pointers
    printf("\nFinding hottest chip:\n");
    chip_state_t* hottest = NULL;
    float max_temp = -999.0f;

    for (int i = 0; i < count; i++) {
        chip_state_t* current = *(chips + i);
        if (current != NULL && current->temperature > max_temp) {
            max_temp = current->temperature;
            hottest = current;
        }
    }

    if (hottest != NULL) {
        printf("  Hottest chip: %s at %.1f°C\n", hottest->chip_id, hottest->temperature);
    }
}

/**
 * Demonstrate advanced pointer techniques
 */
void demonstrate_advanced_pointers(void) {
    printf("\n=== Advanced Pointer Techniques Demo ===\n");

    // Create test chip
    chip_state_t test_chip;
    memset(&test_chip, 0, sizeof(chip_state_t));
    strcpy(test_chip.chip_id, "ADV_TEST");
    strcpy(test_chip.part_number, "ADV_PART");
    test_chip.serial_number = 555555;
    test_chip.temperature = 45.0f;
    test_chip.voltage = 3.3f;
    test_chip.is_initialized = true;
    test_chip.registers.control_register = 0x00000001;
    test_chip.registers.status_register = 0x80000001;

    printf("\n--- Function Pointer Validation ---\n");
    run_all_validations(&test_chip);

    printf("\n--- Callback Registration and Testing ---\n");
    register_chip_callback(&test_chip, power_event_callback);
    register_chip_callback(&test_chip, error_event_callback);
    register_chip_callback(&test_chip, temperature_monitor_callback);

    // Trigger various events
    trigger_chip_callbacks(&test_chip, EVENT_POWER_ON);
    trigger_chip_callbacks(&test_chip, EVENT_TEMPERATURE);

    // Simulate error condition
    test_chip.temperature = 90.0f;
    trigger_chip_callbacks(&test_chip, EVENT_TEMPERATURE);
    trigger_chip_callbacks(&test_chip, EVENT_ERROR);

    printf("\n--- Dynamic Chip Array Management ---\n");
    chip_state_t** chip_array = create_chip_array(5);
    if (chip_array != NULL) {
        process_chip_array(chip_array, 5);

        // Run validations on all chips in array
        printf("\n--- Array Validation ---\n");
        for (int i = 0; i < 5; i++) {
            printf("\nValidating chip %d:\n", i);
            run_all_validations(chip_array[i]);
        }

        destroy_chip_array(chip_array, 5);
    }

    printf("\n--- Pointer-to-Pointer Operations ---\n");
    chip_state_t* chip_ptr = &test_chip;
    chip_state_t** chip_ptr_ptr = &chip_ptr;

    printf("Direct access: %s\n", test_chip.chip_id);
    printf("Pointer access: %s\n", chip_ptr->chip_id);
    printf("Pointer-to-pointer access: %s\n", (*chip_ptr_ptr)->chip_id);

    // Modify through pointer-to-pointer
    (*chip_ptr_ptr)->temperature = 55.0f;
    printf("Modified temperature via pointer-to-pointer: %.1f°C\n",
           test_chip.temperature);
}

/**
 * Test function pointer arrays and dynamic selection
 */
void test_function_pointer_arrays(void) {
    printf("\n=== Function Pointer Array Testing ===\n");

    // Create test chips with different conditions
    chip_state_t chips[4];

    // Chip 0: Normal operation
    memset(&chips[0], 0, sizeof(chip_state_t));
    strcpy(chips[0].chip_id, "NORMAL");
    chips[0].temperature = 45.0f;
    chips[0].voltage = 3.3f;
    chips[0].is_initialized = true;
    chips[0].registers.control_register = 0x00000001;
    chips[0].registers.status_register = 0x80000001;

    // Chip 1: Power issue
    memset(&chips[1], 0, sizeof(chip_state_t));
    strcpy(chips[1].chip_id, "POWER_ISSUE");
    chips[1].temperature = 50.0f;
    chips[1].voltage = 2.8f;  // Low voltage
    chips[1].is_initialized = true;
    chips[1].registers.control_register = 0x00000001;
    chips[1].registers.status_register = 0x80000001;

    // Chip 2: Temperature issue
    memset(&chips[2], 0, sizeof(chip_state_t));
    strcpy(chips[2].chip_id, "TEMP_ISSUE");
    chips[2].temperature = 95.0f;  // High temperature
    chips[2].voltage = 3.3f;
    chips[2].is_initialized = true;
    chips[2].registers.control_register = 0x00000001;
    chips[2].registers.status_register = 0x80000001;

    // Chip 3: Register inconsistency
    memset(&chips[3], 0, sizeof(chip_state_t));
    strcpy(chips[3].chip_id, "REG_ISSUE");
    chips[3].temperature = 40.0f;
    chips[3].voltage = 3.3f;
    chips[3].is_initialized = true;
    chips[3].registers.control_register = 0x00000000;  // Not enabled
    chips[3].registers.status_register = 0x80000001;   // But ready

    // Test each validation strategy on each chip
    const char* strategy_names[] = {
        "Power Levels",
        "Temperature Range",
        "Register Consistency",
        "Error States"
    };

    for (int chip_idx = 0; chip_idx < 4; chip_idx++) {
        printf("\n--- Testing Chip: %s ---\n", chips[chip_idx].chip_id);

        for (int strategy_idx = 0; strategy_idx < NUM_VALIDATION_STRATEGIES; strategy_idx++) {
            printf("\nStrategy: %s\n", strategy_names[strategy_idx]);
            run_validation_strategy(&chips[chip_idx], strategy_idx);
        }
    }

    // Demonstrate dynamic strategy selection
    printf("\n--- Dynamic Strategy Selection ---\n");
    int user_choice = 2;  // Simulate user choosing strategy 2
    printf("User selected strategy %d: %s\n", user_choice, strategy_names[user_choice]);

    for (int i = 0; i < 4; i++) {
        printf("\nApplying selected strategy to %s:\n", chips[i].chip_id);
        run_validation_strategy(&chips[i], user_choice);
    }
}

