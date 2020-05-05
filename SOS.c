#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
int board[25] = {
	:,:,:,:,:,
	:,O,-,S,:,
	:,S,O,-,:,
	:,-,-,-,:,
	:,:,:,:,:,
}
*/

enum { CHARO, CHARS, BORDER, EMPTY };
enum { HUMAN, COMP };

int ConvertTo25[9] = {
	6, 7, 8,
	11,12,13,
	16,17,18
};

void InitialiseBoard(int *board);
void PrintBoard(int *board);
int HasEmpty(int *board);
void MakeMove(int *board, int sq, int side);
void MakeComputerMove(int *board);
int minimax(int *board, int depth, int isMax);
void MakeHumanMove(int *board);
int check_game(int *board);
void RunGame();

int main() {
	srand(time(NULL));
	RunGame();

	return 0;
}

// Main Function to run the game
void RunGame() {

	int GameOver = 0;
	int Side = COMP;
	int board[25];

	InitialiseBoard(board);
	PrintBoard(board);

	while(!GameOver) {
		if(Side==HUMAN) {
			MakeHumanMove(board);
			Side=COMP;
			PrintBoard(board);
		} else {
			MakeComputerMove(board);
			Side=HUMAN;
			PrintBoard(board);
		}

		// if three in a row exists Game is over
		if (check_game(board) == 1){
            printf("Game over!\n");
			GameOver = 1;
			if(Side==HUMAN) {
				printf("Computer Wins\n");
				return;
			} else {
				printf("Human Wins\n");
				return;
			}
		}

		// if no more moves, game is a draw
		if(!HasEmpty(board)) {
			printf("Game over!\n");
			GameOver = 1;
			printf("It's a draw\n");
			return;
		}
	}
}

// Function to create starting board with O at position 4 or 6
void InitialiseBoard(int *board) {
	int index = 0;

	for(index = 0; index < 25; index++) {
		board[index] = BORDER;
	}

	for(index = 0; index < 9; index++) {
		board[ConvertTo25[index]] = EMPTY;
	}
	int left_right = (rand() % 2);
	if (left_right == 0){
        board[ConvertTo25[3]] = CHARO;
	}
    else {
        board[ConvertTo25[5]] = CHARO;
    }
}

// Print the 3x3 board
void PrintBoard(int *board) {
	int index = 0;
	char pceChars[] = "OS|-";

	printf("\n\nBoard:\n\n");
	for(index = 0; index < 9; index++) {
		if(index!=0 && index%3==0) {
			printf("\n\n");
		}
		printf("%4c",pceChars[board[ConvertTo25[index]]]);
	}
	printf("\n");
}

// If one EMPTY in board return 1 else 0
int HasEmpty(int *board) {
	int index = 0;

	for(index = 0; index < 9; index++) {
		if( board[ConvertTo25[index]] == EMPTY) return 1;
	}
	return 0;
}

// Places move to board
void MakeMove(int *board, int sq, int side) {
	board[sq] = side;
}

// Function that makes computer move
void MakeComputerMove(int *board) {
    int MoveToMake[2] = {-1, -1};
    int bestScore = -2147483648;
    int index;
    int score;


    for (index = 0; index < 9; index++){
        if (board[ConvertTo25[index]] == EMPTY){
            MakeMove(board, ConvertTo25[index], CHARO);
            score = minimax(board, 0, HUMAN);
            MakeMove(board, ConvertTo25[index], EMPTY);
            if (score > bestScore){
                bestScore = score;
                MoveToMake[0] = index;
                MoveToMake[1] = CHARO;
            }
            MakeMove(board, ConvertTo25[index], CHARS);
            score = minimax(board, 0, HUMAN);
            MakeMove(board, ConvertTo25[index], EMPTY);
            if (score > bestScore){
                bestScore = score;
                MoveToMake[0] = index;
                MoveToMake[1] = CHARS;
            }
        }
    }
    MoveToMake[0] = ConvertTo25[MoveToMake[0]];
    MakeMove(board, MoveToMake[0], MoveToMake[1]);
}

// AI algorithm to find best move
int minimax(int *board, int depth, int isMax){
    if (check_game(board) == 1){
        if (isMax == HUMAN){
           return 1;
        }
        if (isMax == COMP){
            return -1;
        }
    }
    if(!HasEmpty(board)) {
        return 0; //tie
    }

    if (isMax == COMP){
        int bestScore = -2147483648;
        int index;
        int score;

        for (index = 0; index < 9; index++){
            if (board[ConvertTo25[index]] == EMPTY){

                MakeMove(board, ConvertTo25[index], CHARO);
                score = minimax(board, depth+1, HUMAN);
                MakeMove(board, ConvertTo25[index], EMPTY);
                if (score > bestScore){
                    bestScore = score;
                }

                MakeMove(board, ConvertTo25[index], CHARS);
                score = minimax(board, depth+1, HUMAN);
                MakeMove(board, ConvertTo25[index], EMPTY);
                if (score > bestScore){
                    bestScore = score;
                }
            }
        }
        return bestScore;

    } else { //(isMax == HUMAN)
        int bestScore = 2147483647;
        int index;
        int score;

        for (index = 0; index < 9; index++){
            if (board[ConvertTo25[index]] == EMPTY){

                MakeMove(board, ConvertTo25[index], CHARO);
                score = minimax(board, depth+1, COMP);
                MakeMove(board, ConvertTo25[index], EMPTY);
                if (score < bestScore){
                    bestScore = score;
                }

                MakeMove(board, ConvertTo25[index], CHARS);
                score = minimax(board, depth+1, COMP);
                MakeMove(board, ConvertTo25[index], EMPTY);
                if (score < bestScore){
                    bestScore = score;
                }
            }
        }
        return bestScore;

    }
}

// Function that takes user input
void MakeHumanMove(int *board) {

	char userInput[4];
	int MoveToMake[2] = {-1,-1};
	int posOk = 0;
	int charOk = 0;
	char SO;
	int move = -1;

	while (posOk == 0) {

		printf("Please enter a move from 1 to 9:");
		fgets(userInput, 3, stdin);
		fflush(stdin);

		if(strlen(userInput) != 2) {
			printf("Invalid strlen()\n");
			continue;
		}

		if( sscanf(userInput, "%d", &move) != 1) {
			move = -1;
			printf("Invalid sscanf()\n");
			continue;
		}

		if( move < 1 || move > 9) {
			move = -1;
			printf("Invalid range\n");
			continue;
		}

		move--; // Zero indexing

		if( board[ConvertTo25[move]]!=EMPTY) {
			move=-1;
			printf("Square not available\n");
			continue;
		}
		posOk = 1;
	}
	MoveToMake[0] = ConvertTo25[move];

    while (charOk == 0) {

		printf("Please enter S or O:");
		fgets(userInput, 3, stdin);
		fflush(stdin);

		if(strlen(userInput) != 2) {
			printf("Invalid strlen()\n");
			continue;
		}

		if( sscanf(userInput, "%c", &SO) != 1) {
			printf("Invalid sscanf()\n");
			continue;
		}

		if( SO == 'S') {
            MoveToMake[1] = CHARS;
            charOk = 1;
            break;
		}
		if( SO == 'O') {
            MoveToMake[1] = CHARO;
            charOk = 1;
            break;
		}
		printf("Invalid character\n");
	}

	printf("Making Move...%d-%c\n",(move+1), SO);
	//printf("MoveArray: %d,%d\n",moveArray[0], moveArray[1]);
    MakeMove(board, MoveToMake[0], MoveToMake[1]);
}

// Check all the possible SOS locations to determine win
int check_game(int *board){

    int index;
    for (index = 0; index < 9; index += 3){
        //printf(" %d %d %d \n",ConvertTo25[index],ConvertTo25[index+1],ConvertTo25[index+2]);
        if ((board[ConvertTo25[index]] == CHARS) && (board[ConvertTo25[index+1]] == CHARO) &&(board[ConvertTo25[index+2]] == CHARS))
            return 1;
    }
    for (index = 0; index < 3; index++){
       // printf(" %d %d %d \n",ConvertTo25[index],ConvertTo25[index+3],ConvertTo25[index+6]);
        if ((board[ConvertTo25[index]] == CHARS) && (board[ConvertTo25[index+3]] == CHARO) &&(board[ConvertTo25[index+6]] == CHARS))
            return 1;
    }
    //printf(" %d %d %d \n",ConvertTo25[0],ConvertTo25[4],ConvertTo25[8]);
    if ((board[ConvertTo25[0]] == CHARS) && (board[ConvertTo25[4]] == CHARO) &&(board[ConvertTo25[8]] == CHARS))
            return 1;
    //printf(" %d %d %d \n",ConvertTo25[2],ConvertTo25[4],ConvertTo25[6]);
    if ((board[ConvertTo25[2]] == CHARS) && (board[ConvertTo25[4]] == CHARO) &&(board[ConvertTo25[6]] == CHARS))
            return 1;
    return -1;
}



