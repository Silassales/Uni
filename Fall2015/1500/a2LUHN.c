/* Silas Sales
November 29, 2015
ssales@mail.uoguelph.ca
0922973*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ARRAY_SIZE_WITHEXTRA 18 //extra so that i can check if the user entered in too many numbers
#define ARRAY_SIZE 16 //real array size, used for for loops
 
/*Reads in the 16-digit credit card number as a string and then converts it 
to the int array that is passed into the function*/
void readCC (int * creditArray); 


/*Takes the credit card array as input, check the validity by calling runLuhn
then returns the checksum to main*/
void isValid (int * creditArray);

/* calculates the checksum based on Luhn's algorithm, and returns
the checksum to isValid()*/
int runLuhn (int * creditArray);

/* main function, calls readCC and isValid */
int main()
{
     int creditArray[ARRAY_SIZE];
    	readCC(creditArray);
	isValid(creditArray);

     return 0;
}

void readCC (int * creditArray) 
{
     char charArray[ARRAY_SIZE_WITHEXTRA];
	int i = 0;
	int error = 0;

	printf("Please enter the card number, enter only integers: ");

	fgets(charArray, sizeof charArray, stdin);
	
	for(i = 0;i < ARRAY_SIZE;i++) //check to make sure no characters have been entered
     {
		if (isalpha(charArray[i]))
          {
			error = 1;
		} 
	}
	if (error == 1)
     {
		printf("You entered a character, please enter only integers. Exiting program.\n");
		exit(1);
	}


	for(i = 0; i < ARRAY_SIZE_WITHEXTRA;i++)
     {
		creditArray[i] = charArray[i] - '0';
	}

	if (creditArray[16] > 0) //checking if number is too long, the last digits should be negative
     {
		printf("That number is too long. Exiting program.\n");
		exit(1);
	}

	for(i = 0;i < ARRAY_SIZE;i++) //checking if the number is too short, the first 16 digits should be positive
     {
          if (creditArray[i] < 0) {
			error = 1;
		}
	}

	if (error == 1)
     {
		printf("That number is too short. Exiting program.\n");
		exit(1);
	}
}

void isValid (int * creditArray)
{
     int checksum = runLuhn(creditArray);

     if (checksum == 1)
     {
     printf("That card number is valid.\n");
     } else {
          printf("That card number is invalid.\n");
     }
}

int runLuhn (int * creditArray)
{
     int sumEvenPos = 0; //counting from the left
	int sumOddPos = 0; 
	int sumTotal = 0;
	int i= 0;
	int checksum;
	int checkRem = 1; //remainder which should = 0
	
	//calculate the sum of all the odd positioned ints, from the left
	for (i = 1; i < ARRAY_SIZE;i+=2)
     {
		sumOddPos += creditArray[i];
	}

	//double the even positions from the left
	for (i = 0; i < ARRAY_SIZE;i+=2)
     {
		creditArray[i] = creditArray[i] * 2;
	}

	//add up the even positions
	for (i = 0; i < ARRAY_SIZE;i+=2) 
     {
		if (creditArray[i] >= 10)
          {
			/*if the number that has been doubled is greater than 10
			% 10 will get the int in the ones place, and since a number 0-9 doubled
			cannot be greater than 18, adding one will account for the tens place */
			sumEvenPos += (creditArray[i] % 10) + 1;
          } else {
			sumEvenPos += creditArray[i];
		}
	}

	sumTotal = sumEvenPos + sumOddPos;

	checkRem = sumTotal % 10;

	if (checkRem == 0)
     {
		checksum = 1;
	} else {
		checksum = 0;
     }

     return checksum;
}


