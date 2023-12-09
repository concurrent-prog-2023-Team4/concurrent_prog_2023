Readme file for HW2 exercise 1
------------------------------
There is a Makefile included that compiles all the files using -fsanatize=address:
-make clean
-make

For testing, we use a driver program labeled "test" that shows how shared data is accessed.
The program is a library that creates binary semaphores using system_v system calls.
The test iterates one shared library between threads and we need toobserve that each 
thread has the correct value of shared library.
Note: this test does not stop on its own, user should use CTRL+c to stop it!

In order to run the executable type ./test

"Team 4"