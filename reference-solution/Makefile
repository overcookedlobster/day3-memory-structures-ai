# Day 3 Reference Solution Makefile
# Memory Management and Data Structures

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -O2
INCLUDES = -Iinclude
SRCDIR = src
OBJDIR = obj
TESTDIR = tests
BINDIR = bin

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TEST_SOURCES = $(wildcard $(TESTDIR)/*.c)
TEST_OBJECTS = $(TEST_SOURCES:$(TESTDIR)/%.c=$(OBJDIR)/%.o)

# Main executable
TARGET = $(BINDIR)/day3_reference
TEST_TARGET = $(BINDIR)/day3_tests

# Individual component targets
POINTER_TARGET = $(BINDIR)/pointer_demo
STRUCTURES_TARGET = $(BINDIR)/structures_demo
BITS_TARGET = $(BINDIR)/bits_demo
MONITOR_TARGET = $(BINDIR)/monitor_demo
ADVANCED_TARGET = $(BINDIR)/advanced_demo
MEMORY_TARGET = $(BINDIR)/memory_demo
AI_TARGET = $(BINDIR)/ai_demo

.PHONY: all clean test individual demos help

# Default target
all: $(TARGET)

# Create directories
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

# Compile object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR)/%.o: $(TESTDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Main executable
$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(OBJECTS) -o $@ -lm

# Test executable
$(TEST_TARGET): $(filter-out $(OBJDIR)/chip_monitor.o, $(OBJECTS)) $(TEST_OBJECTS) | $(BINDIR)
	$(CC) $(filter-out $(OBJDIR)/chip_monitor.o, $(OBJECTS)) $(TEST_OBJECTS) -o $@ -lm

# Individual component demos
$(POINTER_TARGET): $(OBJDIR)/pointer_registers.o | $(BINDIR)
	$(CC) $(OBJDIR)/pointer_registers.o -o $@ -lm
	@echo "Built pointer operations demo"

$(STRUCTURES_TARGET): $(OBJDIR)/chip_structures.o | $(BINDIR)
	$(CC) $(OBJDIR)/chip_structures.o -o $@ -lm
	@echo "Built structures demo"

$(BITS_TARGET): $(OBJDIR)/bit_operations.o | $(BINDIR)
	$(CC) $(OBJDIR)/bit_operations.o -o $@ -lm
	@echo "Built bit operations demo"

$(MONITOR_TARGET): $(OBJDIR)/chip_monitor.o $(OBJDIR)/pointer_registers.o $(OBJDIR)/chip_structures.o $(OBJDIR)/bit_operations.o | $(BINDIR)
	$(CC) $(OBJDIR)/chip_monitor.o $(OBJDIR)/pointer_registers.o $(OBJDIR)/chip_structures.o $(OBJDIR)/bit_operations.o -o $@ -lm
	@echo "Built chip monitor demo"

$(ADVANCED_TARGET): $(OBJDIR)/advanced_pointers.o | $(BINDIR)
	$(CC) $(OBJDIR)/advanced_pointers.o -o $@ -lm
	@echo "Built advanced pointers demo"

$(MEMORY_TARGET): $(OBJDIR)/memory_safety.o | $(BINDIR)
	$(CC) $(OBJDIR)/memory_safety.o -o $@ -lm
	@echo "Built memory safety demo"

$(AI_TARGET): $(OBJDIR)/ai_optimized_code.o | $(BINDIR)
	$(CC) $(OBJDIR)/ai_optimized_code.o -o $@ -lm
	@echo "Built AI optimization demo"

# Build individual component demos
individual: $(POINTER_TARGET) $(STRUCTURES_TARGET) $(BITS_TARGET) $(ADVANCED_TARGET) $(MEMORY_TARGET) $(AI_TARGET)

# Build all demos
demos: individual $(MONITOR_TARGET)

# Run tests
test: $(TEST_TARGET)
	@echo "Running Day 3 tests..."
	./$(TEST_TARGET)

# Run main program
run: $(TARGET)
	@echo "Running Day 3 reference solution..."
	./$(TARGET)

# Run individual demos
run-pointer: $(POINTER_TARGET)
	@echo "Running pointer operations demo..."
	./$(POINTER_TARGET)

run-structures: $(STRUCTURES_TARGET)
	@echo "Running structures demo..."
	./$(STRUCTURES_TARGET)

run-bits: $(BITS_TARGET)
	@echo "Running bit operations demo..."
	./$(BITS_TARGET)

run-monitor: $(MONITOR_TARGET)
	@echo "Running chip monitor demo..."
	./$(MONITOR_TARGET)

run-advanced: $(ADVANCED_TARGET)
	@echo "Running advanced pointers demo..."
	./$(ADVANCED_TARGET)

run-memory: $(MEMORY_TARGET)
	@echo "Running memory safety demo..."
	./$(MEMORY_TARGET)

run-ai: $(AI_TARGET)
	@echo "Running AI optimization demo..."
	./$(AI_TARGET)

# Run all demos
run-all: run run-pointer run-structures run-bits run-advanced run-memory run-ai

# Memory check with valgrind
memcheck: $(TARGET)
	@echo "Running memory check with valgrind..."
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

# Static analysis
analyze:
	@echo "Running static analysis..."
	cppcheck --enable=all --std=c99 $(SRCDIR)/ $(TESTDIR)/

# Code formatting
format:
	@echo "Formatting code..."
	find $(SRCDIR) $(TESTDIR) -name "*.c" -exec clang-format -i {} \;
	find include -name "*.h" -exec clang-format -i {} \;

# Generate documentation
docs:
	@echo "Generating documentation..."
	doxygen Doxyfile 2>/dev/null || echo "Doxygen not available"

# Clean build artifacts
clean:
	rm -rf $(OBJDIR) $(BINDIR)
	@echo "Cleaned build artifacts"

# Show help
help:
	@echo "Day 3 Reference Solution Makefile"
	@echo "=================================="
	@echo ""
	@echo "Targets:"
	@echo "  all          - Build main executable (default)"
	@echo "  test         - Build and run tests"
	@echo "  individual   - Build individual component demos"
	@echo "  demos        - Build all demos including integrated monitor"
	@echo "  run          - Run main program"
	@echo "  run-all      - Run all demos"
	@echo "  run-pointer  - Run pointer operations demo"
	@echo "  run-structures - Run structures demo"
	@echo "  run-bits     - Run bit operations demo"
	@echo "  run-monitor  - Run chip monitor demo"
	@echo "  run-advanced - Run advanced pointers demo"
	@echo "  run-memory   - Run memory safety demo"
	@echo "  run-ai       - Run AI optimization demo"
	@echo "  memcheck     - Run with valgrind memory checking"
	@echo "  analyze      - Run static analysis with cppcheck"
	@echo "  format       - Format code with clang-format"
	@echo "  docs         - Generate documentation with doxygen"
	@echo "  clean        - Remove build artifacts"
	@echo "  help         - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make                    # Build main executable"
	@echo "  make test               # Run tests"
	@echo "  make demos run-all      # Build and run all demos"
	@echo "  make memcheck           # Check for memory leaks"

# Debug build
debug: CFLAGS += -DDEBUG -g3 -O0
debug: $(TARGET)
	@echo "Built debug version"

# Release build
release: CFLAGS += -DNDEBUG -O3
release: $(TARGET)
	@echo "Built release version"

# Profile build
profile: CFLAGS += -pg
profile: $(TARGET)
	@echo "Built profile version"

