#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

// Forward declarations for macros
void* safe_malloc(size_t size, const char* file, int line);
void safe_free(void* ptr, const char* file, int line);

// Memory safety macros
#define SAFE_MALLOC(size) safe_malloc(size, __FILE__, __LINE__)
#define SAFE_FREE(ptr) do { safe_free(ptr, __FILE__, __LINE__); ptr = NULL; } while(0)

// Maximum number of tracked allocations
#define MAX_ALLOCATIONS 1000

// Memory debugging utilities
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

// Magic numbers for corruption detection
#define MAGIC_HEADER 0xDEADBEEF
#define MAGIC_FOOTER 0xBEEFDEAD
#define FREED_MAGIC  0xFEEDFACE

// Global allocation tracking
static memory_allocation_t g_allocations[MAX_ALLOCATIONS];
static int g_allocation_count = 0;
static size_t g_total_allocated = 0;
static size_t g_peak_allocated = 0;
static int g_allocation_id_counter = 0;

// Statistics
static int g_malloc_calls = 0;
static int g_free_calls = 0;
static int g_leak_count = 0;
static int g_double_free_count = 0;
static int g_corruption_count = 0;

/**
 * Initialize memory safety system
 */
void init_memory_safety(void) {
    printf("=== Initializing Memory Safety System ===\n");

    memset(g_allocations, 0, sizeof(g_allocations));
    g_allocation_count = 0;
    g_total_allocated = 0;
    g_peak_allocated = 0;
    g_allocation_id_counter = 0;

    g_malloc_calls = 0;
    g_free_calls = 0;
    g_leak_count = 0;
    g_double_free_count = 0;
    g_corruption_count = 0;

    printf("Memory safety system initialized\n");
}

/**
 * Find allocation record by pointer
 * @param ptr Pointer to find
 * @return Index of allocation or -1 if not found
 */
static int find_allocation(void* ptr) {
    if (ptr == NULL) return -1;

    for (int i = 0; i < g_allocation_count; i++) {
        if (g_allocations[i].ptr == ptr) {
            return i;
        }
    }
    return -1;
}

/**
 * Check for memory corruption in an allocation
 * @param alloc Pointer to allocation record
 * @return 1 if corrupted, 0 if intact
 */
static int check_corruption(memory_allocation_t* alloc) {
    if (alloc == NULL || alloc->ptr == NULL) return 1;

    // Check header magic
    uint32_t* header = (uint32_t*)((char*)alloc->ptr - sizeof(uint32_t));
    if (*header != MAGIC_HEADER) {
        printf("CORRUPTION: Header magic corrupted at %p (expected 0x%08X, got 0x%08X)\n",
               alloc->ptr, MAGIC_HEADER, *header);
        return 1;
    }

    // Check footer magic
    uint32_t* footer = (uint32_t*)((char*)alloc->ptr + alloc->size);
    if (*footer != MAGIC_FOOTER) {
        printf("CORRUPTION: Footer magic corrupted at %p (expected 0x%08X, got 0x%08X)\n",
               alloc->ptr, MAGIC_FOOTER, *footer);
        return 1;
    }

    return 0;
}

/**
 * Safe malloc with tracking and corruption detection
 * @param size Size to allocate
 * @param file Source file name
 * @param line Source line number
 * @return Allocated pointer or NULL if failed
 */
void* safe_malloc(size_t size, const char* file, int line) {
    if (size == 0) {
        printf("WARNING: Attempting to allocate 0 bytes at %s:%d\n", file, line);
        return NULL;
    }

    if (g_allocation_count >= MAX_ALLOCATIONS) {
        printf("ERROR: Maximum allocations (%d) reached at %s:%d\n",
               MAX_ALLOCATIONS, file, line);
        return NULL;
    }

    // Allocate extra space for magic numbers
    size_t total_size = size + 2 * sizeof(uint32_t);
    void* raw_ptr = malloc(total_size);

    if (raw_ptr == NULL) {
        printf("ERROR: malloc failed for %zu bytes at %s:%d\n", size, file, line);
        return NULL;
    }

    // Set up magic numbers
    uint32_t* header = (uint32_t*)raw_ptr;
    *header = MAGIC_HEADER;

    void* user_ptr = (char*)raw_ptr + sizeof(uint32_t);

    uint32_t* footer = (uint32_t*)((char*)user_ptr + size);
    *footer = MAGIC_FOOTER;

    // Record allocation
    memory_allocation_t* alloc = &g_allocations[g_allocation_count];
    alloc->ptr = user_ptr;
    alloc->size = size;
    alloc->file = file;
    alloc->line = line;
    alloc->is_freed = false;
    alloc->alloc_time = time(NULL);
    alloc->magic_header = MAGIC_HEADER;
    alloc->magic_footer = MAGIC_FOOTER;

    g_allocation_count++;
    g_malloc_calls++;
    g_total_allocated += size;

    if (g_total_allocated > g_peak_allocated) {
        g_peak_allocated = g_total_allocated;
    }

    printf("MALLOC: %zu bytes at %p (%s:%d) [ID: %d]\n",
           size, user_ptr, file, line, g_allocation_id_counter++);

    return user_ptr;
}

/**
 * Safe free with tracking and corruption detection
 * @param ptr Pointer to free
 * @param file Source file name
 * @param line Source line number
 */
void safe_free(void* ptr, const char* file, int line) {
    if (ptr == NULL) {
        printf("WARNING: Attempting to free NULL pointer at %s:%d\n", file, line);
        return;
    }

    int alloc_index = find_allocation(ptr);
    if (alloc_index == -1) {
        printf("ERROR: Attempting to free untracked pointer %p at %s:%d\n",
               ptr, file, line);
        return;
    }

    memory_allocation_t* alloc = &g_allocations[alloc_index];

    if (alloc->is_freed) {
        printf("ERROR: Double free detected for pointer %p at %s:%d\n",
               ptr, file, line);
        printf("  Originally allocated at %s:%d\n", alloc->file, alloc->line);
        g_double_free_count++;
        return;
    }

    // Check for corruption before freeing
    if (check_corruption(alloc)) {
        printf("ERROR: Memory corruption detected before free at %s:%d\n", file, line);
        printf("  Originally allocated at %s:%d\n", alloc->file, alloc->line);
        g_corruption_count++;
        // Continue with free to avoid leaks, but mark as corrupted
    }

    // Mark as freed
    alloc->is_freed = true;
    g_total_allocated -= alloc->size;
    g_free_calls++;

    // Fill freed memory with pattern to detect use-after-free
    memset(ptr, 0xDD, alloc->size);

    // Free the actual memory (including magic numbers)
    void* raw_ptr = (char*)ptr - sizeof(uint32_t);
    free(raw_ptr);

    printf("FREE: %zu bytes at %p (%s:%d)\n", alloc->size, ptr, file, line);
}

/**
 * Check for memory leaks
 */
void check_memory_leaks(void) {
    printf("\n=== Memory Leak Check ===\n");

    g_leak_count = 0;
    size_t leaked_bytes = 0;

    for (int i = 0; i < g_allocation_count; i++) {
        memory_allocation_t* alloc = &g_allocations[i];

        if (!alloc->is_freed) {
            printf("LEAK: %zu bytes at %p allocated at %s:%d\n",
                   alloc->size, alloc->ptr, alloc->file, alloc->line);

            time_t current_time = time(NULL);
            double age = difftime(current_time, alloc->alloc_time);
            printf("  Age: %.0f seconds\n", age);

            g_leak_count++;
            leaked_bytes += alloc->size;
        }
    }

    if (g_leak_count == 0) {
        printf("No memory leaks detected!\n");
    } else {
        printf("Found %d leaks totaling %zu bytes\n", g_leak_count, leaked_bytes);
    }
}

/**
 * Print comprehensive memory usage report
 */
void print_memory_usage_report(void) {
    printf("\n=== Memory Usage Report ===\n");

    printf("Allocation Statistics:\n");
    printf("  Total malloc calls: %d\n", g_malloc_calls);
    printf("  Total free calls: %d\n", g_free_calls);
    printf("  Current allocations: %d\n", g_allocation_count - g_free_calls);
    printf("  Peak memory usage: %zu bytes\n", g_peak_allocated);
    printf("  Current memory usage: %zu bytes\n", g_total_allocated);

    printf("\nError Statistics:\n");
    printf("  Memory leaks: %d\n", g_leak_count);
    printf("  Double frees: %d\n", g_double_free_count);
    printf("  Corruptions: %d\n", g_corruption_count);

    printf("\nActive Allocations:\n");
    int active_count = 0;
    for (int i = 0; i < g_allocation_count; i++) {
        memory_allocation_t* alloc = &g_allocations[i];
        if (!alloc->is_freed) {
            printf("  [%d] %zu bytes at %p (%s:%d)\n",
                   active_count++, alloc->size, alloc->ptr, alloc->file, alloc->line);
        }
    }

    if (active_count == 0) {
        printf("  No active allocations\n");
    }

    printf("\nMemory Health: ");
    if (g_leak_count == 0 && g_double_free_count == 0 && g_corruption_count == 0) {
        printf("EXCELLENT\n");
    } else if (g_corruption_count == 0 && g_double_free_count == 0) {
        printf("GOOD (minor leaks)\n");
    } else if (g_corruption_count == 0) {
        printf("FAIR (leaks and double frees)\n");
    } else {
        printf("POOR (memory corruption detected)\n");
    }
}

/**
 * Validate all active allocations for corruption
 */
void validate_all_allocations(void) {
    printf("\n=== Validating All Active Allocations ===\n");

    int corrupted_count = 0;
    int validated_count = 0;

    for (int i = 0; i < g_allocation_count; i++) {
        memory_allocation_t* alloc = &g_allocations[i];

        if (!alloc->is_freed) {
            validated_count++;
            if (check_corruption(alloc)) {
                corrupted_count++;
                printf("CORRUPTION found in allocation at %p (%s:%d)\n",
                       alloc->ptr, alloc->file, alloc->line);
            }
        }
    }

    printf("Validated %d allocations\n", validated_count);
    if (corrupted_count == 0) {
        printf("All allocations are intact!\n");
    } else {
        printf("Found %d corrupted allocations\n", corrupted_count);
        g_corruption_count += corrupted_count;
    }
}

/**
 * Simulate memory corruption for testing
 * @param ptr Pointer to corrupt
 */
void simulate_corruption(void* ptr) {
    if (ptr == NULL) return;

    printf("Simulating memory corruption at %p\n", ptr);

    int alloc_index = find_allocation(ptr);
    if (alloc_index == -1) {
        printf("Cannot corrupt untracked pointer\n");
        return;
    }

    memory_allocation_t* alloc = &g_allocations[alloc_index];

    // Corrupt the footer magic number
    uint32_t* footer = (uint32_t*)((char*)ptr + alloc->size);
    *footer = 0xBADC0DE;

    printf("Corrupted footer magic at %p\n", ptr);
}

/**
 * Demonstrate memory safety features
 */
void demonstrate_memory_safety(void) {
    printf("\n=== Memory Safety Demonstration ===\n");

    init_memory_safety();

    printf("\n--- Normal Allocation/Deallocation ---\n");
    void* ptr1 = SAFE_MALLOC(100);
    void* ptr2 = SAFE_MALLOC(200);
    void* ptr3 = SAFE_MALLOC(50);

    if (ptr1 && ptr2 && ptr3) {
        // Use the memory
        memset(ptr1, 0xAA, 100);
        memset(ptr2, 0xBB, 200);
        memset(ptr3, 0xCC, 50);

        printf("Memory allocated and initialized\n");

        // Free some memory
        SAFE_FREE(ptr2);
        ptr2 = NULL;

        print_memory_usage_report();
    }

    printf("\n--- Error Condition Testing ---\n");

    // Test double free
    printf("\nTesting double free:\n");
    void* ptr4 = SAFE_MALLOC(75);
    if (ptr4) {
        SAFE_FREE(ptr4);
        SAFE_FREE(ptr4);  // This should be caught
    }

    // Test free of NULL
    printf("\nTesting free of NULL:\n");
    void* null_ptr = NULL;
    SAFE_FREE(null_ptr);

    // Test corruption detection
    printf("\nTesting corruption detection:\n");
    void* ptr5 = SAFE_MALLOC(128);
    if (ptr5) {
        simulate_corruption(ptr5);
        validate_all_allocations();
        SAFE_FREE(ptr5);  // Should detect corruption
    }

    // Test memory leak detection
    printf("\n--- Memory Leak Testing ---\n");
    void* leak1 = SAFE_MALLOC(64);
    void* leak2 = SAFE_MALLOC(32);
    // Intentionally not freeing these to test leak detection

    check_memory_leaks();

    // Clean up remaining allocations
    printf("\n--- Cleanup ---\n");
    SAFE_FREE(ptr1);
    SAFE_FREE(ptr3);
    SAFE_FREE(leak1);
    SAFE_FREE(leak2);

    print_memory_usage_report();
    check_memory_leaks();
}

/**
 * Test memory safety with chip structures
 */
void test_memory_safety_with_structures(void) {
    printf("\n=== Memory Safety with Structures ===\n");

    // Define a simple chip structure for testing
    typedef struct {
        char name[32];
        uint32_t id;
        float temperature;
        uint32_t* registers;
        int register_count;
    } test_chip_t;

    printf("\n--- Allocating Chip Structures ---\n");

    // Allocate array of chip structures
    int chip_count = 5;
    test_chip_t* chips = (test_chip_t*)SAFE_MALLOC(chip_count * sizeof(test_chip_t));

    if (chips == NULL) {
        printf("Failed to allocate chip array\n");
        return;
    }

    // Initialize chips and allocate registers for each
    for (int i = 0; i < chip_count; i++) {
        snprintf(chips[i].name, sizeof(chips[i].name), "CHIP_%d", i);
        chips[i].id = 1000 + i;
        chips[i].temperature = 25.0f + (i * 5.0f);
        chips[i].register_count = 10 + i;

        // Allocate registers for each chip
        chips[i].registers = (uint32_t*)SAFE_MALLOC(chips[i].register_count * sizeof(uint32_t));

        if (chips[i].registers != NULL) {
            // Initialize registers
            for (int j = 0; j < chips[i].register_count; j++) {
                chips[i].registers[j] = 0x1000 + (i * 100) + j;
            }
            printf("Initialized %s with %d registers\n",
                   chips[i].name, chips[i].register_count);
        }
    }

    print_memory_usage_report();

    printf("\n--- Using Chip Structures ---\n");

    // Use the structures
    for (int i = 0; i < chip_count; i++) {
        printf("Chip %s: ID=%u, Temp=%.1f°C, Registers=%d\n",
               chips[i].name, chips[i].id, chips[i].temperature, chips[i].register_count);

        // Access some registers
        if (chips[i].registers != NULL && chips[i].register_count > 0) {
            printf("  First register: 0x%08X\n", chips[i].registers[0]);
            printf("  Last register:  0x%08X\n", chips[i].registers[chips[i].register_count - 1]);
        }
    }

    printf("\n--- Partial Cleanup (Simulating Leak) ---\n");

    // Free registers for some chips but not others (simulate partial cleanup)
    for (int i = 0; i < chip_count; i++) {
        if (i % 2 == 0 && chips[i].registers != NULL) {
            SAFE_FREE(chips[i].registers);
            chips[i].registers = NULL;
            printf("Freed registers for %s\n", chips[i].name);
        }
    }

    check_memory_leaks();

    printf("\n--- Complete Cleanup ---\n");

    // Free remaining registers
    for (int i = 0; i < chip_count; i++) {
        if (chips[i].registers != NULL) {
            SAFE_FREE(chips[i].registers);
            chips[i].registers = NULL;
        }
    }

    // Free chip array
    SAFE_FREE(chips);

    print_memory_usage_report();
    check_memory_leaks();
}

/**
 * Stress test memory allocation system
 */
void stress_test_memory_system(void) {
    printf("\n=== Memory System Stress Test ===\n");

    const int num_iterations = 100;
    const int max_alloc_size = 1024;
    void* ptrs[num_iterations];

    printf("Performing %d random allocations/deallocations\n", num_iterations);

    // Initialize pointer array
    for (int i = 0; i < num_iterations; i++) {
        ptrs[i] = NULL;
    }

    // Random allocation/deallocation pattern
    srand((unsigned int)time(NULL));

    for (int iteration = 0; iteration < num_iterations * 2; iteration++) {
        int index = rand() % num_iterations;

        if (ptrs[index] == NULL) {
            // Allocate
            size_t size = (rand() % max_alloc_size) + 1;
            ptrs[index] = SAFE_MALLOC(size);

            if (ptrs[index] != NULL) {
                // Fill with pattern
                memset(ptrs[index], (index & 0xFF), size);
            }
        } else {
            // Free
            SAFE_FREE(ptrs[index]);
            ptrs[index] = NULL;
        }

        // Occasionally validate all allocations
        if (iteration % 20 == 0) {
            validate_all_allocations();
        }
    }

    printf("Stress test completed\n");

    // Clean up remaining allocations
    printf("Cleaning up remaining allocations\n");
    for (int i = 0; i < num_iterations; i++) {
        if (ptrs[i] != NULL) {
            SAFE_FREE(ptrs[i]);
            ptrs[i] = NULL;
        }
    }

    print_memory_usage_report();
    check_memory_leaks();
}

// Macros moved to top of file

