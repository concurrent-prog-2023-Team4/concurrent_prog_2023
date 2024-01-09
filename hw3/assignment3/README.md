Readme file for HW3 exercise 3
------------------------------
There is a Makefile included that compiles all the files using -fsanatize=address:
-make clean
-make

For testing we use 5 files (folder /tests) that create different scenarios of red and blue cars arriving at the bridge. 
The files contain the color of the car to be created and an integer that is how many seconds later it arrives at the bridge.
The file more_reds creates 20 red cars and 4 blue cars.
The file 2_waits creates a scenario where the time the bridge is empty there are both red and blue cars waiting to pass.
The remaining 3 files create a random combination of red and blue cars

The program prints messages for when a car is created, when it is waiting and for what reason, when it enters and exits the bridge as well as information about how many cars are waiting on each side of the bridge and how many are currenty on the bridge.

There is also a kind of simulation available to see how cars pass the bridge. Each slot of bridge represented by a '_' and when one car
gets in it changes in 'r' or 'b' accordingly.
Example: b:2 _ _ _ _ r:1 the number after b: represents how many blue cars are waiting and the number after r: how many reds are waiting.
In case to run this simulation do:
-make clean
-make -f makefile_simulation

To run the program you need to give as arguments a test file and the max number of cars that can be on the brigde at once. 
The format is: ./hw3_3 tests/<filename> <max_cars>
ex: ./hw3_3 tests/input.txt 3

"Team 4"