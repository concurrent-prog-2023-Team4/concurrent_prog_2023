Readme file for HW3 exercise 1
------------------------------
There is a Makefile included that compiles all the files using -fsanatize=address:
-make clean
-make

For testing, we use a driver program labeled "test" that shows how shared data is accessed.
The program is a library that creates binary semaphores using monitors in C.
The test iterates one shared variable between threads and we need to observe that each thread has the correct value of shared variable.
Note: this test does not stop on its own, user should use CTRL+C to stop it!

In order to run the executable type ./hw3_1_test

"Team 4"