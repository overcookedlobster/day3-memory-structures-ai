#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_CHIPS 16
#define MAX_ERROR_LOG 100

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

typedef struct {
    chip_state_t chips[MAX_CHIPS];
    int active_chip_count;
    int total_error_count;
    float average_temperature;
    char system_status[64];
} system_state_t;

// Global system state
static system_state_t g_system;

/**
 * Initialize a chip state structure
 * @param chip Pointer to chip state structure
 * @param id Chip identifier string
 * @param part_num Part number string
 */
void init_chip_state(chip_state_t* chip, const char* id, const char* part_num) {
    if (chip == NULL || id == NULL || part_num == NULL) {
        printf("Error: NULL pointer passed to init_chip_state\n");
        return;
    }

    // Clear the entire structure
    memset(chip, 0, sizeof(chip_state_t));

    // Set string fields with bounds checking
    strncpy(chip->chip_id, id, sizeof(chip->chip_id) - 1);
    chip->chip_id[sizeof(chip->chip_id) - 1] = '\0';

    strncpy(chip->part_number, part_num, sizeof(chip->part_number) - 1);
    chip->part_number[sizeof(chip->part_number) - 1] = '\0';

    // Generate a pseudo-random serial number
    chip->serial_number = (uint32_t)(rand() % 1000000 + 100000);

    // Initialize default values
    chip->temperature = 25.0f;  // Room temperature
    chip->voltage = 3.3f;       // Standard logic voltage
    chip->is_initialized = true;
    chip->has_errors = false;
    chip->error_count = 0;
    chip->uptime_seconds = 0;

    // Initialize registers with default values
    chip->registers.control_register = 0x00000001;  // Enable bit set
    chip->registers.status_register = 0x80000000;   // Ready bit set
    chip->registers.error_register = 0x00000000;    // No errors
    chip->registers.config_register = 0x12345678;   // Default config

    printf("Initialized chip '%s' (Part: %s, Serial: %u)\n",
           chip->chip_id, chip->part_number, chip->serial_number);
}

/**
 * Update chip temperature and check for thermal issues
 * @param chip Pointer to chip state structure
 * @param new_temp New temperature value in Celsius
 */
void update_chip_temperature(chip_state_t* chip, float new_temp) {
    if (chip == NULL) {
        printf("Error: NULL chip pointer in update_chip_temperature\n");
        return;
    }

    if (!chip->is_initialized) {
        printf("Error: Chip '%s' not initialized\n", chip->chip_id);
        return;
    }

    float old_temp = chip->temperature;
    chip->temperature = new_temp;

    // Check for thermal warnings
    if (new_temp > 85.0f) {
        printf("WARNING: Chip '%s' overheating! Temp: %.1f°C\n",
               chip->chip_id, new_temp);
        chip->has_errors = true;
        chip->error_count++;
        chip->registers.error_register |= 0x00000001;  // Thermal error bit
    } else if (new_temp < -40.0f) {
        printf("WARNING: Chip '%s' too cold! Temp: %.1f°C\n",
               chip->chip_id, new_temp);
        chip->has_errors = true;
        chip->error_count++;
        chip->registers.error_register |= 0x00000002;  // Cold error bit
    } else {
        // Clear thermal error bits if temperature is normal
        chip->registers.error_register &= ~0x00000003;
    }

    printf("Chip '%s' temperature: %.1f°C -> %.1f°C\n",
           chip->chip_id, old_temp, new_temp);
}

/**
 * Update chip register set
 * @param chip Pointer to chip state structure
 * @param new_regs Pointer to new register values
 */
void update_chip_registers(chip_state_t* chip, register_set_t* new_regs) {
    if (chip == NULL || new_regs == NULL) {
        printf("Error: NULL pointer in update_chip_registers\n");
        return;
    }

    if (!chip->is_initialized) {
        printf("Error: Chip '%s' not initialized\n", chip->chip_id);
        return;
    }

    // Save old values for comparison
    register_set_t old_regs = chip->registers;

    // Update registers
    chip->registers = *new_regs;

    // Check for error conditions in new registers
    if (new_regs->error_register != 0) {
        chip->has_errors = true;
        chip->error_count++;
    }

    printf("Chip '%s' registers updated:\n", chip->chip_id);
    printf("  Control: 0x%08X -> 0x%08X\n",
           old_regs.control_register, new_regs->control_register);
    printf("  Status:  0x%08X -> 0x%08X\n",
           old_regs.status_register, new_regs->status_register);
    printf("  Error:   0x%08X -> 0x%08X\n",
           old_regs.error_register, new_regs->error_register);
    printf("  Config:  0x%08X -> 0x%08X\n",
           old_regs.config_register, new_regs->config_register);
}

/**
 * Validate chip state for consistency and errors
 * @param chip Pointer to chip state structure
 * @return 1 if valid, 0 if invalid
 */
int validate_chip_state(const chip_state_t* chip) {
    if (chip == NULL) {
        printf("Error: NULL chip pointer in validate_chip_state\n");
        return 0;
    }

    int validation_errors = 0;

    // Check initialization status
    if (!chip->is_initialized) {
        printf("Validation Error: Chip '%s' not initialized\n", chip->chip_id);
        validation_errors++;
    }

    // Validate temperature range
    if (chip->temperature < -55.0f || chip->temperature > 125.0f) {
        printf("Validation Error: Chip '%s' temperature out of range: %.1f°C\n",
               chip->chip_id, chip->temperature);
        validation_errors++;
    }

    // Validate voltage range
    if (chip->voltage < 1.8f || chip->voltage > 5.5f) {
        printf("Validation Error: Chip '%s' voltage out of range: %.2fV\n",
               chip->chip_id, chip->voltage);
        validation_errors++;
    }

    // Check for empty chip ID
    if (strlen(chip->chip_id) == 0) {
        printf("Validation Error: Empty chip ID\n");
        validation_errors++;
    }

    // Check for empty part number
    if (strlen(chip->part_number) == 0) {
        printf("Validation Error: Empty part number\n");
        validation_errors++;
    }

    // Validate serial number (should not be zero)
    if (chip->serial_number == 0) {
        printf("Validation Error: Invalid serial number\n");
        validation_errors++;
    }

    // Check register consistency
    if (chip->has_errors && chip->registers.error_register == 0) {
        printf("Validation Warning: Chip '%s' has_errors flag set but error register is clear\n",
               chip->chip_id);
    }

    if (validation_errors == 0) {
        printf("Chip '%s' validation: PASS\n", chip->chip_id);
        return 1;
    } else {
        printf("Chip '%s' validation: FAIL (%d errors)\n",
               chip->chip_id, validation_errors);
        return 0;
    }
}

/**
 * Print a comprehensive summary of chip state
 * @param chip Pointer to chip state structure
 */
void print_chip_summary(const chip_state_t* chip) {
    if (chip == NULL) {
        printf("Error: NULL chip pointer in print_chip_summary\n");
        return;
    }

    printf("\n=== Chip Summary ===\n");
    printf("Chip ID:      %s\n", chip->chip_id);
    printf("Part Number:  %s\n", chip->part_number);
    printf("Serial:       %u\n", chip->serial_number);
    printf("Temperature:  %.1f°C\n", chip->temperature);
    printf("Voltage:      %.2fV\n", chip->voltage);
    printf("Initialized:  %s\n", chip->is_initialized ? "Yes" : "No");
    printf("Has Errors:   %s\n", chip->has_errors ? "Yes" : "No");
    printf("Error Count:  %u\n", chip->error_count);
    printf("Uptime:       %llu seconds\n", (unsigned long long)chip->uptime_seconds);

    printf("\nRegisters:\n");
    printf("  Control: 0x%08X\n", chip->registers.control_register);
    printf("  Status:  0x%08X\n", chip->registers.status_register);
    printf("  Error:   0x%08X\n", chip->registers.error_register);
    printf("  Config:  0x%08X\n", chip->registers.config_register);
    printf("==================\n\n");
}

/**
 * Initialize the system state
 */
void init_system_state(void) {
    memset(&g_system, 0, sizeof(system_state_t));
    g_system.active_chip_count = 0;
    g_system.total_error_count = 0;
    g_system.average_temperature = 0.0f;
    strcpy(g_system.system_status, "SYSTEM_IDLE");

    printf("System state initialized\n");
}

/**
 * Add a chip to the system
 * @param chip Pointer to initialized chip
 * @return 1 if successful, 0 if failed
 */
int add_chip_to_system(chip_state_t* chip) {
    if (chip == NULL) {
        printf("Error: Cannot add NULL chip to system\n");
        return 0;
    }

    if (g_system.active_chip_count >= MAX_CHIPS) {
        printf("Error: System at maximum chip capacity (%d)\n", MAX_CHIPS);
        return 0;
    }

    if (!chip->is_initialized) {
        printf("Error: Cannot add uninitialized chip to system\n");
        return 0;
    }

    // Copy chip to system array
    g_system.chips[g_system.active_chip_count] = *chip;
    g_system.active_chip_count++;

    printf("Added chip '%s' to system (Total: %d chips)\n",
           chip->chip_id, g_system.active_chip_count);

    update_system_statistics();
    return 1;
}

/**
 * Update system-wide statistics
 */
void update_system_statistics(void) {
    if (g_system.active_chip_count == 0) {
        g_system.average_temperature = 0.0f;
        g_system.total_error_count = 0;
        strcpy(g_system.system_status, "NO_CHIPS");
        return;
    }

    float temp_sum = 0.0f;
    int total_errors = 0;
    bool system_has_errors = false;

    for (int i = 0; i < g_system.active_chip_count; i++) {
        temp_sum += g_system.chips[i].temperature;
        total_errors += g_system.chips[i].error_count;

        if (g_system.chips[i].has_errors) {
            system_has_errors = true;
        }
    }

    g_system.average_temperature = temp_sum / g_system.active_chip_count;
    g_system.total_error_count = total_errors;

    // Update system status
    if (system_has_errors) {
        strcpy(g_system.system_status, "SYSTEM_ERROR");
    } else if (g_system.average_temperature > 70.0f) {
        strcpy(g_system.system_status, "SYSTEM_HOT");
    } else {
        strcpy(g_system.system_status, "SYSTEM_OK");
    }

    printf("System statistics updated: %d chips, %.1f°C avg, %d total errors, Status: %s\n",
           g_system.active_chip_count, g_system.average_temperature,
           g_system.total_error_count, g_system.system_status);
}

/**
 * Print system summary
 */
void print_system_summary(void) {
    printf("\n=== System Summary ===\n");
    printf("Active Chips:     %d / %d\n", g_system.active_chip_count, MAX_CHIPS);
    printf("Average Temp:     %.1f°C\n", g_system.average_temperature);
    printf("Total Errors:     %d\n", g_system.total_error_count);
    printf("System Status:    %s\n", g_system.system_status);

    printf("\nChip Details:\n");
    for (int i = 0; i < g_system.active_chip_count; i++) {
        printf("  [%d] %s: %.1f°C, %s\n",
               i, g_system.chips[i].chip_id,
               g_system.chips[i].temperature,
               g_system.chips[i].has_errors ? "ERROR" : "OK");
    }
    printf("=====================\n\n");
}

/**
 * Demonstrate chip structure operations
 */
void demonstrate_chip_structures(void) {
    printf("\n=== Chip Structure Operations Demo ===\n");

    // Initialize system
    init_system_state();

    // Create and initialize test chips
    chip_state_t chip1, chip2, chip3;

    init_chip_state(&chip1, "CPU_CORE_0", "ARM_CORTEX_A78");
    init_chip_state(&chip2, "GPU_CORE_0", "MALI_G78_MP14");
    init_chip_state(&chip3, "DSP_CORE_0", "HEXAGON_V68");

    // Add chips to system
    add_chip_to_system(&chip1);
    add_chip_to_system(&chip2);
    add_chip_to_system(&chip3);

    // Test temperature updates
    printf("\n--- Temperature Updates ---\n");
    update_chip_temperature(&chip1, 45.5f);
    update_chip_temperature(&chip2, 78.2f);
    update_chip_temperature(&chip3, 92.1f);  // This should trigger warning

    // Test register updates
    printf("\n--- Register Updates ---\n");
    register_set_t new_regs = {
        .control_register = 0x0000000F,
        .status_register = 0x80000001,
        .error_register = 0x00000000,
        .config_register = 0xABCDEF00
    };
    update_chip_registers(&chip2, &new_regs);

    // Validate all chips
    printf("\n--- Chip Validation ---\n");
    validate_chip_state(&chip1);
    validate_chip_state(&chip2);
    validate_chip_state(&chip3);

    // Print individual summaries
    print_chip_summary(&chip1);
    print_chip_summary(&chip2);

    // Print system summary
    print_system_summary();
}

/**
 * Test structure array operations
 */
void test_structure_arrays(void) {
    printf("\n=== Structure Array Operations Test ===\n");

    // Create array of chips
    chip_state_t chip_array[5];

    // Initialize array elements
    for (int i = 0; i < 5; i++) {
        char chip_id[16];
        char part_num[32];

        snprintf(chip_id, sizeof(chip_id), "CHIP_%d", i);
        snprintf(part_num, sizeof(part_num), "PART_NUM_%d", i + 1000);

        init_chip_state(&chip_array[i], chip_id, part_num);

        // Set different temperatures
        update_chip_temperature(&chip_array[i], 25.0f + (i * 10.0f));
    }

    // Process array using pointer arithmetic
    chip_state_t* chip_ptr = chip_array;
    for (int i = 0; i < 5; i++) {
        printf("Processing chip at index %d:\n", i);
        print_chip_summary(chip_ptr + i);
    }

    // Find hottest chip
    chip_state_t* hottest = chip_array;
    for (int i = 1; i < 5; i++) {
        if (chip_array[i].temperature > hottest->temperature) {
            hottest = &chip_array[i];
        }
    }

    printf("Hottest chip: %s at %.1f°C\n",
           hottest->chip_id, hottest->temperature);
}

