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
	cout << "PIECE " << currentGame.currentTurn;
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
	// if (currentGame.gameBoard[0][column] != 0)
	// {
	// 	return 0;
	// }

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
//AI is maximizing
int minimax(gameStatus currentGame, bool AI, int depth, int aiPiece)
{
	printGameBoard(currentGame);
	countScore(currentGame);
	cout << currentGame.player1Score << " " << currentGame.player2Score << endl;
	// Base case
	if (depth == maxDepth)
	{
		if (AI_PIECE == 1)
		{
			countScore(currentGame);
			return currentGame.player1Score - currentGame.player2Score;
		}
		else
		{
			countScore(currentGame);
			return currentGame.player2Score - currentGame.player1Score;
		}

		// Player 1 = +1, Player 2 = -1
	}
	int maxScore = -INFINITY;
	int minScore = INFINITY;
	int minColumn = 0;
	int maxColumn = 0;
	for (int column = 0; column < 7; column++)
	{
		bool foundOne = false;
		for (int row = 5; row >= 0; row--)
		{
			// Find first open row for current column
			if (currentGame.gameBoard[row][column] == 0)
			{
				foundOne = true;
				if (AI)
				{
					currentGame.gameBoard[row][column] = currentGame.currentTurn;
					if (currentGame.currentTurn == 1)
						currentGame.currentTurn = 2;
					else if (currentGame.currentTurn == 2)
						currentGame.currentTurn = 1;
					int score = minimax(currentGame, false, depth + 1, aiPiece);
					currentGame.gameBoard[row][column] = 0;
					if (currentGame.currentTurn == 1)
						currentGame.currentTurn = 2;
					else if (currentGame.currentTurn == 2)
						currentGame.currentTurn = 1;
					maxScore = max(score, maxScore);
				}
				// Set spot for human check for AI
				else
				{

					currentGame.gameBoard[row][column] = currentGame.currentTurn;
					if (currentGame.currentTurn == 1)
						currentGame.currentTurn = 2;
					else if (currentGame.currentTurn == 2)
						currentGame.currentTurn = 1;
					int score = minimax(currentGame, true, depth + 1, aiPiece);
					currentGame.gameBoard[row][column] = 0;
					if (currentGame.currentTurn == 1)
						currentGame.currentTurn = 2;
					else if (currentGame.currentTurn == 2)
						currentGame.currentTurn = 1;
					minScore = min(minScore, score);
					//cout << "SCORE: " << score;
					//cout << " Checking Human... " << minScore << endl;
				}
			}
			if (foundOne)
			{
				break;
			}
		}
	}
	if (AI)
	{
		//cout << "MaxScore: " << maxScore << endl;
		return maxScore;
	}
	else
	{
		// cout << "MinScore: " << minScore << endl;
		return minScore;
	}
}

// The AI section.  Currently plays randomly.
void aiPlay(gameStatus &currentGame)
{
	int maxScore = -INFINITY;
	int selectColumn = 0;
	AI_PIECE = currentGame.currentTurn;
	for (int column = 0; column < 7; column++)
	{
		bool foundOne = false;
		for (int row = 5; row >= 0; row--)
		{
			// Find first open row for current column
			if (currentGame.gameBoard[row][column] == 0)
			{
				foundOne = true;
				currentGame.gameBoard[row][column] = currentGame.currentTurn;
				cout << "FFFFF" << endl;
				if (currentGame.currentTurn == 1)
					currentGame.currentTurn = 2;
				else if (currentGame.currentTurn == 2)
					currentGame.currentTurn = 1;
				int score = minimax(currentGame, false, currentDepth, currentGame.currentTurn);
				currentGame.gameBoard[row][column] = 0;
				if (currentGame.currentTurn == 1)
					currentGame.currentTurn = 2;
				else if (currentGame.currentTurn == 2)
					currentGame.currentTurn = 1;

				cout << "FIN COL : " << column << " " << score << endl;
				if (score > maxScore)
				{
					maxScore = score;
					selectColumn = column;
				}
				// 	cout << "COLUMN: ///////////////////////////////// " << column << " MaxScore: " << maxScore << endl;
			}
			if (foundOne)
			{
				break;
			}
		}
	}
	cout << "FINAL MAX SCORE: " << maxScore << "Select Column: " << selectColumn << endl;

	cout << "Prior" << endl;

	// if (result == 0)
	// {
	// 	cout << "Full column" << endl;
	// 	//aiPlay(currentGame);
	// }

	// else
	// {
	playPiece(selectColumn, currentGame);
	printGameBoard(currentGame);
	printf("\n\nmove %li: Player %li, column %li\n",
		   currentGame.pieceCount, currentGame.currentTurn, selectColumn + 1);
	if (currentGame.currentTurn == 1)
		currentGame.currentTurn = 2;
	else if (currentGame.currentTurn == 2)
		currentGame.currentTurn = 1;
	// }
}

void countScore(gameStatus &currentGame)
{
	currentGame.player1Score = 0;
	currentGame.player2Score = 0;

	//check horizontally
	int i;
	for (i = 0; i < 6; i++)
	{
		//check player 1
		if (currentGame.gameBoard[i][0] == 1 && currentGame.gameBoard[i][1] == 1 && currentGame.gameBoard[i][2] == 1 && currentGame.gameBoard[i][3] == 1)
		{
			currentGame.player1Score++;
		}
		if (currentGame.gameBoard[i][1] == 1 && currentGame.gameBoard[i][2] == 1 && currentGame.gameBoard[i][3] == 1 && currentGame.gameBoard[i][4] == 1)
		{
			currentGame.player1Score++;
		}
		if (currentGame.gameBoard[i][2] == 1 && currentGame.gameBoard[i][3] == 1 && currentGame.gameBoard[i][4] == 1 && currentGame.gameBoard[i][5] == 1)
		{
			currentGame.player1Score++;
		}
		if (currentGame.gameBoard[i][3] == 1 && currentGame.gameBoard[i][4] == 1 && currentGame.gameBoard[i][5] == 1 && currentGame.gameBoard[i][6] == 1)
		{
			currentGame.player1Score++;
		}
		//check player 2
		if (currentGame.gameBoard[i][0] == 2 && currentGame.gameBoard[i][1] == 2 && currentGame.gameBoard[i][2] == 2 && currentGame.gameBoard[i][3] == 2)
		{
			currentGame.player2Score++;
		}
		if (currentGame.gameBoard[i][1] == 2 && currentGame.gameBoard[i][2] == 2 && currentGame.gameBoard[i][3] == 2 && currentGame.gameBoard[i][4] == 2)
		{
			currentGame.player2Score++;
		}
		if (currentGame.gameBoard[i][2] == 2 && currentGame.gameBoard[i][3] == 2 && currentGame.gameBoard[i][4] == 2 && currentGame.gameBoard[i][5] == 2)
		{
			currentGame.player2Score++;
		}
		if (currentGame.gameBoard[i][3] == 2 && currentGame.gameBoard[i][4] == 2 && currentGame.gameBoard[i][5] == 2 && currentGame.gameBoard[i][6] == 2)
		{
			currentGame.player2Score++;
		}
	}

	//check vertically
	int j;
	for (j = 0; j < 7; j++)
	{
		//check player 1
		if (currentGame.gameBoard[0][j] == 1 && currentGame.gameBoard[1][j] == 1 && currentGame.gameBoard[2][j] == 1 && currentGame.gameBoard[3][j] == 1)
		{
			currentGame.player1Score++;
		}
		if (currentGame.gameBoard[1][j] == 1 && currentGame.gameBoard[2][j] == 1 && currentGame.gameBoard[3][j] == 1 && currentGame.gameBoard[4][j] == 1)
		{
			currentGame.player1Score++;
		}
		if (currentGame.gameBoard[2][j] == 1 && currentGame.gameBoard[3][j] == 1 && currentGame.gameBoard[4][j] == 1 && currentGame.gameBoard[5][j] == 1)
		{
			currentGame.player1Score++;
		}
		//check player 2
		if (currentGame.gameBoard[0][j] == 2 && currentGame.gameBoard[1][j] == 2 && currentGame.gameBoard[2][j] == 2 && currentGame.gameBoard[3][j] == 2)
		{
			currentGame.player2Score++;
		}
		if (currentGame.gameBoard[1][j] == 2 && currentGame.gameBoard[2][j] == 2 && currentGame.gameBoard[3][j] == 2 && currentGame.gameBoard[4][j] == 2)
		{
			currentGame.player2Score++;
		}
		if (currentGame.gameBoard[2][j] == 2 && currentGame.gameBoard[3][j] == 2 && currentGame.gameBoard[4][j] == 2 && currentGame.gameBoard[5][j] == 2)
		{
			currentGame.player2Score++;
		}
	}

	//check diagonally

	//check player 1
	if (currentGame.gameBoard[2][0] == 1 && currentGame.gameBoard[3][1] == 1 && currentGame.gameBoard[4][2] == 1 && currentGame.gameBoard[5][3] == 1)
	{
		currentGame.player1Score++;
	}
	if (currentGame.gameBoard[1][0] == 1 && currentGame.gameBoard[2][1] == 1 && currentGame.gameBoard[3][2] == 1 && currentGame.gameBoard[4][3] == 1)
	{
		currentGame.player1Score++;
	}
	if (currentGame.gameBoard[2][1] == 1 && currentGame.gameBoard[3][2] == 1 && currentGame.gameBoard[4][3] == 1 && currentGame.gameBoard[5][4] == 1)
	{
		currentGame.player1Score++;
	}
	if (currentGame.gameBoard[0][0] == 1 && currentGame.gameBoard[1][1] == 1 && currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][3] == 1)
	{
		currentGame.player1Score++;
	}
	if (currentGame.gameBoard[1][1] == 1 && currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][3] == 1 && currentGame.gameBoard[4][4] == 1)
	{
		currentGame.player1Score++;
	}
	if (currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][3] == 1 && currentGame.gameBoard[4][4] == 1 && currentGame.gameBoard[5][5] == 1)
	{
		currentGame.player1Score++;
	}
	if (currentGame.gameBoard[0][1] == 1 && currentGame.gameBoard[1][2] == 1 && currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][4] == 1)
	{
		currentGame.player1Score++;
	}
	if (currentGame.gameBoard[1][2] == 1 && currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][4] == 1 && currentGame.gameBoard[4][5] == 1)
	{
		currentGame.player1Score++;
	}
	if (currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][4] == 1 && currentGame.gameBoard[4][5] == 1 && currentGame.gameBoard[5][6] == 1)
	{
		currentGame.player1Score++;
	}
	if (currentGame.gameBoard[0][2] == 1 && currentGame.gameBoard[1][3] == 1 && currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][5] == 1)
	{
		currentGame.player1Score++;
	}
	if (currentGame.gameBoard[1][3] == 1 && currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][5] == 1 && currentGame.gameBoard[4][6] == 1)
	{
		currentGame.player1Score++;
	}
	if (currentGame.gameBoard[0][3] == 1 && currentGame.gameBoard[1][4] == 1 && currentGame.gameBoard[2][5] == 1 && currentGame.gameBoard[3][6] == 1)
	{
		currentGame.player1Score++;
	}

	if (currentGame.gameBoard[0][3] == 1 && currentGame.gameBoard[1][2] == 1 && currentGame.gameBoard[2][1] == 1 && currentGame.gameBoard[3][0] == 1)
	{
		currentGame.player1Score++;
	}
	if (currentGame.gameBoard[0][4] == 1 && currentGame.gameBoard[1][3] == 1 && currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][1] == 1)
	{
		currentGame.player1Score++;
	}
	if (currentGame.gameBoard[1][3] == 1 && currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][1] == 1 && currentGame.gameBoard[4][0] == 1)
	{
		currentGame.player1Score++;
	}
	if (currentGame.gameBoard[0][5] == 1 && currentGame.gameBoard[1][4] == 1 && currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][2] == 1)
	{
		currentGame.player1Score++;
	}
	if (currentGame.gameBoard[1][4] == 1 && currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][2] == 1 && currentGame.gameBoard[4][1] == 1)
	{
		currentGame.player1Score++;
	}
	if (currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][2] == 1 && currentGame.gameBoard[4][1] == 1 && currentGame.gameBoard[5][0] == 1)
	{
		currentGame.player1Score++;
	}
	if (currentGame.gameBoard[0][6] == 1 && currentGame.gameBoard[1][5] == 1 && currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][3] == 1)
	{
		currentGame.player1Score++;
	}
	if (currentGame.gameBoard[1][5] == 1 && currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][3] == 1 && currentGame.gameBoard[4][2] == 1)
	{
		currentGame.player1Score++;
	}
	if (currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][3] == 1 && currentGame.gameBoard[4][2] == 1 && currentGame.gameBoard[5][1] == 1)
	{
		currentGame.player1Score++;
	}
	if (currentGame.gameBoard[1][6] == 1 && currentGame.gameBoard[2][5] == 1 && currentGame.gameBoard[3][4] == 1 && currentGame.gameBoard[4][3] == 1)
	{
		currentGame.player1Score++;
	}
	if (currentGame.gameBoard[2][5] == 1 && currentGame.gameBoard[3][4] == 1 && currentGame.gameBoard[4][3] == 1 && currentGame.gameBoard[5][2] == 1)
	{
		currentGame.player1Score++;
	}
	if (currentGame.gameBoard[2][6] == 1 && currentGame.gameBoard[3][5] == 1 && currentGame.gameBoard[4][4] == 1 && currentGame.gameBoard[5][3] == 1)
	{
		currentGame.player1Score++;
	}

	//check player 2
	if (currentGame.gameBoard[2][0] == 2 && currentGame.gameBoard[3][1] == 2 && currentGame.gameBoard[4][2] == 2 && currentGame.gameBoard[5][3] == 2)
	{
		currentGame.player2Score++;
	}
	if (currentGame.gameBoard[1][0] == 2 && currentGame.gameBoard[2][1] == 2 && currentGame.gameBoard[3][2] == 2 && currentGame.gameBoard[4][3] == 2)
	{
		currentGame.player2Score++;
	}
	if (currentGame.gameBoard[2][1] == 2 && currentGame.gameBoard[3][2] == 2 && currentGame.gameBoard[4][3] == 2 && currentGame.gameBoard[5][4] == 2)
	{
		currentGame.player2Score++;
	}
	if (currentGame.gameBoard[0][0] == 2 && currentGame.gameBoard[1][1] == 2 && currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][3] == 2)
	{
		currentGame.player2Score++;
	}
	if (currentGame.gameBoard[1][1] == 2 && currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][3] == 2 && currentGame.gameBoard[4][4] == 2)
	{
		currentGame.player2Score++;
	}
	if (currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][3] == 2 && currentGame.gameBoard[4][4] == 2 && currentGame.gameBoard[5][5] == 2)
	{
		currentGame.player2Score++;
	}
	if (currentGame.gameBoard[0][1] == 2 && currentGame.gameBoard[1][2] == 2 && currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][4] == 2)
	{
		currentGame.player2Score++;
	}
	if (currentGame.gameBoard[1][2] == 2 && currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][4] == 2 && currentGame.gameBoard[4][5] == 2)
	{
		currentGame.player2Score++;
	}
	if (currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][4] == 2 && currentGame.gameBoard[4][5] == 2 && currentGame.gameBoard[5][6] == 2)
	{
		currentGame.player2Score++;
	}
	if (currentGame.gameBoard[0][2] == 2 && currentGame.gameBoard[1][3] == 2 && currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][5] == 2)
	{
		currentGame.player2Score++;
	}
	if (currentGame.gameBoard[1][3] == 2 && currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][5] == 2 && currentGame.gameBoard[4][6] == 2)
	{
		currentGame.player2Score++;
	}
	if (currentGame.gameBoard[0][3] == 2 && currentGame.gameBoard[1][4] == 2 && currentGame.gameBoard[2][5] == 2 && currentGame.gameBoard[3][6] == 2)
	{
		currentGame.player2Score++;
	}

	if (currentGame.gameBoard[0][3] == 2 && currentGame.gameBoard[1][2] == 2 && currentGame.gameBoard[2][1] == 2 && currentGame.gameBoard[3][0] == 2)
	{
		currentGame.player2Score++;
	}
	if (currentGame.gameBoard[0][4] == 2 && currentGame.gameBoard[1][3] == 2 && currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][1] == 2)
	{
		currentGame.player2Score++;
	}
	if (currentGame.gameBoard[1][3] == 2 && currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][1] == 2 && currentGame.gameBoard[4][0] == 2)
	{
		currentGame.player2Score++;
	}
	if (currentGame.gameBoard[0][5] == 2 && currentGame.gameBoard[1][4] == 2 && currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][2] == 2)
	{
		currentGame.player2Score++;
	}
	if (currentGame.gameBoard[1][4] == 2 && currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][2] == 2 && currentGame.gameBoard[4][1] == 2)
	{
		currentGame.player2Score++;
	}
	if (currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][2] == 2 && currentGame.gameBoard[4][1] == 2 && currentGame.gameBoard[5][0] == 2)
	{
		currentGame.player2Score++;
	}
	if (currentGame.gameBoard[0][6] == 2 && currentGame.gameBoard[1][5] == 2 && currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][3] == 2)
	{
		currentGame.player2Score++;
	}
	if (currentGame.gameBoard[1][5] == 2 && currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][3] == 2 && currentGame.gameBoard[4][2] == 2)
	{
		currentGame.player2Score++;
	}
	if (currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][3] == 2 && currentGame.gameBoard[4][2] == 2 && currentGame.gameBoard[5][1] == 2)
	{
		currentGame.player2Score++;
	}
	if (currentGame.gameBoard[1][6] == 2 && currentGame.gameBoard[2][5] == 2 && currentGame.gameBoard[3][4] == 2 && currentGame.gameBoard[4][3] == 2)
	{
		currentGame.player2Score++;
	}
	if (currentGame.gameBoard[2][5] == 2 && currentGame.gameBoard[3][4] == 2 && currentGame.gameBoard[4][3] == 2 && currentGame.gameBoard[5][2] == 2)
	{
		currentGame.player2Score++;
	}
	if (currentGame.gameBoard[2][6] == 2 && currentGame.gameBoard[3][5] == 2 && currentGame.gameBoard[4][4] == 2 && currentGame.gameBoard[5][3] == 2)
	{
		currentGame.player2Score++;
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

	char *game_mode = command_line[1];

	if (strcmp(game_mode, "interactive") == 0)
	{
		printf("interactive mode is currently not implemented\n");
		return 0;
	}
	else if (strcmp(game_mode, "one-move") != 0)
	{
		printf("%s is an unrecognized game mode\n", game_mode);
		return 0;
	}

	char *input = command_line[2];
	char *output = command_line[3];
	maxDepth = atoi(command_line[4]);

	gameStatus currentGame; // Declare current game
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

	printGameBoard(currentGame);
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

	aiPlay(currentGame);
	printf("game state after move:\n");
	printGameBoard(currentGame);
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