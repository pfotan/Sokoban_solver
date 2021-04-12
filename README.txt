The application is graphic Sokoban solver using SFML library.
The goal of the game is to put all red spheres in orange fields.
In one move player can move his character to adjacent square, possibly
pusing exaclty one red sphere next to him (see exact rules in the internet).
Program reads input board from the file and draws moves sequence,
which leads to the solution of given puzzle.
To compile and run you will need Linux and SFML library installed.
Use following commands:

g++ -c -std=c++11  main.cpp -I<path_to_SFML_folder>/include
g++ main.o -o sfml-app -L<path_to_SFML_folder>/lib -lsfml-graphics -lsfml-window -lsfml-system
./sfml-app

You can also modify input file to test program on different boards.

Meaning of characters in the input file:
. - free square
W - wall, which can't be moves
K - starting position of player
+ - starting position of player, which is one of the target positions at the same time
B - square, where red sphere is placed at the beginning
* - square with red sphere, which is target square at the same time

Program will accept any board as input, but will not produce solution for invalid or unsolvable boards.
