# Autograding Test Suite

This directory contains the comprehensive test suite for the Day 3 Memory Management and Data Structures assignment.

## Test Structure

The autograding system tests 5 main areas with a total of 150 points:

### 1. Compilation Tests (30 points)
- **Build System Check** (10 points): Tests if `make all` works
- **Individual Compilation** (20 points): Tests each source file compiles independently

### 2. Functionality Tests (75 points)
- **Task 1: Pointer Operations** (15 points): Tests pointer fundamentals and register access
- **Task 2: Structure Operations** (15 points): Tests ChipState structure implementation
- **Task 3: Bit Operations** (15 points): Tests bit manipulation macros and operations
- **Task 4: Multi-Chip Arrays** (15 points): Tests array handling and string operations
- **Task 5: AI Integration** (15 points): Tests AI documentation and implementation

### 3. Code Quality Tests (25 points)
- **Memory Safety** (10 points): Tests with AddressSanitizer for memory leaks/errors
- **Code Quality** (10 points): Tests header guards, includes, documentation
- **Function Implementation** (15 points): Verifies actual implementation vs TODO stubs

### 4. Integration Tests (20 points)
- **Integration Test** (15 points): Tests all components work together
- **AI Documentation** (5 points): Additional AI usage documentation check

## Test Scripts

### Core Functionality Tests
- `test_pointers.sh`: Tests Task 1 pointer operations
- `test_structures.sh`: Tests Task 2 structure operations
- `test_bit_operations.sh`: Tests Task 3 bit manipulation
- `test_multi_chip.sh`: Tests Task 4 array operations
- `test_ai_documentation.sh`: Tests Task 5 AI documentation

### Quality and Safety Tests
- `test_memory_safety.sh`: Memory leak and safety testing
- `test_code_quality.sh`: Code structure and documentation
- `test_integration.sh`: Cross-component integration
- `test_function_implementation.sh`: Implementation completeness

## Test Criteria

### Pointer Operations (test_pointers.sh)
Tests for:
- `write_register_pointer` function usage
- `read_register_pointer` function usage
- Pointer arithmetic demonstration
- NULL pointer error handling
- No segmentation faults

### Structure Operations (test_structures.sh)
Tests for:
- `chip_init` function implementation
- `chip_update_state` function usage
- `chip_health_check` function usage
- `chip_print_state` function usage
- Structure pointer demonstrations
- Proper error handling

### Bit Operations (test_bit_operations.sh)
Tests for:
- Basic bit manipulation (SET_BIT, CLEAR_BIT, etc.)
- Register field operations
- Status register analysis
- Error register management
- Advanced bit operations
- Chip-specific bit operations
- Hexadecimal output patterns

### Multi-Chip Arrays (test_multi_chip.sh)
Tests for:
- `chip_system_init` function
- `chip_system_update_all` function
- `chip_system_find_by_id` function
- `chip_system_health_check` function
- `chip_system_print_summary` function
- Temperature sorting functionality
- String operations demonstration
- Proper chip ID formatting (CHIP_XX)

### AI Documentation (test_ai_documentation.sh)
Tests for:
- `ai_docs/AI_USAGE_LOG.md` file exists
- Substantial content in AI documentation (>50 words)
- `ai_docs` directory structure
- AI validation program compilation and execution

### Memory Safety (test_memory_safety.sh)
Tests for:
- Compilation with AddressSanitizer
- Execution without memory errors
- All programs pass memory safety checks
- No segmentation faults or memory leaks

### Code Quality (test_code_quality.sh)
Tests for:
- Proper header guards in `.h` files
- Correct `#include` statements
- Function documentation comments (`/**`)
- Basic Makefile targets (`all`, `clean`)
- Error handling patterns (NULL checks, return values)

### Integration (test_integration.sh)
Tests for:
- All components build together successfully
- All expected executables are created
- Programs run without crashing
- Header files are properly structured
- Cross-references between source files

### Function Implementation (test_function_implementation.sh)
Tests for:
- Actual code implementation beyond TODO comments
- Proper function logic and return values
- Implementation patterns (loops, conditionals, assignments)
- Reduced TODO comment density
- Real functionality vs placeholder code

## Running Tests Manually

You can run individual test scripts for debugging:

```bash
# Make scripts executable
chmod +x tests/*.sh

# Run individual tests
./tests/test_pointers.sh
./tests/test_structures.sh
./tests/test_bit_operations.sh
./tests/test_multi_chip.sh
./tests/test_ai_documentation.sh
./tests/test_memory_safety.sh
./tests/test_code_quality.sh
./tests/test_integration.sh
./tests/test_function_implementation.sh
```

## Expected Output

Each test script outputs either:
- **Success**: `[TEST_NAME]_PASSED` or specific success message
- **Failure**: `[TEST_NAME]_FAILED` with details about what failed

## Grading Rubric Alignment

The tests align with the 150-point grading rubric:

- **Code Functionality (60 pts)**: Covered by functionality tests
- **Code Quality & Style (30 pts)**: Covered by quality and safety tests
- **Technical Implementation (25 pts)**: Covered by implementation and integration tests
- **Documentation (20 pts)**: Covered by AI documentation and code quality tests
- **GitHub Workflow (15 pts)**: Handled by GitHub Classroom integration

## Troubleshooting

### Common Issues
1. **Compilation Failures**: Check include paths and missing headers
2. **Memory Safety Failures**: Look for NULL pointer dereferences or buffer overflows
3. **Function Implementation Failures**: Ensure TODO comments are replaced with actual code
4. **Integration Failures**: Verify all source files compile and link properly

### Debug Tips
- Use `make debug` to compile with debugging symbols
- Run programs individually to isolate issues
- Check that all required functions are implemented
- Ensure proper error handling throughout code

