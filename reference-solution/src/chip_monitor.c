#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Include external function declarations
extern void demonstrate_pointer_operations(void);
extern void test_pointer_validation(void);
extern void demonstrate_chip_structures(void);
extern void test_structure_arrays(void);
extern void demonstrate_bit_operations(void);
extern void demonstrate_comprehensive_bit_ops(void);
extern void test_bit_patterns(void);
extern void demonstrate_advanced_bit_fields(void);

// Chip state structure (duplicated for integration)
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

#define MAX_MONITORED_CHIPS 8
#define MONITOR_UPDATE_INTERVAL 1000  // milliseconds

// Global monitoring state
static chip_state_t monitored_chips[MAX_MONITORED_CHIPS];
static int active_monitors = 0;
static bool monitoring_active = false;

/**
 * Initialize the chip monitoring system
 */
void init_chip_monitor(void) {
    printf("=== Initializing Chip Monitor System ===\n");

    // Clear all monitored chips
    memset(monitored_chips, 0, sizeof(monitored_chips));
    active_monitors = 0;
    monitoring_active = false;

    printf("Chip monitor system initialized\n");
    printf("Maximum monitored chips: %d\n", MAX_MONITORED_CHIPS);
}

/**
 * Add a chip to the monitoring system
 * @param chip Pointer to chip to monitor
 * @return Monitor index or -1 if failed
 */
int add_chip_to_monitor(chip_state_t* chip) {
    if (chip == NULL) {
        printf("Error: Cannot monitor NULL chip\n");
        return -1;
    }

    if (active_monitors >= MAX_MONITORED_CHIPS) {
        printf("Error: Monitor system at capacity (%d chips)\n", MAX_MONITORED_CHIPS);
        return -1;
    }

    if (!chip->is_initialized) {
        printf("Error: Cannot monitor uninitialized chip\n");
        return -1;
    }

    // Copy chip to monitoring array
    monitored_chips[active_monitors] = *chip;
    int monitor_index = active_monitors;
    active_monitors++;

    printf("Added chip '%s' to monitor (Index: %d)\n",
           chip->chip_id, monitor_index);

    return monitor_index;
}

/**
 * Remove a chip from monitoring
 * @param monitor_index Index of chip to remove
 * @return 1 if successful, 0 if failed
 */
int remove_chip_from_monitor(int monitor_index) {
    if (monitor_index < 0 || monitor_index >= active_monitors) {
        printf("Error: Invalid monitor index %d\n", monitor_index);
        return 0;
    }

    printf("Removing chip '%s' from monitor\n",
           monitored_chips[monitor_index].chip_id);

    // Shift remaining chips down
    for (int i = monitor_index; i < active_monitors - 1; i++) {
        monitored_chips[i] = monitored_chips[i + 1];
    }

    active_monitors--;
    memset(&monitored_chips[active_monitors], 0, sizeof(chip_state_t));

    printf("Chip removed. Active monitors: %d\n", active_monitors);
    return 1;
}

/**
 * Update chip status using pointer-based register access
 * @param chip Pointer to chip to update
 */
void update_chip_status(chip_state_t* chip) {
    if (chip == NULL || !chip->is_initialized) {
        return;
    }

    // Simulate register reads using pointer access
    uint32_t* status_ptr = &chip->registers.status_register;
    uint32_t* error_ptr = &chip->registers.error_register;

    // Check for errors in error register
    if (*error_ptr != 0) {
        chip->has_errors = true;
        chip->error_count++;

        // Set error bit in status register
        *status_ptr |= (1U << 2);  // STATUS_ERROR_BIT
    } else {
        // Clear error bit if no errors
        *status_ptr &= ~(1U << 2);
    }

    // Update temperature field in status register (bits 15-8)
    uint8_t temp_code = (uint8_t)(chip->temperature + 40);  // Offset for negative temps
    *status_ptr = (*status_ptr & 0xFFFF00FF) | ((uint32_t)temp_code << 8);

    // Simulate uptime increment
    chip->uptime_seconds++;
}

/**
 * Perform comprehensive chip health check
 * @param chip Pointer to chip to check
 * @return Health score (0-100)
 */
int perform_health_check(chip_state_t* chip) {
    if (chip == NULL || !chip->is_initialized) {
        return 0;
    }

    int health_score = 100;

    printf("\n--- Health Check: %s ---\n", chip->chip_id);

    // Check temperature
    if (chip->temperature > 85.0f) {
        printf("  Temperature WARNING: %.1f°C (critical)\n", chip->temperature);
        health_score -= 30;
    } else if (chip->temperature > 70.0f) {
        printf("  Temperature CAUTION: %.1f°C (high)\n", chip->temperature);
        health_score -= 15;
    } else {
        printf("  Temperature OK: %.1f°C\n", chip->temperature);
    }

    // Check voltage
    if (chip->voltage < 3.0f || chip->voltage > 3.6f) {
        printf("  Voltage WARNING: %.2fV (out of range)\n", chip->voltage);
        health_score -= 25;
    } else {
        printf("  Voltage OK: %.2fV\n", chip->voltage);
    }

    // Check error count
    if (chip->error_count > 10) {
        printf("  Error count HIGH: %u errors\n", chip->error_count);
        health_score -= 20;
    } else if (chip->error_count > 0) {
        printf("  Error count MODERATE: %u errors\n", chip->error_count);
        health_score -= 10;
    } else {
        printf("  Error count OK: %u errors\n", chip->error_count);
    }

    // Check register consistency
    bool ready_bit = (chip->registers.status_register & (1U << 0)) != 0;
    bool enable_bit = (chip->registers.control_register & (1U << 0)) != 0;

    if (enable_bit && !ready_bit) {
        printf("  Register INCONSISTENCY: Enabled but not ready\n");
        health_score -= 15;
    } else {
        printf("  Register consistency OK\n");
    }

    // Ensure health score doesn't go below 0
    if (health_score < 0) health_score = 0;

    printf("  Overall Health Score: %d/100\n", health_score);

    return health_score;
}

/**
 * Monitor all chips and generate status report
 */
void monitor_all_chips(void) {
    if (active_monitors == 0) {
        printf("No chips currently being monitored\n");
        return;
    }

    printf("\n=== Chip Monitoring Report ===\n");
    printf("Active Monitors: %d/%d\n", active_monitors, MAX_MONITORED_CHIPS);
    printf("Timestamp: %lu\n", (unsigned long)time(NULL));

    int total_health = 0;
    int critical_chips = 0;
    int warning_chips = 0;

    for (int i = 0; i < active_monitors; i++) {
        chip_state_t* chip = &monitored_chips[i];

        // Update chip status
        update_chip_status(chip);

        // Perform health check
        int health = perform_health_check(chip);
        total_health += health;

        if (health < 50) {
            critical_chips++;
        } else if (health < 80) {
            warning_chips++;
        }

        // Display quick status
        printf("\n[%d] %s: Health=%d%%, Temp=%.1f°C, Errors=%u, Uptime=%llus\n",
               i, chip->chip_id, health, chip->temperature,
               chip->error_count, (unsigned long long)chip->uptime_seconds);
    }

    // System summary
    float avg_health = (float)total_health / active_monitors;
    printf("\n=== System Summary ===\n");
    printf("Average Health: %.1f%%\n", avg_health);
    printf("Critical Chips: %d\n", critical_chips);
    printf("Warning Chips: %d\n", warning_chips);
    printf("Healthy Chips: %d\n", active_monitors - critical_chips - warning_chips);

    if (critical_chips > 0) {
        printf("SYSTEM STATUS: CRITICAL - Immediate attention required\n");
    } else if (warning_chips > 0) {
        printf("SYSTEM STATUS: WARNING - Monitor closely\n");
    } else {
        printf("SYSTEM STATUS: HEALTHY - All systems nominal\n");
    }
}

/**
 * Simulate chip stress test with monitoring
 */
void simulate_stress_test(void) {
    printf("\n=== Chip Stress Test Simulation ===\n");

    // Create test chips
    chip_state_t test_chips[3];

    // Initialize test chips
    memset(test_chips, 0, sizeof(test_chips));

    strcpy(test_chips[0].chip_id, "CPU_CORE");
    strcpy(test_chips[0].part_number, "ARM_A78");
    test_chips[0].serial_number = 123456;
    test_chips[0].temperature = 45.0f;
    test_chips[0].voltage = 3.3f;
    test_chips[0].is_initialized = true;
    test_chips[0].registers.control_register = 0x00000001;
    test_chips[0].registers.status_register = 0x80000001;

    strcpy(test_chips[1].chip_id, "GPU_CORE");
    strcpy(test_chips[1].part_number, "MALI_G78");
    test_chips[1].serial_number = 234567;
    test_chips[1].temperature = 55.0f;
    test_chips[1].voltage = 3.2f;
    test_chips[1].is_initialized = true;
    test_chips[1].registers.control_register = 0x00000001;
    test_chips[1].registers.status_register = 0x80000001;

    strcpy(test_chips[2].chip_id, "DSP_CORE");
    strcpy(test_chips[2].part_number, "HEXAGON");
    test_chips[2].serial_number = 345678;
    test_chips[2].temperature = 38.0f;
    test_chips[2].voltage = 3.4f;
    test_chips[2].is_initialized = true;
    test_chips[2].registers.control_register = 0x00000001;
    test_chips[2].registers.status_register = 0x80000001;

    // Add chips to monitor
    for (int i = 0; i < 3; i++) {
        add_chip_to_monitor(&test_chips[i]);
    }

    // Simulate stress test phases
    printf("\n--- Phase 1: Normal Operation ---\n");
    monitor_all_chips();

    printf("\n--- Phase 2: Increased Load ---\n");
    monitored_chips[0].temperature = 72.0f;  // CPU heating up
    monitored_chips[1].temperature = 68.0f;  // GPU heating up
    monitored_chips[2].voltage = 3.1f;       // DSP voltage drop
    monitor_all_chips();

    printf("\n--- Phase 3: Stress Conditions ---\n");
    monitored_chips[0].temperature = 88.0f;  // CPU overheating
    monitored_chips[0].registers.error_register = 0x00000001;  // Thermal error
    monitored_chips[1].temperature = 82.0f;  // GPU very hot
    monitored_chips[1].error_count = 5;      // Some errors
    monitored_chips[2].voltage = 2.9f;       // DSP undervoltage
    monitored_chips[2].registers.error_register = 0x00000002;  // Voltage error
    monitor_all_chips();

    printf("\n--- Phase 4: Recovery ---\n");
    monitored_chips[0].temperature = 65.0f;  // CPU cooling
    monitored_chips[0].registers.error_register = 0x00000000;  // Errors cleared
    monitored_chips[1].temperature = 58.0f;  // GPU cooling
    monitored_chips[2].voltage = 3.3f;       // DSP voltage restored
    monitored_chips[2].registers.error_register = 0x00000000;  // Errors cleared
    monitor_all_chips();
}

/**
 * Demonstrate integrated pointer, structure, and bit operations
 */
void demonstrate_integrated_operations(void) {
    printf("\n=== Integrated Operations Demo ===\n");

    // Create a chip for demonstration
    chip_state_t demo_chip;
    memset(&demo_chip, 0, sizeof(chip_state_t));

    strcpy(demo_chip.chip_id, "DEMO_CHIP");
    strcpy(demo_chip.part_number, "DEMO_PART");
    demo_chip.serial_number = 999999;
    demo_chip.temperature = 50.0f;
    demo_chip.voltage = 3.3f;
    demo_chip.is_initialized = true;

    // Initialize registers with specific patterns
    demo_chip.registers.control_register = 0x12345678;
    demo_chip.registers.status_register = 0x87654321;
    demo_chip.registers.error_register = 0x00000000;
    demo_chip.registers.config_register = 0xABCDEF00;

    printf("\n--- Pointer Operations on Chip Registers ---\n");
    uint32_t* ctrl_ptr = &demo_chip.registers.control_register;
    uint32_t* stat_ptr = &demo_chip.registers.status_register;

    printf("Control register via pointer: 0x%08X\n", *ctrl_ptr);
    printf("Status register via pointer:  0x%08X\n", *stat_ptr);

    // Modify registers using pointer arithmetic
    uint32_t* reg_array = (uint32_t*)&demo_chip.registers;
    for (int i = 0; i < 4; i++) {
        printf("Register[%d] = 0x%08X\n", i, *(reg_array + i));
        *(reg_array + i) |= (1U << i);  // Set bit i in each register
        printf("After setting bit %d: 0x%08X\n", i, *(reg_array + i));
    }

    printf("\n--- Structure Field Access ---\n");
    printf("Chip ID: %s\n", demo_chip.chip_id);
    printf("Temperature: %.1f°C\n", demo_chip.temperature);
    printf("Voltage: %.2fV\n", demo_chip.voltage);

    // Modify structure fields
    demo_chip.temperature += 10.0f;
    demo_chip.error_count = 3;
    printf("Updated temperature: %.1f°C\n", demo_chip.temperature);
    printf("Error count: %u\n", demo_chip.error_count);

    printf("\n--- Bit Manipulation on Registers ---\n");
    // Set enable bit (bit 0) in control register
    demo_chip.registers.control_register |= (1U << 0);
    printf("Control after enable: 0x%08X\n", demo_chip.registers.control_register);

    // Set ready bit (bit 0) in status register
    demo_chip.registers.status_register |= (1U << 0);
    printf("Status after ready: 0x%08X\n", demo_chip.registers.status_register);

    // Check if chip is enabled and ready
    bool enabled = (demo_chip.registers.control_register & (1U << 0)) != 0;
    bool ready = (demo_chip.registers.status_register & (1U << 0)) != 0;
    printf("Chip enabled: %s, ready: %s\n",
           enabled ? "Yes" : "No", ready ? "Yes" : "No");

    printf("\n--- Combined Operations ---\n");
    add_chip_to_monitor(&demo_chip);
    monitor_all_chips();
}

/**
 * Main demonstration function
 */
void run_chip_monitor_demo(void) {
    printf("\n========================================\n");
    printf("    CHIP MONITOR SYSTEM DEMO\n");
    printf("========================================\n");

    // Initialize the monitoring system
    init_chip_monitor();

    // Run integrated operations demo
    demonstrate_integrated_operations();

    // Run stress test simulation
    simulate_stress_test();

    printf("\n========================================\n");
    printf("    CHIP MONITOR DEMO COMPLETE\n");
    printf("========================================\n");
}

/**
 * Main function for comprehensive testing
 */
int main(void) {
    printf("=== Day 3: Memory Management and Data Structures ===\n");
    printf("Reference Solution Demonstration\n\n");

    // Seed random number generator
    srand((unsigned int)time(NULL));

    // Run all demonstrations
    printf("\n1. POINTER OPERATIONS\n");
    printf("=====================\n");
    demonstrate_pointer_operations();
    test_pointer_validation();

    printf("\n2. CHIP STRUCTURES\n");
    printf("==================\n");
    demonstrate_chip_structures();
    test_structure_arrays();

    printf("\n3. BIT OPERATIONS\n");
    printf("=================\n");
    demonstrate_bit_operations();
    demonstrate_comprehensive_bit_ops();
    test_bit_patterns();
    demonstrate_advanced_bit_fields();

    printf("\n4. INTEGRATED CHIP MONITOR\n");
    printf("==========================\n");
    run_chip_monitor_demo();

    printf("\n=== ALL DEMONSTRATIONS COMPLETE ===\n");
    printf("Reference solution successfully executed!\n");

    return 0;
}

