# Instructor Guide: Day 3 Memory Management Assignment

## Overview

This GitHub Classroom template provides a comprehensive C programming assignment focused on memory management, data structures, pointers, bit manipulation, and AI integration. The assignment includes robust autograding with 15 test categories totaling 150 points.

## Repository Structure

```
day3-memory-structures-ai/
├── .github/
│   ├── classroom/
│   │   └── autograding.json          # Autograding configuration
│   └── workflows/
│       └── classroom.yml             # GitHub Actions workflow
├── src/                              # Student implementation files
│   ├── chip_monitor.c                # Tasks 1 & 2: Pointers and structures
│   ├── bit_operations.c              # Task 3: Bit manipulation
│   ├── multi_chip_test.c             # Task 4: Array operations
│   └── ai_enhanced_validation.c      # Task 5: AI integration
├── include/                          # Header files
│   ├── chip_state.h                  # Structure definitions
│   └── bit_macros.h                  # Bit manipulation macros
├── tests/                            # Autograding test scripts
│   ├── test_pointers.sh              # Task 1 tests
│   ├── test_structures.sh            # Task 2 tests
│   ├── test_bit_operations.sh        # Task 3 tests
│   ├── test_multi_chip.sh            # Task 4 tests
│   ├── test_ai_documentation.sh      # Task 5 tests
│   ├── test_memory_safety.sh         # Memory safety checks
│   ├── test_code_quality.sh          # Code quality checks
│   ├── test_integration.sh           # Integration tests
│   └── test_function_implementation.sh # Implementation verification
├── ai_docs/
│   └── AI_USAGE_LOG.md               # AI usage documentation template
├── reference-solution/               # Complete reference implementation
├── docs/                             # Additional documentation
├── Makefile                          # Build system
└── README.md                         # Student instructions
```

## Autograding System (150 Points Total)

### Test Categories

1. **Build System Check** (10 points)
   - Tests if `make all` works properly
   - Verifies build system integrity

2. **Compilation Tests** (25 points)
   - Individual compilation of each source file
   - Ensures code compiles with standard flags

3. **Functionality Tests** (75 points)
   - **Task 1: Pointer Operations** (15 points)
   - **Task 2: Structure Operations** (15 points)
   - **Task 3: Bit Operations** (15 points)
   - **Task 4: Multi-Chip Arrays** (15 points)
   - **Task 5: AI Documentation** (15 points)

4. **Quality & Safety Tests** (25 points)
   - **Memory Safety** (10 points): AddressSanitizer checks
   - **Code Quality** (10 points): Header guards, documentation
   - **Function Implementation** (15 points): Actual vs TODO code

5. **Integration Test** (15 points)
   - Cross-component functionality
   - Overall system integration

### Test Execution

Each test script:
- Compiles the relevant source files
- Runs the programs with timeout protection
- Checks for specific output patterns
- Validates memory safety
- Returns pass/fail with detailed feedback

## Setting Up GitHub Classroom

### 1. Create Assignment

1. Go to your GitHub Classroom organization
2. Click "New Assignment"
3. Choose "Individual Assignment"
4. Set assignment title: "Day 3: Memory Management and Data Structures"
5. Use this repository as the template: `https://github.com/overcookedlobster/day3-memory-structures-ai`

### 2. Configure Autograding

The autograding is pre-configured in `.github/classroom/autograding.json`. GitHub Classroom will automatically detect and use this configuration.

### 3. Assignment Settings

**Recommended Settings:**
- **Deadline**: 1 week from assignment date
- **Enable feedback pull requests**: Yes
- **Grant students admin access**: No
- **Enable autograding**: Yes (automatically detected)

### 4. Student Instructions

Students receive:
- Complete assignment repository with TODO-marked functions
- Comprehensive README with learning objectives
- Build system and testing framework
- AI documentation requirements

## Grading Rubric Alignment

The autograding aligns with the 150-point rubric:

- **Code Functionality (60 pts)**: Covered by functionality tests (75 pts in autograding)
- **Code Quality & Style (30 pts)**: Covered by quality tests (25 pts in autograding)
- **Technical Implementation (25 pts)**: Covered by implementation tests (25 pts in autograding)
- **Documentation (20 pts)**: Covered by AI documentation tests (15 pts in autograding)
- **GitHub Workflow (15 pts)**: Handled by GitHub Classroom integration (10 pts in autograding)

## Manual Review Areas

While autograding covers most aspects, instructors should manually review:

1. **AI Usage Quality**: Review `ai_docs/AI_USAGE_LOG.md` for:
   - Depth of AI interaction documentation
   - Critical evaluation of AI suggestions
   - Learning reflection quality

2. **Code Style**: Beyond basic checks, review for:
   - Meaningful variable names
   - Code organization and readability
   - Comment quality and helpfulness

3. **Advanced Implementation**: Check for:
   - Creative solutions beyond minimum requirements
   - Proper error handling patterns
   - Performance considerations

## Common Student Issues

### Compilation Problems
- **Missing includes**: Students often forget `#include <string.h>`
- **Header guard errors**: Incorrect or missing header guards
- **Makefile issues**: Path or target problems

### Implementation Issues
- **TODO stubs**: Students submit without implementing functions
- **Memory safety**: Null pointer dereferences, buffer overflows
- **Bit manipulation**: Incorrect use of macros or bit operations

### AI Documentation
- **Insufficient detail**: Generic or minimal AI interaction logs
- **Missing critical evaluation**: Accepting AI suggestions without analysis
- **Incomplete reflection**: Shallow learning insights

## Troubleshooting Autograding

### Test Failures

1. **Compilation Failures**:
   - Check for missing header files
   - Verify include paths in Makefile
   - Look for syntax errors in student code

2. **Memory Safety Failures**:
   - Usually indicates null pointer dereferences
   - Check for buffer overflows in string operations
   - Verify proper error handling

3. **Function Implementation Failures**:
   - Student left TODO comments without implementation
   - Functions return placeholder values
   - Missing core logic

### GitHub Actions Issues

If autograding fails to run:
1. Check `.github/workflows/classroom.yml` syntax
2. Verify test script permissions (`chmod +x tests/*.sh`)
3. Ensure all test scripts exist and are executable

## Extending the Assignment

### Adding New Tests

1. Create new test script in `tests/` directory
2. Make it executable: `chmod +x tests/new_test.sh`
3. Add entry to `.github/classroom/autograding.json`
4. Test locally before deploying

### Modifying Point Values

Edit `.github/classroom/autograding.json`:
```json
{
  "name": "Test Name",
  "points": 15,  // Modify this value
  "run": "./tests/test_script.sh"
}
```

### Adding Memory Tools

The system supports additional memory checking tools:
- **Valgrind**: Add to test scripts for detailed memory analysis
- **Static Analysis**: Include cppcheck or similar tools
- **Coverage**: Add gcov for code coverage analysis

## Reference Solution

The `reference-solution/` directory contains:
- Complete implementation of all functions
- Comprehensive test suite (44 tests)
- Memory safety features
- Performance optimizations
- AI integration examples

Use this for:
- Verifying expected behavior
- Creating additional test cases
- Understanding implementation approaches
- Debugging student issues

## Best Practices

### For Instructors

1. **Review Early Submissions**: Identify common issues early
2. **Provide Feedback**: Use GitHub's feedback features
3. **Monitor Autograding**: Check for systematic failures
4. **Update Tests**: Refine tests based on student submissions

### For Students

1. **Start Early**: Complex assignment requiring significant time
2. **Test Frequently**: Use provided build system and tests
3. **Document AI Usage**: Required for full credit
4. **Ask Questions**: Encourage use of office hours and forums

## Support and Maintenance

### Regular Updates

- **Test Refinement**: Update tests based on student feedback
- **Documentation**: Keep instructions current and clear
- **Tool Updates**: Maintain compatibility with newer GCC versions

### Issue Tracking

Common issues to monitor:
- Autograding timeout problems
- Platform-specific compilation issues
- Test script reliability
- Student confusion points

## Conclusion

This autograding system provides comprehensive assessment of C programming skills while teaching responsible AI usage. The combination of automated testing and manual review ensures both technical competency and learning depth.

For questions or issues, refer to the test documentation in `tests/README.md` or contact the course development team.

