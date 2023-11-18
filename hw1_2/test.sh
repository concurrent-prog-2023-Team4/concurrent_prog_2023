#!/bin/bash

# Set the path to your hw1_2 program
HW1_2_PATH="./hw1_2"

# Input file
INPUT_FILE="primes.txt"

# Array of thread values to test
THREADS=(21 22 23 24 25 26 27 28 29 30)

# Number of times to run the program for each thread count
RUNS=10

# Function to calculate average time
calculate_average_time() {
    local total_time=0
    for ((i = 1; i <= $RUNS; i++)); do
        local current_time=$(/usr/bin/time -p $HW1_2_PATH "$1" < "$INPUT_FILE" 2>&1 | grep real | awk '{print $2}')
        total_time=$(python3 -c "print('{:.3f}'.format($total_time + $current_time))")
    done
    local average_time=$(python3 -c "print('{:.3f}'.format($total_time / $RUNS))")
    echo $average_time
}

# Loop through each thread value and run the program
for thread_count in "${THREADS[@]}"; do
    echo "Running with $thread_count threads:"
    average_time=$(calculate_average_time $thread_count)
    echo "Average time: $average_time seconds"
    echo "----------------------------------------"
done
