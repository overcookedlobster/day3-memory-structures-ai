#!/bin/bash

# Test script for Task 5: AI Documentation Check
# Tests if AI usage documentation exists and has content

set -e

echo "Testing Task 5: AI Documentation..."

ai_docs_found=0

# Test 1: Check if AI_USAGE_LOG.md exists
if [ -f "ai_docs/AI_USAGE_LOG.md" ]; then
    ai_docs_found=$((ai_docs_found + 1))
    echo "✓ AI_USAGE_LOG.md found"

    # Check if the file has meaningful content (more than just template)
    if [ -s "ai_docs/AI_USAGE_LOG.md" ]; then
        word_count=$(wc -w < "ai_docs/AI_USAGE_LOG.md")
        if [ "$word_count" -gt 50 ]; then
            ai_docs_found=$((ai_docs_found + 1))
            echo "✓ AI_USAGE_LOG.md has substantial content ($word_count words)"
        fi
    fi
else
    echo "✗ AI_USAGE_LOG.md not found"
fi

# Test 2: Check if ai_docs directory exists
if [ -d "ai_docs" ]; then
    ai_docs_found=$((ai_docs_found + 1))
    echo "✓ ai_docs directory found"
fi

# Test 3: Check if AI-enhanced validation compiles
if gcc -Wall -Wextra -std=c11 -Iinclude src/ai_enhanced_validation.c -o bin/ai_validation -lm 2>/dev/null; then
    ai_docs_found=$((ai_docs_found + 1))
    echo "✓ AI validation program compiles"
fi

# Test 4: Check if AI validation program runs
if [ -f "bin/ai_validation" ]; then
    if timeout 10s ./bin/ai_validation >/dev/null 2>&1; then
        ai_docs_found=$((ai_docs_found + 1))
        echo "✓ AI validation program runs successfully"
    fi
fi

# Require at least 3 out of 5 tests to pass
if [ $ai_docs_found -ge 3 ]; then
    echo "AI_DOCS_FOUND"
    exit 0
else
    echo "AI_DOCS_MISSING: Only $ai_docs_found/5 tests passed"
    exit 1
fi

