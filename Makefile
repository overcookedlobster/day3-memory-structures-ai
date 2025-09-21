# Makefile for Day 3: Memory Management and Data Structures with AI Integration
# Chip State Monitoring with AI Integration

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic
DEBUG_FLAGS = -g -DDEBUG -O0 -fsanitize=address
RELEASE_FLAGS = -O2 -DNDEBUG
INCLUDE_DIR = include
SRC_DIR = src
BUILD_DIR = bin
TEST_DIR = tests
AI_DOCS_DIR = ai_docs

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
EXECUTABLES = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%)

# Test files
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.c)
TEST_EXECUTABLES = $(TEST_SOURCES:$(TEST_DIR)/%.c=$(BUILD_DIR)/%)

# Default target
.PHONY: all
all: setup $(EXECUTABLES)

# Setup build directory
.PHONY: setup
setup:
	@mkdir -p $(BUILD_DIR)

# Build individual programs
$(BUILD_DIR)/%: $(SRC_DIR)/%.c
	@echo "Building $@..."
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $< -o $@ -lm

# Debug builds with memory sanitizer
.PHONY: debug
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean all

# Release builds
.PHONY: release
release: CFLAGS += $(RELEASE_FLAGS)
release: clean all

# Build specific programs
.PHONY: chip-monitor
chip-monitor: $(BUILD_DIR)/chip_monitor

.PHONY: bit-operations
bit-operations: $(BUILD_DIR)/bit_operations

.PHONY: multi-chip-test
multi-chip-test: $(BUILD_DIR)/multi_chip_test

.PHONY: ai-validation
ai-validation: $(BUILD_DIR)/ai_enhanced_validation

# Testing targets
.PHONY: test
test: debug test-pointers test-structures test-bits test-arrays test-ai

.PHONY: test-pointers
test-pointers: $(BUILD_DIR)/chip_monitor
	@echo "=== Testing Pointer Operations ==="
	@$(BUILD_DIR)/chip_monitor

.PHONY: test-structures
test-structures: $(BUILD_DIR)/chip_monitor
	@echo "=== Testing Structure Operations ==="
	@$(BUILD_DIR)/chip_monitor

.PHONY: test-bits
test-bits: $(BUILD_DIR)/bit_operations
	@echo "=== Testing Bit Manipulation ==="
	@$(BUILD_DIR)/bit_operations

.PHONY: test-arrays
test-arrays: $(BUILD_DIR)/multi_chip_test
	@echo "=== Testing Array Operations ==="
	@$(BUILD_DIR)/multi_chip_test

.PHONY: test-ai
test-ai: $(BUILD_DIR)/ai_enhanced_validation
	@echo "=== Testing AI Integration ==="
	@$(BUILD_DIR)/ai_enhanced_validation

# Memory checking with Valgrind
.PHONY: valgrind
valgrind: debug
	@echo "Running memory check with Valgrind..."
	@if command -v valgrind >/dev/null 2>&1; then \
		echo "Testing chip_monitor:"; \
		valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $(BUILD_DIR)/chip_monitor; \
		echo "Testing bit_operations:"; \
		valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $(BUILD_DIR)/bit_operations; \
	else \
		echo "Valgrind not installed. Install with: sudo apt-get install valgrind"; \
	fi

# Address sanitizer build (alternative to valgrind)
.PHONY: asan
asan: CFLAGS += -fsanitize=address -fno-omit-frame-pointer
asan: clean all
	@echo "Built with AddressSanitizer"

# Memory sanitizer build
.PHONY: msan
msan: CFLAGS += -fsanitize=memory -fno-omit-frame-pointer
msan: clean all
	@echo "Built with MemorySanitizer"

# Static analysis
.PHONY: static-analysis
static-analysis:
	@echo "Running static analysis..."
	@if command -v cppcheck >/dev/null 2>&1; then \
		cppcheck --enable=all --std=c11 --suppress=missingIncludeSystem -I$(INCLUDE_DIR) $(SRC_DIR)/; \
	else \
		echo "cppcheck not installed. Install with: sudo apt-get install cppcheck"; \
	fi

# Code style checking
.PHONY: style-check
style-check:
	@echo "Checking code style..."
	@if command -v astyle >/dev/null 2>&1; then \
		astyle --style=linux --indent=spaces=4 --max-code-length=100 --dry-run $(SRC_DIR)/*.c $(INCLUDE_DIR)/*.h; \
	else \
		echo "astyle not installed. Install with: sudo apt-get install astyle"; \
	fi

# Format code
.PHONY: format
format:
	@echo "Formatting code..."
	@if command -v astyle >/dev/null 2>&1; then \
		astyle --style=linux --indent=spaces=4 --max-code-length=100 $(SRC_DIR)/*.c $(INCLUDE_DIR)/*.h; \
	else \
		echo "astyle not installed. Install with: sudo apt-get install astyle"; \
	fi

# AI documentation check
.PHONY: check-ai-docs
check-ai-docs:
	@echo "Checking AI documentation..."
	@if [ -f "$(AI_DOCS_DIR)/AI_USAGE_LOG.md" ]; then \
		echo "✓ AI_USAGE_LOG.md exists"; \
		wc -l $(AI_DOCS_DIR)/AI_USAGE_LOG.md; \
	else \
		echo "✗ AI_USAGE_LOG.md missing - required for Task 5"; \
	fi
	@if [ -f "$(AI_DOCS_DIR)/ai_guidelines.md" ]; then \
		echo "✓ AI guidelines available"; \
	else \
		echo "✗ AI guidelines missing"; \
	fi

# Comprehensive test suite
.PHONY: test-comprehensive
test-comprehensive: debug valgrind static-analysis check-ai-docs
	@echo "=== Comprehensive Test Suite Complete ==="

# Performance profiling
.PHONY: profile
profile: CFLAGS += -pg
profile: clean all
	@echo "Built with profiling support"
	@echo "Run programs and use 'gprof' to analyze performance"

# Clean build artifacts
.PHONY: clean
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(BUILD_DIR)
	@rm -f $(SRC_DIR)/*.orig $(INCLUDE_DIR)/*.orig  # Remove astyle backup files
	@rm -f gmon.out  # Remove profiling output

# Install development tools
.PHONY: install-tools
install-tools:
	@echo "Installing development tools..."
	sudo apt-get update
	sudo apt-get install -y gcc gdb valgrind astyle cppcheck

# Documentation generation
.PHONY: docs
docs:
	@echo "Generating documentation..."
	@if command -v doxygen >/dev/null 2>&1; then \
		doxygen Doxyfile; \
	else \
		echo "doxygen not installed. Install with: sudo apt-get install doxygen"; \
	fi

# Help target
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all              - Build all programs"
	@echo "  debug            - Build with debug flags and AddressSanitizer"
	@echo "  release          - Build with optimization"
	@echo "  chip-monitor     - Build chip monitor program"
	@echo "  bit-operations   - Build bit operations program"
	@echo "  multi-chip-test  - Build multi-chip test program"
	@echo "  ai-validation    - Build AI validation program"
	@echo "  test             - Run all tests"
	@echo "  test-pointers    - Test pointer operations"
	@echo "  test-structures  - Test structure operations"
	@echo "  test-bits        - Test bit manipulation"
	@echo "  test-arrays      - Test array operations"
	@echo "  test-ai          - Test AI integration"
	@echo "  valgrind         - Run memory checking with Valgrind"
	@echo "  asan             - Build with AddressSanitizer"
	@echo "  msan             - Build with MemorySanitizer"
	@echo "  static-analysis  - Run static code analysis"
	@echo "  style-check      - Check code style"
	@echo "  format           - Format code with astyle"
	@echo "  check-ai-docs    - Verify AI documentation"
	@echo "  test-comprehensive - Run all tests and checks"
	@echo "  profile          - Build with profiling support"
	@echo "  clean            - Remove build artifacts"
	@echo "  install-tools    - Install development tools"
	@echo "  docs             - Generate documentation"
	@echo "  help             - Show this help message"

# Prevent make from deleting intermediate files
.PRECIOUS: $(BUILD_DIR)/%.o

