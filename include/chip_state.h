#ifndef CHIP_STATE_H
#define CHIP_STATE_H

#include <stdint.h>
#include <stdbool.h>

// Maximum number of chips in a system
#define MAX_CHIPS 8
#define CHIP_ID_LENGTH 16

// Chip status flags
#define CHIP_STATUS_ACTIVE    0x01
#define CHIP_STATUS_ERROR     0x02
#define CHIP_STATUS_OVERTEMP  0x04
#define CHIP_STATUS_UNDERVOLT 0x08

// Temperature thresholds (in Celsius)
#define TEMP_WARNING_THRESHOLD  75.0f
#define TEMP_CRITICAL_THRESHOLD 85.0f

// Voltage thresholds (in Volts)
#define VOLTAGE_MIN_THRESHOLD 3.0f
#define VOLTAGE_MAX_THRESHOLD 3.6f

/**
 * @brief Structure representing the state of a single chip
 *
 * This structure models a hardware chip with its registers,
 * environmental conditions, and identification information.
 */
typedef struct {
    // Hardware registers (simulated memory-mapped I/O)
    uint32_t control_reg;    /**< Control register value */
    uint32_t status_reg;     /**< Status register value */
    uint32_t error_reg;      /**< Error register value */
    uint32_t config_reg;     /**< Configuration register value */

    // Environmental monitoring
    float temperature;       /**< Temperature reading in Celsius */
    float voltage;          /**< Supply voltage in Volts */
    float current;          /**< Current consumption in Amperes */

    // Chip identification and status
    char chip_id[CHIP_ID_LENGTH];  /**< Chip identification string */
    bool is_active;         /**< Chip active status */
    uint8_t error_count;    /**< Number of errors detected */

    // Timing information
    uint64_t last_update;   /**< Timestamp of last update */
    uint32_t uptime_seconds; /**< Chip uptime in seconds */
} ChipState;

/**
 * @brief Structure for managing multiple chips
 */
typedef struct {
    ChipState chips[MAX_CHIPS];  /**< Array of chip states */
    int active_count;            /**< Number of active chips */
    int total_count;             /**< Total number of chips */
    float system_temperature;    /**< Average system temperature */
    bool system_healthy;         /**< Overall system health status */
} ChipSystem;

// Function prototypes for chip management

/**
 * @brief Initialize a single chip state
 * @param chip Pointer to ChipState structure
 * @param chip_id String identifier for the chip
 * @return true if initialization successful, false otherwise
 */
bool chip_init(ChipState *chip, const char *chip_id);

/**
 * @brief Update chip state from hardware registers
 * @param chip Pointer to ChipState structure
 * @return true if update successful, false otherwise
 */
bool chip_update_state(ChipState *chip);

/**
 * @brief Check if chip is within safe operating parameters
 * @param chip Pointer to ChipState structure
 * @return true if chip is healthy, false if issues detected
 */
bool chip_health_check(const ChipState *chip);

/**
 * @brief Initialize chip system with multiple chips
 * @param system Pointer to ChipSystem structure
 * @param num_chips Number of chips to initialize
 * @return true if initialization successful, false otherwise
 */
bool chip_system_init(ChipSystem *system, int num_chips);

/**
 * @brief Update all chips in the system
 * @param system Pointer to ChipSystem structure
 * @return Number of chips successfully updated
 */
int chip_system_update_all(ChipSystem *system);

/**
 * @brief Find chip by ID in the system
 * @param system Pointer to ChipSystem structure
 * @param chip_id String identifier to search for
 * @return Pointer to ChipState if found, NULL otherwise
 */
ChipState* chip_system_find_by_id(ChipSystem *system, const char *chip_id);

/**
 * @brief Get system health summary
 * @param system Pointer to ChipSystem structure
 * @return true if all chips are healthy, false otherwise
 */
bool chip_system_health_check(ChipSystem *system);

/**
 * @brief Print chip state information
 * @param chip Pointer to ChipState structure
 */
void chip_print_state(const ChipState *chip);

/**
 * @brief Print system summary
 * @param system Pointer to ChipSystem structure
 */
void chip_system_print_summary(const ChipSystem *system);

#endif // CHIP_STATE_H

