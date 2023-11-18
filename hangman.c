#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

// This is the start menu, where the user decides if they want to start the game.
void startMenu() {
	char chosenCharacter;
	printf("Press S to start playing the game!\n");
	printf("Press Q to quit!\n");
	chosenCharacter = _getch();
	if (chosenCharacter == 'S' || chosenCharacter == 's') {
		system("cls");
	}
	else if (chosenCharacter == 'Q' || chosenCharacter == 'q') {
		system("cls");
		printf("That's fine. I hope you try again later!\n");
		Sleep(2000);
		exit(0);
	}
	else {
		system("cls");
		printf("Please enter either S or Q, not any different letter!\n");
		startMenu();
	}
}
// The idea of this Menu is to allow the user to select a difficulty, each difficulty allows the user a specific number of mistakes to make before the game terminates.
int difficultyMenu(int* difficulty) {
	char chosenCharacter1;
	printf("Welcome to the Hangman game! Will you prevail, or lose?\n");
	printf("There is a total of four difficulties in this game, beginner, intermediate, hard, and EXTREME.\n");
	printf("Press B for beginner mode.\n");
	printf("Press I for intermediate mode.\n");
	printf("Press H for hard mode.\n");
	printf("Press E for extreme mode.\n\n");
	chosenCharacter1 = _getch();
	// Difficulty based on the user input.
	if (chosenCharacter1 == 'B' || chosenCharacter1 == 'b') {
		printf("You have chosen beginner mode.\n");
		*difficulty = 1;
	}
	else if (chosenCharacter1 == 'I' || chosenCharacter1 == 'i') {
		printf("You have chosen intermediate mode.\n");
		*difficulty = 2;
	}
	else if (chosenCharacter1 == 'H' || chosenCharacter1 == 'h') {
		printf("You have chosen hard mode.\n");
		*difficulty = 3;
	}
	else if (chosenCharacter1 == 'E' || chosenCharacter1 == 'e') {
		printf("You have chosen EXTREME mode!\nWARNING: be prepared >:(\n");
		*difficulty = 4;
	}
	else {
		system("cls");
		printf("Please enter a valid letter!\n");
		difficultyMenu(difficulty);
	}
	return *difficulty;
}
// Function to convert a string to lowercase.
char* toLowercase(char* str) {
	if (str == NULL) return NULL;
	char* originalStr = str; // Saving the original pointer.
	while (*str) {
		*str = tolower((unsigned char)*str);
		str++;
	}
	return originalStr; // Returning the original pointer.
}
// Function of finding a random word that has the same number of letters as requested by the user.
char* randomWord(int wordLength) {
	FILE* fptr = NULL;
	errno_t err;
	int maxLines = wordLength < 14 ? 3000 : 25; // Assuming 3000 lines in words.txt and 25 in longwords.txt, which is the case in my files.
	int randomLine = (rand() % maxLines) + 1;

	// Open the appropriate file
	err = wordLength < 14 ? fopen_s(&fptr, "words.txt", "r") : fopen_s(&fptr, "longwords.txt", "r");
	if (err != 0 || fptr == NULL) {
		return NULL;
	}

	char* word = (char*)malloc((wordLength + 2) * sizeof(char)); // +2 because we're taking into account the newline and null terminator
	if (word == NULL) {
		fclose(fptr);
		return NULL;
	}

	int currentLine = 0;
	while (fgets(word, wordLength + 2, fptr)) {
		currentLine++;
		if (currentLine == randomLine) {
			// Checking if the word is of the desired length (excluding the newline character)
			if (strlen(word) == wordLength + 1 && word[wordLength] == '\n') {
				word[wordLength] = '\0'; // Remove newline
				fclose(fptr);
				return word;
			}
			break; // Break if we reached the random line but it's not of the desired length.
		}
	}
	fclose(fptr);
	free(word);
	return randomWord(wordLength); // Recursive call if the word wasn't found.
}
void playHangman() {
	FILE* fptr;
	int wordLength;
	startMenu();
	int difficulty;
	int valid;
	difficultyMenu(&difficulty);
	// Scanning the number of letters of the word from the user.
	do {
		printf("Please enter the length of the word you would like to guess.\n");
		printf("Note that the length must be an integer between 2 and 15.\n");

		valid = scanf_s("%d", &wordLength);

		// Clearing the input buffer to handle non-integer inputs
		while (getchar() != '\n');

		if (valid == 1 && wordLength >= 2 && wordLength <= 15) {
			break; // Valid input, so break out of the loop.
		}
		else {
			printf("Error! Please enter a different, valid word length between 2 and 15.\n");
		}

	} while (1); // Will only break out on valid input, otherwise an infinite loop.
	printf("The length of the word you'll have to guess is %d.\nGood luck!\n", wordLength);
	// Relation between difficulty level and number of mistakes the user is allowed to make.
	int mistakes;
	if (difficulty == 1)
		mistakes = 10;
	else if (difficulty == 2)
		mistakes = 8;
	else if (difficulty == 3)
		mistakes = 6;
	else if (difficulty == 4)
		mistakes = 4;
	printf("You are permitted %d mistakes!\n", mistakes);
	//Creating the random word, and the underscore array which will be displayed to the user.
	char* randomw = randomWord(wordLength);
	randomw = toLowercase(randomw);
	char* underscores = (char*)malloc(wordLength * sizeof(char));
	for (int i = 0; i < wordLength; i++) {
		underscores[i] = '_';
		printf("%c ", underscores[i]);
	}
	// Variables that help in the main loop below, iterator is for the game to keep going, keeps incrementing by 1.
	int iterator = 1;
	int win;
	// The following wrongCharacters array will be used to store the user's wrong guesses.
	char wrongCharacters[10] = { 0 };
	int counter = 0;
	// Main loop that allows the user to play the game!
	for (int k = 0; k <= iterator; k++) {
		int letterRepeat = 0;
		win = 1;
		int guess = 0;
		printf("\nEnter a letter:\n");
		char gamechar;
		do {
			gamechar = _getch(); // Get one character input from the user.
			gamechar = tolower((unsigned char)gamechar); // Convert to lowercase.

			// If it's not an alphabetic character, keep prompting.
			if (!isalpha((unsigned char)gamechar)) {
				printf("Please enter a letter.\n");
			}

		} while (!isalpha((unsigned char)gamechar));
		// Scenario when the user enters a letter they have already entered previously.
		for (int i = 0; i < wordLength; i++) {
			if (gamechar == underscores[i] || gamechar == wrongCharacters[i]) {
				printf("You have already entered this letter previously!\n");
				letterRepeat = 1;
				break;
			}
			// If the guessed character was a correct one.
			if (randomw[i] == gamechar) {
				underscores[i] = gamechar;
				guess = 1;
				if (letterRepeat == 0)
					printf("Good job! %c is in the word, at position %d!\n", gamechar, (i + 1));
				continue;
			}
		}
		// Win is the variable that decides if the user won or not.
		for (int p = 0; p < wordLength; p++) {
			if (underscores[p] == '_')
				win = 0;
		}
		// Winning scenario!
		if (win == 1) {
			for (int i = 0; i < wordLength; i++) {
				printf("%c", underscores[i]);
			}
			printf("\nCongratulations, you've won the game!\n");
			printf("You still had %d mistake(s) left!\n", mistakes);
			printf("Thanks for playing! :)\n");
			Sleep(2000);
			return;
		}
		// If the guessed character was a wrong one.
		if (guess == 0 && letterRepeat == 0) {
			mistakes--;
			printf("Unfortunate! %c is not in the word.\n", gamechar);
			printf("You have %d mistake(s) left.\n", mistakes);
			wrongCharacters[counter] = gamechar;
			counter++;
			printf("Your wrong guesses (in order) are: ");
			for (int i = 0; i <= counter; i++) {
				if (wrongCharacters[i] != '\0')
					printf("%c,", wrongCharacters[i]);
			}
			printf("\n");
		}
		// Printing the word with underscores to represent missing letters, as well as the losing scenario!
		for (int i = 0; i < wordLength; i++) {
			if (mistakes == 0) {
				printf("You've lost the game! The word was %s\nBetter luck next time x_x!\n", randomw);
				Sleep(2000);
				return;
			}
			printf("%c ", underscores[i]);
		}
		iterator++;
	}
	free(randomw);
	free(underscores);

}
// Main function!
int main(void) {
	// Seed to allow the program to give a random number.
	srand(time(0));
	char playAgain;

	do {
		playHangman(); // Play a round of Hangman.
		printf("Would you like to play again? Enter Y if so, and enter anything else if you'd like to quit. ");
		scanf_s(" %c", &playAgain); // A space is added before %c to skip any leftover newline characters.

		while (getchar() != '\n'); // Clears the input buffer.

	} while (playAgain == 'Y' || playAgain == 'y');

	printf("I hope you return soon!\n");
	return 0;
}
