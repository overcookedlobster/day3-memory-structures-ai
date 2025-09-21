#ifndef BIT_MACROS_H
#define BIT_MACROS_H

#include <stdint.h>

/**
 * @file bit_macros.h
 * @brief Comprehensive bit manipulation macros for register operations
 *
 * This header provides safe, efficient macros for bit-level operations
 * commonly used in embedded systems and hardware validation.
 */

// Basic bit manipulation macros
#define SET_BIT(reg, bit)     ((reg) |= (1U << (bit)))
#define CLEAR_BIT(reg, bit)   ((reg) &= ~(1U << (bit)))
#define TOGGLE_BIT(reg, bit)  ((reg) ^= (1U << (bit)))
#define CHECK_BIT(reg, bit)   (((reg) >> (bit)) & 1U)

// Multi-bit field operations
#define SET_BITS(reg, mask)     ((reg) |= (mask))
#define CLEAR_BITS(reg, mask)   ((reg) &= ~(mask))
#define TOGGLE_BITS(reg, mask)  ((reg) ^= (mask))
#define CHECK_BITS(reg, mask)   ((reg) & (mask))

// Field extraction and insertion macros
#define EXTRACT_FIELD(reg, pos, width) (((reg) >> (pos)) & ((1U << (width)) - 1U))
#define INSERT_FIELD(reg, val, pos, width) do { \
    uint32_t mask = ((1U << (width)) - 1U) << (pos); \
    (reg) = ((reg) & ~mask) | (((val) << (pos)) & mask); \
} while(0)

// Bit position definitions for common register fields
#define BIT_0   0
#define BIT_1   1
#define BIT_2   2
#define BIT_3   3
#define BIT_4   4
#define BIT_5   5
#define BIT_6   6
#define BIT_7   7
#define BIT_8   8
#define BIT_9   9
#define BIT_10  10
#define BIT_11  11
#define BIT_12  12
#define BIT_13  13
#define BIT_14  14
#define BIT_15  15
#define BIT_16  16
#define BIT_17  17
#define BIT_18  18
#define BIT_19  19
#define BIT_20  20
#define BIT_21  21
#define BIT_22  22
#define BIT_23  23
#define BIT_24  24
#define BIT_25  25
#define BIT_26  26
#define BIT_27  27
#define BIT_28  28
#define BIT_29  29
#define BIT_30  30
#define BIT_31  31

// Common bit masks
#define MASK_1BIT   0x01U
#define MASK_2BIT   0x03U
#define MASK_3BIT   0x07U
#define MASK_4BIT   0x0FU
#define MASK_5BIT   0x1FU
#define MASK_6BIT   0x3FU
#define MASK_7BIT   0x7FU
#define MASK_8BIT   0xFFU
#define MASK_16BIT  0xFFFFU
#define MASK_32BIT  0xFFFFFFFFU

// Chip-specific register bit definitions
// Control Register bits
#define CTRL_ENABLE_BIT     BIT_0
#define CTRL_RESET_BIT      BIT_1
#define CTRL_TEST_MODE_BIT  BIT_2
#define CTRL_DEBUG_BIT      BIT_3
#define CTRL_POWER_DOWN_BIT BIT_4

// Status Register bits
#define STATUS_READY_BIT    BIT_0
#define STATUS_ERROR_BIT    BIT_1
#define STATUS_BUSY_BIT     BIT_2
#define STATUS_OVERTEMP_BIT BIT_3
#define STATUS_UNDERVOLT_BIT BIT_4

// Error Register bits
#define ERROR_PARITY_BIT    BIT_0
#define ERROR_TIMEOUT_BIT   BIT_1
#define ERROR_OVERFLOW_BIT  BIT_2
#define ERROR_UNDERFLOW_BIT BIT_3
#define ERROR_CHECKSUM_BIT  BIT_4

// Configuration Register fields
#define CONFIG_MODE_POS     0
#define CONFIG_MODE_WIDTH   3
#define CONFIG_SPEED_POS    4
#define CONFIG_SPEED_WIDTH  2
#define CONFIG_VOLTAGE_POS  8
#define CONFIG_VOLTAGE_WIDTH 4

// Utility macros for register operations
#define IS_BIT_SET(reg, bit)    (CHECK_BIT(reg, bit) != 0)
#define IS_BIT_CLEAR(reg, bit)  (CHECK_BIT(reg, bit) == 0)

// Safe bit manipulation with bounds checking
#define SAFE_SET_BIT(reg, bit) do { \
    if ((bit) < 32) SET_BIT(reg, bit); \
} while(0)

#define SAFE_CLEAR_BIT(reg, bit) do { \
    if ((bit) < 32) CLEAR_BIT(reg, bit); \
} while(0)

#define SAFE_TOGGLE_BIT(reg, bit) do { \
    if ((bit) < 32) TOGGLE_BIT(reg, bit); \
} while(0)

// Byte manipulation macros
#define GET_BYTE_0(reg) ((reg) & 0xFF)
#define GET_BYTE_1(reg) (((reg) >> 8) & 0xFF)
#define GET_BYTE_2(reg) (((reg) >> 16) & 0xFF)
#define GET_BYTE_3(reg) (((reg) >> 24) & 0xFF)

#define SET_BYTE_0(reg, val) ((reg) = ((reg) & 0xFFFFFF00) | ((val) & 0xFF))
#define SET_BYTE_1(reg, val) ((reg) = ((reg) & 0xFFFF00FF) | (((val) & 0xFF) << 8))
#define SET_BYTE_2(reg, val) ((reg) = ((reg) & 0xFF00FFFF) | (((val) & 0xFF) << 16))
#define SET_BYTE_3(reg, val) ((reg) = ((reg) & 0x00FFFFFF) | (((val) & 0xFF) << 24))

// Endianness conversion macros
#define SWAP_BYTES_16(val) (((val) << 8) | ((val) >> 8))
#define SWAP_BYTES_32(val) (((val) << 24) | (((val) & 0xFF00) << 8) | \
                           (((val) & 0xFF0000) >> 8) | ((val) >> 24))

// Bit counting utilities
#define COUNT_ONES(reg) __builtin_popcount(reg)
#define COUNT_ZEROS(reg) (32 - __builtin_popcount(reg))
#define FIND_FIRST_SET(reg) __builtin_ffs(reg)
#define FIND_LAST_SET(reg) (32 - __builtin_clz(reg))

// Register validation macros
#define VALIDATE_REGISTER_RANGE(reg, min, max) \
    ((reg) >= (min) && (reg) <= (max))

#define VALIDATE_BIT_POSITION(bit) ((bit) < 32)

#define VALIDATE_FIELD_WIDTH(width) ((width) > 0 && (width) <= 32)

// Debug macros for bit operations (compile-time removable)
#ifdef DEBUG_BIT_OPS
#define DEBUG_PRINT_BITS(reg) do { \
    printf("Register 0x%08X: ", (reg)); \
    for (int i = 31; i >= 0; i--) { \
        printf("%d", CHECK_BIT(reg, i)); \
        if (i % 4 == 0 && i > 0) printf(" "); \
    } \
    printf("\n"); \
} while(0)
#else
#define DEBUG_PRINT_BITS(reg) do {} while(0)
#endif

#endif // BIT_MACROS_H

