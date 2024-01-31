Readme file for HW4 exercise 1
------------------------------
There is a Makefile included that compiles all the files using -fsanatize=address:
-make clean
-make

The excersise consists of a library of functions to use coroutines.
For testing, we modified the 1st assignment of the 1st homework given to use the coroutines we created.


In order to run the executable type ./hw4_1 hw4_1_test/tests/input.txt (alternatively input_large.txt -- text from wikipedia)

The program writes the output on a file out.copy and then user should run
command "diff out.copy hw4_1_test/tests/<input_file>" in order to check if output is succesful.

"Team 4"