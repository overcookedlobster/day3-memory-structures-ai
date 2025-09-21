/**
 * @file multi_chip_test.c
 * @brief Day 3 Task 4: Array of Chips for Multi-Device Testing (45 minutes)
 *
 * This file implements array manipulation and string handling for managing
 * multiple chip instances. Students will learn array operations, string
 * manipulation, and batch processing in hardware validation contexts.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "chip_state.h"

/**
 * Task 4: Array of Chips for Multi-Device Testing (45 minutes)
 *
 * TODO: Implement multi-chip array management and string handling
 *
 * Learning objectives:
 * - Master array manipulation for multiple chip instances
 * - Practice string handling for chip identification
 * - Implement batch operations on chip arrays
 * - Develop search and sort algorithms for chip management
 */

/**
 * @brief Initialize chip system with multiple chips
 * @param system Pointer to ChipSystem structure
 * @param num_chips Number of chips to initialize
 * @return true if initialization successful, false otherwise
 */
bool chip_system_init(ChipSystem *system, int num_chips) {
    // TODO: Implement chip system initialization
    //
    // Steps to complete:
    // 1. Validate input parameters
    // 2. Check if num_chips is within MAX_CHIPS limit
    // 3. Initialize each chip in the array
    // 4. Set system-level parameters
    // 5. Generate unique chip IDs for each chip
    //
    // Chip ID format: "CHIP_XX" where XX is zero-padded number
    // Example: CHIP_00, CHIP_01, CHIP_02, etc.

    if (system == NULL) {
        printf("ERROR: System pointer is NULL\n");
        return false;
    }

    if (num_chips <= 0 || num_chips > MAX_CHIPS) {
        printf("ERROR: Invalid number of chips: %d (max: %d)\n", num_chips, MAX_CHIPS);
        return false;
    }

    printf("TODO: Implement chip_system_init function\n");
    printf("  - Initializing %d chips\n", num_chips);
    printf("  - Maximum chips supported: %d\n", MAX_CHIPS);

    // Example framework (uncomment and complete):
    // system->total_count = num_chips;
    // system->active_count = 0;
    // system->system_healthy = true;
    // system->system_temperature = 0.0f;
    //
    // for (int i = 0; i < num_chips; i++) {
    //     char chip_id[CHIP_ID_LENGTH];
    //     snprintf(chip_id, sizeof(chip_id), "CHIP_%02d", i);
    //
    //     if (chip_init(&system->chips[i], chip_id)) {
    //         system->active_count++;
    //         printf("Initialized chip %d: %s\n", i, chip_id);
    //     } else {
    //         printf("Failed to initialize chip %d\n", i);
    //         return false;
    //     }
    // }
    //
    // printf("Chip system initialized: %d/%d chips active\n",
    //        system->active_count, system->total_count);

    return false; // Replace with actual implementation
}

/**
 * @brief Update all chips in the system
 * @param system Pointer to ChipSystem structure
 * @return Number of chips successfully updated
 */
int chip_system_update_all(ChipSystem *system) {
    // TODO: Implement batch chip update operation
    //
    // Steps to complete:
    // 1. Validate input parameter
    // 2. Loop through all chips in the system
    // 3. Update each chip's state
    // 4. Count successful updates
    // 5. Update system-level statistics (average temperature, etc.)
    // 6. Check overall system health

    if (system == NULL) {
        printf("ERROR: System pointer is NULL\n");
        return 0;
    }

    printf("TODO: Implement chip_system_update_all function\n");
    printf("  - Updating %d chips\n", system->total_count);

    int updated_count = 0;
    float total_temperature = 0.0f;
    int healthy_chips = 0;

    // Example framework (uncomment and complete):
    // for (int i = 0; i < system->total_count; i++) {
    //     if (chip_update_state(&system->chips[i])) {
    //         updated_count++;
    //
    //         // Accumulate temperature for average calculation
    //         total_temperature += system->chips[i].temperature;
    //
    //         // Count healthy chips
    //         if (chip_health_check(&system->chips[i])) {
    //             healthy_chips++;
    //         }
    //
    //         printf("Updated chip %s: %.1f°C\n",
    //                system->chips[i].chip_id,
    //                system->chips[i].temperature);
    //     } else {
    //         printf("Failed to update chip %s\n", system->chips[i].chip_id);
    //     }
    // }
    //
    // // Update system statistics
    // if (updated_count > 0) {
    //     system->system_temperature = total_temperature / updated_count;
    //     system->system_healthy = (healthy_chips == system->total_count);
    //     system->active_count = updated_count;
    // }
    //
    // printf("System update complete: %d/%d chips updated\n",
    //        updated_count, system->total_count);
    // printf("Average temperature: %.1f°C\n", system->system_temperature);
    // printf("System health: %s\n", system->system_healthy ? "HEALTHY" : "ISSUES DETECTED");

    return updated_count;
}

/**
 * @brief Find chip by ID in the system
 * @param system Pointer to ChipSystem structure
 * @param chip_id String identifier to search for
 * @return Pointer to ChipState if found, NULL otherwise
 */
ChipState* chip_system_find_by_id(ChipSystem *system, const char *chip_id) {
    // TODO: Implement chip search by ID
    //
    // Steps to complete:
    // 1. Validate input parameters
    // 2. Loop through all chips in the system
    // 3. Use string comparison to find matching chip ID
    // 4. Return pointer to found chip or NULL if not found
    // 5. Handle case-insensitive search (bonus)
    //
    // Use strcmp() or strncmp() for string comparison
    // Consider using strcasecmp() for case-insensitive comparison

    if (system == NULL || chip_id == NULL) {
        printf("ERROR: Invalid parameters for chip search\n");
        return NULL;
    }

    printf("TODO: Implement chip_system_find_by_id function\n");
    printf("  - Searching for chip ID: %s\n", chip_id);
    printf("  - Total chips to search: %d\n", system->total_count);

    // Example framework (uncomment and complete):
    // for (int i = 0; i < system->total_count; i++) {
    //     if (strcmp(system->chips[i].chip_id, chip_id) == 0) {
    //         printf("Found chip: %s at index %d\n", chip_id, i);
    //         return &system->chips[i];
    //     }
    // }
    //
    // printf("Chip not found: %s\n", chip_id);

    return NULL; // Replace with actual implementation
}

/**
 * @brief Get system health summary
 * @param system Pointer to ChipSystem structure
 * @return true if all chips are healthy, false otherwise
 */
bool chip_system_health_check(ChipSystem *system) {
    // TODO: Implement comprehensive system health check
    //
    // Steps to complete:
    // 1. Validate input parameter
    // 2. Check each chip's health status
    // 3. Count healthy vs unhealthy chips
    // 4. Check system-level parameters (average temperature, etc.)
    // 5. Return overall system health status
    // 6. Print detailed health report

    if (system == NULL) {
        printf("ERROR: System pointer is NULL\n");
        return false;
    }

    printf("TODO: Implement chip_system_health_check function\n");
    printf("  - Checking health of %d chips\n", system->total_count);

    int healthy_count = 0;
    int warning_count = 0;
    int critical_count = 0;

    // Example framework (uncomment and complete):
    // printf("=== System Health Check ===\n");
    //
    // for (int i = 0; i < system->total_count; i++) {
    //     bool chip_healthy = chip_health_check(&system->chips[i]);
    //
    //     if (chip_healthy) {
    //         healthy_count++;
    //         printf("  %s: HEALTHY\n", system->chips[i].chip_id);
    //     } else {
    //         if (system->chips[i].temperature > TEMP_CRITICAL_THRESHOLD) {
    //             critical_count++;
    //             printf("  %s: CRITICAL (%.1f°C)\n",
    //                    system->chips[i].chip_id,
    //                    system->chips[i].temperature);
    //         } else {
    //             warning_count++;
    //             printf("  %s: WARNING\n", system->chips[i].chip_id);
    //         }
    //     }
    // }
    //
    // printf("\nHealth Summary:\n");
    // printf("  Healthy: %d\n", healthy_count);
    // printf("  Warning: %d\n", warning_count);
    // printf("  Critical: %d\n", critical_count);
    // printf("  System Temperature: %.1f°C\n", system->system_temperature);
    //
    // bool system_ok = (critical_count == 0) && (warning_count <= system->total_count / 4);
    // printf("  Overall Status: %s\n", system_ok ? "HEALTHY" : "NEEDS ATTENTION");
    //
    // system->system_healthy = system_ok;
    // return system_ok;

    return true; // Replace with actual implementation
}

/**
 * @brief Print system summary
 * @param system Pointer to ChipSystem structure
 */
void chip_system_print_summary(const ChipSystem *system) {
    // TODO: Implement comprehensive system summary
    //
    // Steps to complete:
    // 1. Validate input parameter
    // 2. Print system-level statistics
    // 3. Print summary of each chip
    // 4. Show system health overview
    // 5. Format output for readability

    if (system == NULL) {
        printf("ERROR: Cannot print NULL system summary\n");
        return;
    }

    printf("TODO: Implement chip_system_print_summary function\n");
    printf("  - System has %d chips\n", system->total_count);
    printf("  - Active chips: %d\n", system->active_count);
    printf("  - System healthy: %s\n", system->system_healthy ? "YES" : "NO");

    // Example framework (uncomment and complete):
    // printf("=== Chip System Summary ===\n");
    // printf("Total Chips: %d\n", system->total_count);
    // printf("Active Chips: %d\n", system->active_count);
    // printf("System Temperature: %.1f°C\n", system->system_temperature);
    // printf("System Health: %s\n", system->system_healthy ? "HEALTHY" : "ISSUES");
    //
    // printf("\nChip Details:\n");
    // for (int i = 0; i < system->total_count; i++) {
    //     printf("  [%d] %s: %.1f°C, %.2fV, %s\n",
    //            i,
    //            system->chips[i].chip_id,
    //            system->chips[i].temperature,
    //            system->chips[i].voltage,
    //            system->chips[i].is_active ? "ACTIVE" : "INACTIVE");
    // }
    // printf("========================\n");
}

/**
 * @brief Sort chips by temperature (bubble sort for educational purposes)
 * @param system Pointer to ChipSystem structure
 * @param ascending true for ascending order, false for descending
 */
void chip_system_sort_by_temperature(ChipSystem *system, bool ascending) {
    // TODO: Implement chip sorting by temperature
    //
    // Steps to complete:
    // 1. Validate input parameter
    // 2. Implement bubble sort algorithm
    // 3. Compare chips by temperature
    // 4. Swap chips when needed
    // 5. Handle both ascending and descending order
    //
    // This is educational - use bubble sort to practice array manipulation
    // In production, you'd use qsort() or other efficient algorithms

    if (system == NULL) {
        printf("ERROR: System pointer is NULL\n");
        return;
    }

    printf("TODO: Implement chip_system_sort_by_temperature function\n");
    printf("  - Sorting %d chips by temperature (%s)\n",
           system->total_count,
           ascending ? "ascending" : "descending");

    // Example framework (uncomment and complete):
    // printf("Sorting chips by temperature...\n");
    //
    // // Bubble sort implementation
    // for (int i = 0; i < system->total_count - 1; i++) {
    //     for (int j = 0; j < system->total_count - i - 1; j++) {
    //         bool should_swap = false;
    //
    //         if (ascending) {
    //             should_swap = system->chips[j].temperature > system->chips[j + 1].temperature;
    //         } else {
    //             should_swap = system->chips[j].temperature < system->chips[j + 1].temperature;
    //         }
    //
    //         if (should_swap) {
    //             // Swap the entire ChipState structures
    //             ChipState temp = system->chips[j];
    //             system->chips[j] = system->chips[j + 1];
    //             system->chips[j + 1] = temp;
    //         }
    //     }
    // }
    //
    // printf("Sorting complete\n");
}

/**
 * @brief Demonstrate string manipulation with chip IDs
 * @param system Pointer to ChipSystem structure
 */
void demonstrate_string_operations(ChipSystem *system) {
    // TODO: Implement string manipulation demonstration
    //
    // Steps to complete:
    // 1. Show string length calculation
    // 2. Demonstrate string copying and concatenation
    // 3. Show string comparison operations
    // 4. Practice string formatting with chip data
    // 5. Handle string safety (bounds checking)

    if (system == NULL) {
        printf("ERROR: System pointer is NULL\n");
        return;
    }

    printf("TODO: Implement demonstrate_string_operations function\n");
    printf("  - Demonstrating string operations with chip IDs\n");

    // Example framework (uncomment and complete):
    // printf("=== String Operations Demo ===\n");
    //
    // for (int i = 0; i < system->total_count && i < 3; i++) {
    //     ChipState *chip = &system->chips[i];
    //
    //     // String length
    //     size_t id_length = strlen(chip->chip_id);
    //     printf("Chip %d ID: '%s' (length: %zu)\n", i, chip->chip_id, id_length);
    //
    //     // String comparison
    //     if (strncmp(chip->chip_id, "CHIP_", 5) == 0) {
    //         printf("  ID starts with 'CHIP_'\n");
    //     }
    //
    //     // String formatting
    //     char status_string[64];
    //     snprintf(status_string, sizeof(status_string),
    //              "%s: %.1f°C, %s",
    //              chip->chip_id,
    //              chip->temperature,
    //              chip->is_active ? "ACTIVE" : "INACTIVE");
    //     printf("  Status: %s\n", status_string);
    //
    //     // String copying (safe)
    //     char id_copy[CHIP_ID_LENGTH];
    //     strncpy(id_copy, chip->chip_id, sizeof(id_copy) - 1);
    //     id_copy[sizeof(id_copy) - 1] = '\0';
    //     printf("  ID Copy: %s\n", id_copy);
    // }
}

/**
 * @brief Main function demonstrating Task 4
 */
int main(void) {
    printf("=== Day 3: Multi-Chip Test - Task 4 ===\n\n");

    // Initialize chip system
    ChipSystem test_system = {0};
    int num_test_chips = 6;

    printf("--- Chip System Initialization ---\n");
    if (chip_system_init(&test_system, num_test_chips)) {
        printf("✓ Chip system initialization successful\n");
    } else {
        printf("✗ Chip system initialization failed (TODO: implement)\n");
    }

    printf("\n--- System Update Operations ---\n");
    int updated_chips = chip_system_update_all(&test_system);
    printf("Updated chips: %d\n", updated_chips);

    printf("\n--- Chip Search Operations ---\n");
    const char* search_ids[] = {"CHIP_00", "CHIP_03", "CHIP_99"};
    for (int i = 0; i < 3; i++) {
        ChipState* found_chip = chip_system_find_by_id(&test_system, search_ids[i]);
        if (found_chip != NULL) {
            printf("✓ Found chip: %s\n", search_ids[i]);
        } else {
            printf("✗ Chip not found: %s (TODO: implement search)\n", search_ids[i]);
        }
    }

    printf("\n--- System Health Check ---\n");
    bool system_healthy = chip_system_health_check(&test_system);
    printf("System health status: %s\n", system_healthy ? "HEALTHY" : "NEEDS ATTENTION");

    printf("\n--- System Summary ---\n");
    chip_system_print_summary(&test_system);

    printf("\n--- Chip Sorting Demo ---\n");
    chip_system_sort_by_temperature(&test_system, true);
    printf("Chips sorted by temperature (ascending)\n");

    printf("\n--- String Operations Demo ---\n");
    demonstrate_string_operations(&test_system);

    printf("\n=== Task 4 Complete ===\n");
    printf("Next: Implement ai_enhanced_validation.c for Task 5\n");

    return 0;
}

