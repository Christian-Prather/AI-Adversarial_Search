// MaxConnect-4 code

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <vector>
#include <iostream>
using namespace std;

int maxDepth = 0;
int currentDepth = 0;
int AI_PIECE = 0;
int HUMAN_PIECE = 0;

class gameStatus
{
private:
	long *gameData;

public:
	long *gameBoard[6];
	long currentTurn;
	int player1Score;
	int player2Score;
	int pieceCount;
	FILE *gameFile;

	gameStatus()
	{
		gameData = new long[42];
		gameBoard[0] = &(gameData[0]);
		gameBoard[1] = &(gameData[7]);
		gameBoard[2] = &(gameData[14]);
		gameBoard[3] = &(gameData[21]);
		gameBoard[4] = &(gameData[28]);
		gameBoard[5] = &(gameData[35]);

		int i;
		for (i = 0; i < 42; i++)
		{
			gameData[i] = 0;
		}

		currentTurn = 1;
		player1Score = 0;
		player2Score = 0;
		pieceCount = 0;
		gameFile = 0;
	}

	~gameStatus()
	{
		//delete[] gameData;
	}
};
void countScore(gameStatus &currentGame);

// Output current game status to console
void printGameBoard(gameStatus &currentGame)
{
	int i = 0;
	int j = 0;
	printf(" -----------------\n");
	for (i = 0; i < 6; i++)
	{
		printf(" | ");
		for (j = 0; j < 7; j++)
		{
			printf("%li ", currentGame.gameBoard[i][j]);
		}
		printf("| \n");
	}
	printf(" -----------------\n");
}

// Output current game status to file
void printGameBoardToFile(gameStatus &currentGame)
{

	int i = 0;
	int j = 0;
	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 7; j++)
		{
			fprintf(currentGame.gameFile, "%li", currentGame.gameBoard[i][j]);
		}
		fprintf(currentGame.gameFile, "\r\n");
	}
	fprintf(currentGame.gameFile, "%d\r\n", currentGame.currentTurn);
}

// Given a column and which player it is,
// place that player's piece in the requested column
int playPiece(int column, gameStatus &currentGame)
{
	// if column full, return 0
	if (currentGame.gameBoard[0][column] != 0)
	{
		return 0;
	}

	int i;
	// starting at the bottom of the board, place the piece into the
	// first empty spot
	for (i = 5; i >= 0; i--)
	{
		if (currentGame.gameBoard[i][column] == 0)
		{
			currentGame.gameBoard[i][column] = currentGame.currentTurn;
			currentGame.pieceCount++;
			return 1;
		}
	}
	return 0;
}

void undoPiece(int column, gameStatus &currentGame)
{
	int i;
	// starting at the bottom of the board, place the piece into the
	// first empty spot
	for (i = 0; i <= 5; i++)
	{
		if (currentGame.gameBoard[i][column] != 0)
		{
			currentGame.gameBoard[i][column] = 0;
			currentGame.pieceCount--;
			break;
		}
	}
}

// If maximize is true its the AI's turn
int minimax(bool maximize, int depth, int alpha, int beta, gameStatus &currentGame)
{
	if (depth == 0)
	{

		countScore(currentGame);

		if (AI_PIECE == 1)
		{
			return currentGame.player1Score - currentGame.player2Score;
		}
		else
		{
			return currentGame.player2Score - currentGame.player1Score;
		}
	}
	if (currentGame.pieceCount == 42)
	{
		countScore(currentGame);
		if (AI_PIECE == 1)
		{
			return currentGame.player1Score - currentGame.player2Score;
		}
		else
		{
			return currentGame.player2Score - currentGame.player1Score;
		}
	}

	if (maximize)
	{
		currentGame.currentTurn = AI_PIECE;
		int maxScore = -INFINITY;
		for (int i = 0; i < 7; i++)
		{
			// Play piece then reset
			int result = playPiece(i, currentGame);

			// Column not available
			if (result != 1)
			{
				continue;
			}

			int score = minimax(!maximize, depth - 1, alpha, beta, currentGame);
			// Fix for when AI is not player 1
			if (score > maxScore)
			{
				maxScore = score;
			}
			alpha = max(alpha, score);
			undoPiece(i, currentGame);

			if (beta <= alpha)
			{
				break;
			}
		}
		return maxScore;
	}
	else
	{
		if (AI_PIECE == 1)
		{
			currentGame.currentTurn = 2;
		}
		else
		{
			currentGame.currentTurn = 1;
		}

		int minScore = INFINITY;
		for (int i = 0; i < 7; i++)
		{
			// Play piece then reset
			int result = playPiece(i, currentGame);

			// Column not available
			if (result != 1)
			{
				continue;
			}

			int score = minimax(!maximize, depth - 1, alpha, beta, currentGame);
			// Fix for when AI is not player 1
			if (score < minScore)
			{
				minScore = score;
			}
			beta = min(beta, score);
			undoPiece(i, currentGame);

			if (beta <= alpha)
			{
				break;
			}
		}
		return minScore;
	}
}

// The AI section.  Currently plays randomly.
void aiPlay(gameStatus &currentGame)
{
	int maxScore = -INFINITY;
	int column = -1;
	for (int i = 0; i < 7; i++)
	{
		cout << "////////////////" << endl;
		currentGame.currentTurn = AI_PIECE;
		int result = playPiece(i, currentGame);
		if (result == 0)
		{
			continue;
		}
		int score = minimax(false, maxDepth, -INFINITY, INFINITY, currentGame);
		cout << "Score " << score << endl;
		undoPiece(i, currentGame);
		// Determin which one to check based on which player AI is;
		// int score = currentGame.player1Score;
		if (score > maxScore)
		{
			maxScore = score;
			column = i;
		}
	}

	if (column < 0)
	{
		cout << "ERROR" << endl;
		return;
	}
	currentGame.currentTurn = AI_PIECE;

	int result = playPiece(column, currentGame);
	if (result == 0)
	{
		cout << "ERRRORRRRR" << endl;
	}
	printf("\n\nmove %li: Player %li, column %li\n",
		   currentGame.pieceCount, currentGame.currentTurn, column + 1);
	if (currentGame.currentTurn == 1)
		currentGame.currentTurn = 2;
	else if (currentGame.currentTurn == 2)
		currentGame.currentTurn = 1;
}

void humanPlay(gameStatus &currentGame)
{
	currentGame.currentTurn = HUMAN_PIECE;
	cout << "Please make a move... enter column  (1-7)" << endl;
	int userInput;
	cin >> userInput;
	userInput = userInput - 1;
	while (userInput<0 | userInput> 6)
	{
		cout << "Please enter valid option... " << endl;
		cin >> userInput;
		userInput = userInput - 1;
	}
	int result = playPiece(userInput, currentGame);
	if (result == 0)
	{
		cout << "Column full pick again ..." << endl;
		humanPlay(currentGame);
	}
	if (currentGame.currentTurn == 1)
		currentGame.currentTurn = 2;
	else if (currentGame.currentTurn == 2)
		currentGame.currentTurn = 1;
}

void countScore(gameStatus &currentGame)
{
	currentGame.player1Score = 0;
	currentGame.player2Score = 0;

	//check horizontally
	// Row
	int inlineCounter = 0;

	for (int i = 0; i < 6; i++)
	{
		//check player 1
		// Check one row at a time [row][column]
		inlineCounter = 0;
		// Column
		for (int j = 0; j < 7; j++)
		{
			if (currentGame.gameBoard[i][j] == 1)
			{
				inlineCounter++;
				if (inlineCounter >= 4)
				{
					inlineCounter = 0;
					currentGame.player1Score++;
					break;
				}
			}
			else
			{
				inlineCounter = 0;
			}
		}

		//check player 2

		inlineCounter = 0;
		for (int j = 0; j < 7; j++)
		{
			if (currentGame.gameBoard[i][j] == 2)
			{
				inlineCounter++;
				if (inlineCounter >= 4)
				{
					inlineCounter = 0;
					currentGame.player2Score++;
					break;
				}
			}
			else
			{
				inlineCounter = 0;
			}
		}
	}

	// check vertically
	for (int j = 0; j < 7; j++)
	{
		//check player 1
		inlineCounter = 0;
		for (int i = 0; i < 6; i++)
		{
			if (currentGame.gameBoard[i][j] == 1)
			{
				inlineCounter++;
				if (inlineCounter >= 4)
				{
					inlineCounter = 0;
					currentGame.player1Score++;
					break;
				}
			}
			else
			{
				inlineCounter = 0;
			}
		}

		// 	//check player 2
		inlineCounter = 0;
		for (int i = 0; i < 6; i++)
		{
			if (currentGame.gameBoard[i][j] == 2)
			{
				inlineCounter++;
				if (inlineCounter >= 4)
				{
					inlineCounter = 0;
					currentGame.player2Score++;
					break;
				}
			}
			else
			{
				inlineCounter = 0;
			}
		}
	}

	//check diagonally

	//check player 1
	// Top left
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (currentGame.gameBoard[i][j] == 1 && (currentGame.gameBoard[i][j] == currentGame.gameBoard[i + 1][j + 1]) && (currentGame.gameBoard[i][j] == currentGame.gameBoard[i + 2][j + 2]) && (currentGame.gameBoard[i][j] == currentGame.gameBoard[i + 3][j + 3]))
			{
				currentGame.player1Score++;
			}
		}
	}

	// Top right
	for (int i = 0; i < 3; i++)
	{
		for (int j = 3; j < 7; j++)
		{
			if (currentGame.gameBoard[i][j] == 1 && (currentGame.gameBoard[i][j] == currentGame.gameBoard[i + 1][j - 1]) && (currentGame.gameBoard[i][j] == currentGame.gameBoard[i + 2][j - 2]) && (currentGame.gameBoard[i][j] == currentGame.gameBoard[i + 3][j - 3]))
			{
				currentGame.player1Score++;
			}
		}
	}

	//check player 2
	// Top Left
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (currentGame.gameBoard[i][j] == 2 && (currentGame.gameBoard[i][j] == currentGame.gameBoard[i + 1][j + 1]) && (currentGame.gameBoard[i][j] == currentGame.gameBoard[i + 2][j + 2]) && (currentGame.gameBoard[i][j] == currentGame.gameBoard[i + 3][j + 3]))
			{
				currentGame.player2Score++;
			}
		}
	}

	// Top right
	for (int i = 0; i < 3; i++)
	{
		for (int j = 3; j < 7; j++)
		{
			if (currentGame.gameBoard[i][j] == 2 && (currentGame.gameBoard[i][j] == currentGame.gameBoard[i + 1][j - 1]) && (currentGame.gameBoard[i][j] == currentGame.gameBoard[i + 2][j - 2]) && (currentGame.gameBoard[i][j] == currentGame.gameBoard[i + 3][j - 3]))
			{
				currentGame.player2Score++;
			}
		}
	}
	// cout << "Count ran .." << endl;
}

void saveFile(char *file, gameStatus &currentGame)
{
	FILE *output = fopen(file, "w");
	if (output != 0)
	{
		printGameBoardToFile(currentGame);
		fclose(currentGame.gameFile);
	}
	else
	{
		printf("error: could not open output file %s\n", output);
	}
}
int main(int argc, char **argv)
{
	char **command_line = argv;

	if (argc != 5)
	{
		printf("Four command-line arguments are needed:\n");
		printf("Usage: %s interactive [input_file] [computer-next/human-next] [depth]\n", command_line[0]);
		printf("or:  %s one-move [input_file] [output_file] [depth]\n", command_line[0]);

		return 0;
	}

	gameStatus currentGame; // Declare current game

	char *game_mode = command_line[1];
	char *output = nullptr;
	char *nextUp = nullptr;

	if (strcmp(game_mode, "interactive") == 0)
	{
		nextUp = command_line[3];
	}
	else if (strcmp(game_mode, "one-move") == 0)
	{
		output = command_line[3];
	}
	else
	{
		printf("%s is an unrecognized game mode\n", game_mode);
		return 0;
	}
	char *input = command_line[2];
	maxDepth = atoi(command_line[4]);

	printf("\nMaxConnect-4 game\n");

	currentGame.gameFile = fopen(input, "r");
	printf("game state before move:\n");

	//set currentTurn
	char current = 0;
	int i, j;
	if (currentGame.gameFile != 0)
	{
		for (i = 0; i < 6; i++)
		{
			for (j = 0; j < 7; j++)
			{
				do
				{
					current = getc(currentGame.gameFile);
				} while ((current == ' ') || (current == '\n') || (current == '\r'));

				currentGame.gameBoard[i][j] = current - 48;
				if (currentGame.gameBoard[i][j] > 0)
				{
					currentGame.pieceCount++;
				}
			}
		}

		do
		{
			current = getc(currentGame.gameFile);
		} while ((current == ' ') || (current == '\n') || (current == '\r'));

		currentGame.currentTurn = current - 48;
		fclose(currentGame.gameFile);
	}
	cout << "Stuff" << endl;
	printGameBoard(currentGame);
	cout << "Printed" << endl;
	countScore(currentGame);
	printf("Score: Player 1 = %d, Player 2 = %d\n\n", currentGame.player1Score, currentGame.player2Score);

	// Seed random number generator
	int seed = time(NULL);
	srand(seed);

	if (currentGame.pieceCount == 42)
	{
		printf("\nBOARD FULL\n");
		printf("Game over!\n\n");

		return 1;
	}
	if (strcmp(game_mode, "one-move") == 0)
	{
		cout << "agaseg " << endl;
		AI_PIECE = currentGame.currentTurn;
		cout << AI_PIECE << "....." << endl;
		aiPlay(currentGame);
	}
	else if (strcmp(game_mode, "interactive") == 0)
	{

		char nextPlayer = 'N';
		// Assign correct piece to AI player
		if (strcmp(nextUp, "computer-next") == 0)
		{

			// Set AI piece to the piece
			nextPlayer = 'A';
			AI_PIECE = currentGame.currentTurn;
			if (AI_PIECE == 1)
			{
				HUMAN_PIECE = 2;
			}
			else
			{
				HUMAN_PIECE = 1;
			}
		}
		else
		{
			nextPlayer = 'H';
			if (currentGame.currentTurn == 1)
			{
				AI_PIECE = 2;
			}
			else
			{
				AI_PIECE = 1;
			}
		}

		while (currentGame.pieceCount != 42)
		{
			if (nextPlayer == 'A')
			{
				currentGame.currentTurn = AI_PIECE;
				cout << "AI up" << endl;
				aiPlay(currentGame);
				saveFile("computer.txt", currentGame);
				nextPlayer = 'H';
			}
			else if (nextPlayer == 'H')
			{
				currentGame.currentTurn = HUMAN_PIECE;
				humanPlay(currentGame);
				saveFile("human.txt", currentGame);
				nextPlayer = 'A';
			}

			printf("game state after move:\n");
			printGameBoard(currentGame);
			countScore(currentGame);
			printf("Score: Player 1 = %d, Player 2 = %d\n\n", currentGame.player1Score, currentGame.player2Score);
		}
	}

	printf("game state after move:\n");
	printGameBoard(currentGame);
	cout << "Printed" << endl;
	countScore(currentGame);
	printf("Score: Player 1 = %d, Player 2 = %d\n\n", currentGame.player1Score, currentGame.player2Score);

	currentGame.gameFile = fopen(output, "w");
	if (currentGame.gameFile != 0)
	{
		printGameBoardToFile(currentGame);
		fclose(currentGame.gameFile);
	}
	else
	{
		printf("error: could not open output file %s\n", output);
	}
	return 1;
}