#ifndef CHIP_STATE_H
#define CHIP_STATE_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_CHIPS 16

// Register set structure
typedef struct {
    uint32_t control_register;
    uint32_t status_register;
    uint32_t error_register;
    uint32_t config_register;
} register_set_t;

// Chip state structure
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

// System state structure
typedef struct {
    chip_state_t chips[MAX_CHIPS];
    int active_chip_count;
    int total_error_count;
    float average_temperature;
    char system_status[64];
} system_state_t;

// Function declarations for chip_structures.c
void init_chip_state(chip_state_t* chip, const char* id, const char* part_num);
void update_chip_temperature(chip_state_t* chip, float new_temp);
void update_chip_registers(chip_state_t* chip, register_set_t* new_regs);
int validate_chip_state(const chip_state_t* chip);
void print_chip_summary(const chip_state_t* chip);
void init_system_state(void);
int add_chip_to_system(chip_state_t* chip);
void update_system_statistics(void);
void print_system_summary(void);
void demonstrate_chip_structures(void);
void test_structure_arrays(void);

// Function declarations for pointer_registers.c
uint32_t* get_register_pointer(uint32_t address);
uint32_t read_register_via_pointer(uint32_t address);
void write_register_via_pointer(uint32_t address, uint32_t value);
int validate_register_pointer(uint32_t* reg_ptr);
void bulk_register_write(uint32_t start_addr, uint32_t* values, int count);
void bulk_register_read(uint32_t start_addr, uint32_t* buffer, int count);
uint32_t* find_register_by_value(uint32_t value, uint32_t start_addr, int range);
void demonstrate_pointer_operations(void);
void test_pointer_validation(void);

// Function declarations for bit_operations.c
void demonstrate_bit_operations(void);
uint32_t create_test_pattern(int pattern_type);
int validate_bit_pattern(uint32_t value, uint32_t expected_pattern, uint32_t mask);
void analyze_register_bits(uint32_t register_value, const char* register_name);
void enable_chip_power(chip_state_t* chip);
void disable_chip_power(chip_state_t* chip);
bool is_chip_ready(const chip_state_t* chip);
uint32_t get_error_flags(const chip_state_t* chip);
void clear_error_flags(chip_state_t* chip, uint32_t flags_to_clear);
void demonstrate_comprehensive_bit_ops(void);
void test_bit_patterns(void);
void demonstrate_advanced_bit_fields(void);

// Function declarations for chip_monitor.c
void init_chip_monitor(void);
int add_chip_to_monitor(chip_state_t* chip);
int remove_chip_from_monitor(int monitor_index);
void update_chip_status(chip_state_t* chip);
int perform_health_check(chip_state_t* chip);
void monitor_all_chips(void);
void simulate_stress_test(void);
void demonstrate_integrated_operations(void);
void run_chip_monitor_demo(void);

// Function declarations for advanced_pointers.c
typedef int (*validation_func_t)(const chip_state_t* chip);
typedef void (*chip_event_callback_t)(chip_state_t* chip, int event_type);

int validate_power_levels(const chip_state_t* chip);
int validate_temperature_range(const chip_state_t* chip);
int validate_register_consistency(const chip_state_t* chip);
int validate_error_states(const chip_state_t* chip);
int run_validation_strategy(chip_state_t* chip, int strategy_index);
int run_all_validations(chip_state_t* chip);
void power_event_callback(chip_state_t* chip, int event_type);
void error_event_callback(chip_state_t* chip, int event_type);
void temperature_monitor_callback(chip_state_t* chip, int event_type);
int register_chip_callback(chip_state_t* chip, chip_event_callback_t callback);
void trigger_chip_callbacks(chip_state_t* chip, int event_type);
chip_state_t** create_chip_array(int count);
void destroy_chip_array(chip_state_t** chips, int count);
void process_chip_array(chip_state_t** chips, int count);
void demonstrate_advanced_pointers(void);
void test_function_pointer_arrays(void);

// Function declarations for memory_safety.c
typedef struct {
    void* ptr;
    size_t size;
    const char* file;
    int line;
    bool is_freed;
    time_t alloc_time;
    uint32_t magic_header;
    uint32_t magic_footer;
} memory_allocation_t;

void init_memory_safety(void);
void* safe_malloc(size_t size, const char* file, int line);
void safe_free(void* ptr, const char* file, int line);
void check_memory_leaks(void);
void print_memory_usage_report(void);
void validate_all_allocations(void);
void simulate_corruption(void* ptr);
void demonstrate_memory_safety(void);
void test_memory_safety_with_structures(void);
void stress_test_memory_system(void);

// Function declarations for ai_optimized_code.c
typedef struct {
    clock_t start_time;
    clock_t end_time;
    double execution_time_ms;
    const char* operation_name;
} performance_metric_t;

void start_performance_measurement(performance_metric_t* metric, const char* operation_name);
void end_performance_measurement(performance_metric_t* metric);
uint32_t calculate_crc32_optimized(const uint8_t* data, size_t length);
uint32_t calculate_crc32_naive(const uint8_t* data, size_t length);
int validate_registers_optimized(const register_set_t* registers);
int validate_registers_original(const register_set_t* registers);
int process_chip_array_optimized(chip_state_t** chips, int count, float temperature_threshold);
int process_chip_array_original(chip_state_t** chips, int count, float temperature_threshold);
void optimized_memory_copy(void* dest, const void* src, size_t size);
void compare_optimization_performance(void);
void demonstrate_ai_optimizations(void);

// Event types for callbacks
#define EVENT_POWER_ON      1
#define EVENT_POWER_OFF     2
#define EVENT_ERROR         3
#define EVENT_TEMPERATURE   4
#define EVENT_VOLTAGE       5

// Memory safety macros
#define SAFE_MALLOC(size) safe_malloc(size, __FILE__, __LINE__)
#define SAFE_FREE(ptr) do { safe_free(ptr, __FILE__, __LINE__); ptr = NULL; } while(0)

#endif // CHIP_STATE_H

