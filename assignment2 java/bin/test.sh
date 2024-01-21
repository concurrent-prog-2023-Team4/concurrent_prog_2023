#!/bin/bash

# Run the Java program 20 times
for i in {1..100}
do
    echo "Running iteration $i"
    java assignment2.hw3_2 5 /home/dimitris/Desktop/concurrent_prog_2023/hw2/assignment2/tests/input.txt
done

