#!/bin/bash

echo "════════════════════════════════════════════════════════"
echo "  Rodeo VM - Test Suite Execution"
echo "════════════════════════════════════════════════════════"
echo ""

# Compile the project
echo "→ Compiling Rodeo VM..."
make clean > /dev/null 2>&1
make > /dev/null 2>&1

if [ $? -ne 0 ]; then
    echo "✗ Compilation failed!"
    exit 1
fi

echo "✓ Compilation successful!"
echo ""

# Array of test files
tests=(
    "test.rodeo:Main test file"
    "examples/test_basic.rodeo:Basic commands"
    "examples/test_arithmetic.rodeo:Arithmetic operations"
    "examples/test_if_else.rodeo:Conditional statements"
    "examples/test_while.rodeo:While loops"
    "examples/test_sensors.rodeo:Sensor reading"
    "examples/test_patterns.rodeo:Movement patterns"
    "examples/test_safety.rodeo:Safety system"
)

# Run each test
passed=0
failed=0
total=${#tests[@]}

for test_info in "${tests[@]}"; do
    IFS=':' read -r file description <<< "$test_info"
    
    echo "────────────────────────────────────────────────────────"
    echo "Test: $description"
    echo "File: $file"
    echo "────────────────────────────────────────────────────────"
    
    if [ ! -f "$file" ]; then
        echo "✗ File not found!"
        ((failed++))
        echo ""
        continue
    fi
    
    # Run the test
    ./rodeo-vm "$file" > /dev/null 2>&1
    
    if [ $? -eq 0 ]; then
        echo "✓ Test passed successfully!"
        ((passed++))
    else
        echo "✗ Test failed!"
        ((failed++))
    fi
    
    echo ""
done

# Summary
echo "════════════════════════════════════════════════════════"
echo "  Test Summary"
echo "════════════════════════════════════════════════════════"
echo "Total tests:  $total"
echo "Passed:       $passed"
echo "Failed:       $failed"
echo ""

if [ $failed -eq 0 ]; then
    echo "✓ All tests passed! 🎉"
    exit 0
else
    echo "✗ Some tests failed."
    exit 1
fi

