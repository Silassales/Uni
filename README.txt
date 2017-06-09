************************
Silas Sales	0922973
CIS*1500	A1
Oct 18, 2015
************************

************
Compilation
************

1. cd to the A1 folder
2. execute the following commmad:
	gcc a1MUG.c -Wall -pedantic -std=c99 -lm -o a1MUG
3. execute the following command:
	./a1MUG

********************
Running the program
********************

The game gives instructions on how to run the game, these are:

	Welcome to Find the Mugwump!
	The goal of this game is to find a predetermined 'Mugwump' on a 10x10 grid.
	You guess by entering in a x,y coordinate to the computer (the first row/col is 0, and the last is 9).
	You have 10 guesses to guess, and if you do not find the mugwump by then you loose.
	The computer will output the current guess as a 'W' to the grid, but will not show previous guesses.
	The computer will also tell you how far away you are diagonally from the mugwump each time you guess.
	Good Luck!

That is how to run the game.

******************
Known Limitations
******************

1. If the user enters a char, the program will enter an endless loop.
2. Due to using floating point numbers and the sqrt/pow functions, distances will not be perfectly exact.
3. Players do not have the option to play again after they win/loose, they have to run the program to restart.
4. Other than ^C or ^Z there is no way to exit the progam after starting.
5. The program does not keep track of previous guesses.
