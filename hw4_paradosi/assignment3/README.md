Readme file for HW4 exercise 3
------------------------------
There is a Makefile included that compiles all the files using -fsanatize=address:
-make clean
-make

The 3rd assignment tests the functionality of the library previously created.
It's an implementation of the writers-readers problem using semaphores. 
The program gets as input a sequence of characters (either w or r) to represent whether each thread is a writer or a reader, combined with an int to define the delay between reading the data and creating the thread.

To run the executable type ./writers-readers tests/<filename>
    Available tests:
        - random.txt: just a randomized test to ensure the program works
        - more_writers: testing starvation for readers & writer priority
        - more_readers: testing starvation for writers & access of multiple readers at a time

"Team 4"