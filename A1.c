#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/*The game of Mugwump:
To see how the game is player check the mainMenu function.*/


/*Main Menu function -> will print out basic instructions to the terminal to explain the game to the user*/
void mainMenu()
{
	printf("Welcome to Find the Mugwump!\n");
	printf("The goal of this game is to find a predetermined 'Mugwump' on a 10x10 grid.\n");
	printf("You guess by entering in an x,y coordinate to the computer (the first row/col is 0, and the last is 9).\n");
	printf("You have 10 guess to guess, and if you do not find the mugwump by then you lose.\n");
	printf("The computer will output you current guess on the grid to the screen, but will not show previous guesses.\n");
	printf("The computer will also tell you how far away you are from the mugwump each time you guess.\n");
	printf("Good Luck!\n");
}


void showBoard(int x, int y)
{
	/*Print out the grid with reference numbers around the edge and a 'W'
	where the players guess is */
	int i,j;

	/*The reason that the for loops start at -1 and not zero is because we want the first actual
	row/col to be 0,0 not 1,1 therefore since we have the refernce numbers that add an extra
	row/col we need to start at negative 1*/
	for (i = -1; i < 10; i++)
	{
		for (j = -1; j < 10; j++)
		{
			if (i == x && j == y){//To display the users guess
				if (y == 9){//incase the guess is in the last column in which case we
					//to go to a new line
					printf("W\n");
				}
				else {
					printf("W ");
				}
			}
			else {
				if (i == -1  && j == -1){//top left corner
					printf("  ");
				} else {
					 if (i == -1 && j == 9){//if it is the first row and last col go to new line
						printf("%d\n", j);
					}else {
						if (i == -1){//print out the reference numbers for row 0
							printf("%d ", j);
						}else {
							if (j == -1){//print out the refernce numbers for col 0
								printf("%d ", i);
							}else {
								if (j == 9){//if we are the end of a row go to a new line
									printf(".\n");
								}else {//if there is no special case as listed above print a '.'
									printf(". ");

								}
							}
						}
					}
				}
			}
		}
	}
}

/* Uses the distance formula to find the distance between the players guess x,y and the
mugwumps x,y*/
double distanceToWump(int pX, int pY, int mX, int mY){
	double answer = 0;

	answer = sqrt(((pow((pX - mX), 2)) + (pow((pY - mY), 2))));

	return answer;

}

int main()
{
	int guessX, guessY; //the guess x,y coordinates
	int turnCounter = 0;//count the users 10 turns
	int mX, mY;//The mugwumps position
	int end = 0;//whether to game has ended or not

	mainMenu();//prints out info about the game to the user

	//gets a random value for the mugwumps x,y coordinates
	srand(time(NULL));
	mX = (rand() % 9);
	mY = (rand() % 9);

	//printf("Mugwump: %d %d\n", mX, mY);

	while (end == 0){

		//check if the player guessed right or if they are out of turns
		if (turnCounter == 10) {
			printf("Aw so sad, you suck aka you lost\n");
			end = 1;//end == true
		} else {
			if ((guessX == mX && guessY == mY)) {
				printf("Congrats! You Won!\n");
				end = 1;
			} else {//Get the input from the user
				printf("Enter your guess (make sure there is a space between x and y):");
				scanf("%d %d", &guessX, &guessY);

				if (guessX < 0 || guessX > 9 || guessY < 0 || guessY > 9){// check for valid input
					printf("Those coordinates are not valid\n");
				}
				else {
					showBoard(guessX, guessY);//prints the grid with the users guess
					turnCounter ++;
					printf("You have %d turns left\n", (10-turnCounter));
					//prints how far way the mugwump is from the users guess
					printf("You are %.1lf units diagonally from the mugwump\n", distanceToWump(guessX, guessY, mX, mY));
				}
			}
		}

	}
	return 0;
}

