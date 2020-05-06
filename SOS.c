#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Initialize with enum structs my variables for sos game
enum { CHARO, CHARS, EMPTY };
enum { HUMAN, COMP };
// The board
int board[9];
// Function prototypes
void InitialiseBoard();
void PrintBoard();
int HasEmpty();
void MakeMove(int sq, int side);
void MakeComputerMove();
int minimax(int depth, int isMax);
void MakeHumanMove();
int check_game();
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
void InitialiseBoard() {
	int index = 0;

	for(index = 0; index < 9; ++index) {
		board[index] = EMPTY;
	}
	int left_right = (rand() % 2);
	if (left_right == 0){
        board[3] = CHARO;
	}
    else {
        board[5] = CHARO;
    }
}

// Print the 9x9 board
void PrintBoard() {
	int index = 0;
	char pceChars[] = "OS-";

	printf("\n\nBoard:\n\n");
	for(index = 0; index < 9; ++index) {
		if(index!=0 && index%3==0) {
			printf("\n\n");
		}
		printf("%3c",pceChars[board[index]]);
	}
	printf("\n");
}

// If one EMPTY in board return 1 else 0
int HasEmpty() {
	int index = 0;

	for(index = 0; index < 9; ++index) {
		if( board[index] == EMPTY) return 1; ///CHANGE
	}
	return 0;
}

// Places move to board
void MakeMove(int sq, int side) {
	board[sq] = side;
}

// Function that makes computer move
void MakeComputerMove() {
    int MoveToMake[2] = {-1, -1};
    int bestScore = -2147483648;
    int index;
    int score;


    for (index = 0; index < 9; index++){
        if (board[index] == EMPTY){
            MakeMove(index, CHARO);
            score = minimax(0, HUMAN);
            MakeMove(index, EMPTY);
            if (score > bestScore){
                bestScore = score;
                MoveToMake[0] = index;
                MoveToMake[1] = CHARO;
            }
            MakeMove(index, CHARS);
            score = minimax(0, HUMAN);
            MakeMove(index, EMPTY);
            if (score > bestScore){
                bestScore = score;
                MoveToMake[0] = index;
                MoveToMake[1] = CHARS;
            }
        }
    }
    MakeMove(MoveToMake[0], MoveToMake[1]);
}

// AI algorithm to find best move
int minimax(int depth, int isMax){
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
            if (board[index] == EMPTY){

                MakeMove(index, CHARO);
                score = minimax(depth+1, HUMAN);
                MakeMove(index, EMPTY);
                if (score > bestScore){
                    bestScore = score;
                }

                MakeMove(index, CHARS);
                score = minimax(depth+1, HUMAN);
                MakeMove(index, EMPTY);
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
            if (board[index] == EMPTY){

                MakeMove(index, CHARO);
                score = minimax(depth+1, COMP);
                MakeMove(index, EMPTY);
                if (score < bestScore){
                    bestScore = score;
                }

                MakeMove(index, CHARS);
                score = minimax(depth+1, COMP);
                MakeMove(index, EMPTY);
                if (score < bestScore){
                    bestScore = score;
                }
            }
        }
        return bestScore;

    }
}

// Function that takes user input
void MakeHumanMove() {

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

		if( board[move]!=EMPTY) {
			move=-1;
			printf("Square not available\n");
			continue;
		}
		posOk = 1;
	}
	MoveToMake[0] = move;

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
    MakeMove(MoveToMake[0], MoveToMake[1]);
}

// Check all the possible SOS locations to determine win
int check_game(){

    int index;
    for (index = 0; index < 9; index += 3){
        //printf(" %d %d %d \n",ConvertTo25[index],ConvertTo25[index+1],ConvertTo25[index+2]);
        if ((board[index] == CHARS) && (board[index+1] == CHARO) &&(board[index+2] == CHARS))
            return 1;
    }
    for (index = 0; index < 3; index++){
       // printf(" %d %d %d \n",ConvertTo25[index],ConvertTo25[index+3],ConvertTo25[index+6]);
        if ((board[index] == CHARS) && (board[index+3] == CHARO) &&(board[index+6] == CHARS))
            return 1;
    }
    //printf(" %d %d %d \n",ConvertTo25[0],ConvertTo25[4],ConvertTo25[8]);
    if ((board[0] == CHARS) && (board[4] == CHARO) &&(board[8] == CHARS))
            return 1;
    //printf(" %d %d %d \n",ConvertTo25[2],ConvertTo25[4],ConvertTo25[6]);
    if ((board[2] == CHARS) && (board[4] == CHARO) &&(board[6] == CHARS))
            return 1;
    return -1;
}



