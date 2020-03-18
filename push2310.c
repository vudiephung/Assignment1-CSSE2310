#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

// struct Myfile defines needed variables for a given board
struct Myfile {
    int rows;			// given rows
    int cols;			// given columns
    int numOfCols;		// number of cols to insert into array
    char turn;			// turn of next player: X or O
    char **data;		// store the information of the board
    bool validBoard;	// determine whether the given board is valid or not
    int exitCode;		// exitCode of the program: From 0 to 6
    char Winners;		// winner of the game
};

// struct Player defines type of Player O and Player X: 0 1H
struct Player {
    char *type; 		// 0 1 H
};

// function print_board take the struct and the given file pointer 
// as parameters to write board on that given file
void print_board(struct Myfile *myStruct, FILE* file) {
    char **data = myStruct->data;
    int rows = myStruct->rows;
    int numOfCols = myStruct->cols * 2 + 1;
	
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < numOfCols; c++) {
			if (data[r][c] == '\n') {
				fprintf(file, "\n");
			}
			else {
				fprintf(file, "%c", data[r][c]); 
			} 
        }
    }
}

// function free_data delete the allocated memory
// and number of rows as the parametere
void free_data(char **data, int rows) {
    for (int r = 0; r < rows ; r++) {
        free(data[r]);
    }

    free(data);
}

// Handle errors of exit code 1,2,3
// Take argc, argv of main() function as parameters
int handle_error_123(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: push2310 typeO typeX fname\n");
        fflush(stderr);

        return 1;	
    }
	
    for (int index = 1; index < 3; index++) {
        if (strcmp(argv[index], "0") && 
				strcmp(argv[index], "1") && 
				strcmp(argv[index], "H")) {

            fprintf(stderr, "Invalid player type\n");
            fflush(stderr);

            return 2;
        }
    }

    char *fileName = argv[3];
    FILE* file;
    file = fopen(fileName, "r");

    if (file == NULL) {
        fprintf(stderr, "No file to load from\n");
        fflush(stderr);

        return 3;
    }

    return 0;
}

// this function returns a file pointer to a given file name that
// took from the argv[3] of main() function
FILE* get_file_to_read(char **argv) {
    char *fileName = argv[3];
    FILE* file;
    file = fopen(fileName, "r");

    return file;
}

// this function takes the pointer to a file to read rows and columns from it
// and then save it into given struct
void read_rows_cols(FILE *file, struct Myfile *myStruct) {
    char **data = myStruct->data;
    int temp;
    int count = 0;

    while (fscanf(file, "%d", &temp) == 1) {
        count++;

        if (count == 1) {
            myStruct->rows = temp;
        } else if (count == 2) {
            myStruct->cols = temp;
        }
    }

    if (count != 2 || myStruct->rows < 3 || myStruct->cols < 3) {
        myStruct->exitCode = 4;
        myStruct->validBoard = false;
        fprintf(stderr, "Invalid file contents\n");
    }
}

// this function takes the pointer to a file, struct pointer and
//int next = fgetc(file) as parameters to read player turn of that file, 
//then save turn to a given struct
// if turn is not valid, set an exit code to struct and print the error
void read_turn(FILE *file, struct Myfile *myStruct, int *next) {
    char turn = (char)(*next);

    if (turn != 'X' && turn != 'O') {
        myStruct->exitCode = 4;
        myStruct->validBoard = false;
        fprintf(stderr, "Invalid file contents\n");
    } else {
        myStruct->turn = turn;
    }

    while (1) {
        *next = fgetc(file);
        if (*next == '\n') {
            *next = fgetc(file);
            break;
        }
    }
}

// after read rows, columns and turn, this function is then used to 
// read the board of that file, takes parameters that were already
// introduced previously in order to save board into "char **data" of struct 
void read_board(FILE *file, struct Myfile *myStruct, int *next) {
	char **data = myStruct->data;
	bool *validBoard = &myStruct->validBoard;
	int *exitCode = &myStruct->exitCode;
	int rows = myStruct->rows;
	int numOfCols = myStruct->cols * 2 + 1;

	for (int r = 0; r < rows; r++) {
		if (!(*validBoard)) {
			break;
		}

		for (int c = 0; c < numOfCols; c++) {
			if (r == 0 || r == rows - 1) {
				if (c > 1 && c < numOfCols - 3 && c % 2 == 0) {
					if ((char)(*next)-'0' != 0) {
						*validBoard = false;
						break;
					}
				}
			} else {
				if (c == 0 || c == numOfCols - 3) {
					if ((char)(*next)-'0' != 0) {
						*validBoard = false;
						break;
					}
				}
			}

			if (*next == '\n') {
				data[r][c] = '\n';
				*next = fgetc(file);
			} else {
				data[r][c] = (char)(*next);
				*next = fgetc(file);
			}
		}
	}
}

// read_file combines three previous functions take array data
void read_file(FILE *file, struct Myfile *myStruct) {
	int next = 0;
	myStruct->validBoard = true;
	myStruct->exitCode = 0;
	int numOfRows, numOfCols;

	read_rows_cols(file, myStruct);
	
	if (!myStruct->validBoard) {
		return;
	} else {
		numOfRows = myStruct->rows;
		numOfCols = myStruct->cols * 2 + 1;
	}

	next = fgetc(file);

	read_turn(file, myStruct, &next);
	
	if (!myStruct->validBoard) {
		return;
	}

	char **data = malloc(sizeof(char *) * numOfRows);
	for (int r = 0; r < numOfRows ; r++) {
		data[r] = malloc(sizeof(char) * numOfCols);
	}

	myStruct->data = data;
	
	read_board(file, myStruct, &next);
}

// the purpose of these four functions are to check whether it is valid to
// insert throw a stone at edges or not, which take the array data, 
// number of rows and columns in board, place to insert into
// edge of the board ('c' or 'r') and place to insert into array data
// ('colToInsert' or 'rowToInsert') and boolean 'shift' (determines whether
// it should push the elements along or not) as arguments
bool is_valid_insert_at_top(char **data, int rows, int cols, int c, 
		int colToInsert, bool shift) {
	bool invalids = c == 0 || c == cols - 1 || data[1][colToInsert] == '.';
	bool isEmptyShell = false;

	for (int r = 1; r < rows; r++) {
		if (data[r][colToInsert] == '.') {
			if (shift) {
				for (int shiftIndex = r; shiftIndex > 0; shiftIndex--) {
					data[shiftIndex][colToInsert] = 
						data[shiftIndex - 1][colToInsert];
				}
			}
			isEmptyShell = true;
			break;
		}
	}

	if (invalids || !isEmptyShell) {
		return false;
	} else {
		return true;
	}
}

bool is_valid_insert_at_bottom(char **data, int rows, int cols, int c, 
		int colToInsert, bool shift) {
	bool invalids = c == 0 || 
			c == cols - 1 || 
			data[rows - 2][colToInsert] == '.';
	bool isEmptyShell = false;

	for (int r = rows - 2; r >= 0; r--) {
			if (data[r][colToInsert] == '.') {
				if (shift) {
					for (int shiftIndex = r; shiftIndex < rows - 1; 
							shiftIndex++) {
						data[shiftIndex][colToInsert] = 
							data[shiftIndex + 1][colToInsert];
					}
				}
				isEmptyShell = true;
				break;
		}
	}

	if (invalids || !isEmptyShell) {
		return false;
	} else {
		return true;
	}
}

bool is_valid_insert_at_left(char **data, int rows, int cols, int r, 
			int rowToInsert, int colToInsert, bool shift) {
		bool invalids = data[rowToInsert][colToInsert + 2] == '.' ||
						data[rowToInsert][2 * cols - 1] != '.';
		bool isEmptyShell = false;

		for (int r = 3; r < 2 *cols; r += 2) {
				if (data[rowToInsert][r] == '.') {
					if (shift) {
						for (int shiftIndex = r; shiftIndex >= 3; 
								shiftIndex -= 2) {
							data[rowToInsert][shiftIndex] = 
								data[rowToInsert][shiftIndex - 2];
						}
					}
					isEmptyShell = true;
					break;
			}
		}

		if (invalids || !isEmptyShell) {
			return false;
		} else {
			return true;
		}
}

bool is_valid_insert_at_right(char **data, int rows, int cols, int r, 
		int rowToInsert, int colToInsert, bool shift) {
	bool invalids = data[rowToInsert][colToInsert - 2] == '.' ||
					data[rowToInsert][1] != '.';
	bool isEmptyShell = false;

	for (int r = 2 * cols - 3; r >= 1; r-=2) {
			if (data[rowToInsert][r] == '.') {
				if (shift) {
					for (int shiftIndex = r; shiftIndex < 2 * cols - 2; 
							shiftIndex += 2) {
						data[rowToInsert][shiftIndex] = 
							data[rowToInsert][shiftIndex + 2];
					}
				}
				isEmptyShell = true;
				break;
		}
	}

	if (invalids || !isEmptyShell) {
		return false;
	} else {
		return true;
	}
}

// combine those 4 functions that were mentioned before
// plus the check or interior
bool is_valid_insert(struct Myfile *myStruct, int r, int c, bool shift) {
	int rows = myStruct->rows; 	 // rows of board
	int cols = myStruct->cols; 	 // cols of board 
	int rowToInsert = r; 		 // position in data
	int colToInsert = 2 * c + 1; // position in data
	char **data = myStruct->data;

	if (r >= rows || c >= cols) {
		return false;
	}

	if (data[rowToInsert][colToInsert] != '.') {
		return false;
	}

	if (r == 0) {
		return is_valid_insert_at_top(data, rows, cols, c, 
			colToInsert, shift);
	} 

	if (r == rows - 1) {
		return is_valid_insert_at_bottom(data, rows, cols, c, 
			colToInsert, shift);
	}

	if (c == 0) {
		return is_valid_insert_at_left(data, rows, cols, r, 
			rowToInsert, colToInsert, shift);
	}

	if (c == cols - 1) {
		return is_valid_insert_at_right(data, rows, cols, r, 
			rowToInsert, colToInsert, shift);
	}

	return true;
}

// After being checked, the stone now can be thrown at position R, C
void insert_board(FILE* file, struct Myfile *myStruct, int R, int C) {
	char **data = myStruct->data;
	int rowToInsert = R;
	int colToInsert = 2 * C + 1;
	int rows = myStruct->rows;
	int cols = myStruct->cols;

	char *turn = &myStruct->turn;

	//Insert into position

	if (R == 0) {
		data[1][colToInsert] = *turn;
	} else if (R == rows - 1) {
		data[rows - 2][colToInsert] = *turn;
	} else if (C == 0) {
		data[rowToInsert][colToInsert + 2] = *turn;
	} else if (C == cols - 1) {
		data[rowToInsert][colToInsert - 2] = *turn;
	} 
	else data[rowToInsert][colToInsert] = *turn;

	*turn = *turn == 'X' ? 'O' : 'X'; 
	
	//Modify data
	myStruct->turn = *turn;
}

// Check whether the game is end or not by looking into interior of the board
bool is_end_game(struct Myfile *myStruct) {
	char **data = myStruct->data;
	int rows = myStruct->rows;
	int cols = myStruct->cols;
	bool isEndGame = true;

	for (int r = 1; r < rows - 1; r++) {
		for (int c=3; c < 2 * cols - 2; c+=2) {
			if (data[r][c] == '.') {
				isEndGame = false;
				break;
			}
		}

		if (isEndGame == false) {
			break;
		}
	}

	return isEndGame;
}

// Find the Winner(s) by calculating the score of each player
void find_winner(struct Myfile *myStruct) {
	char **data = myStruct->data;
	int rows = myStruct->rows;
	int numOfCols = myStruct->cols * 2 + 1;
	int scoreX = 0;
	int scoreO = 0;

	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < numOfCols; c++) {
			if (data[r][c] == 'X') {
				scoreX+= (data[r][c - 1]-'0');
			} else if (data[r][c] == 'O') {
				scoreO+= (data[r][c - 1]-'0');
			}
		}
	}

	if (scoreX > scoreO) {
		myStruct->Winners = 'X';
	} else if (scoreO > scoreX) {
		myStruct->Winners = 'O';
	} else myStruct->Winners = '?';

	if (myStruct->Winners == '?') {
		printf("Winners: %c %c\n", 'O', 'X');
	} else {
		printf("Winners: %c\n", myStruct->Winners);
	}
}

// this function run when type 0 is triggered
void handle_type0(FILE *file, struct Myfile *myStruct) {
	char* turn = &myStruct->turn;
	int numOfRows = myStruct->rows;
	int numOfCols = myStruct->cols * 2 + 1;
	bool finish = false;
	char **data = myStruct->data;

	if (*turn == 'O') { // left to right, top to bottom
		for (int r = 1; r < numOfRows - 1; r++) {
			if (finish) {
				break;
			}
			for (int c = 3; c < numOfCols - 3; c += 2) {
				if (data[r][c] == '.') {
					data[r][c] = *turn;
					printf("Player %c placed at %d %d\n", 
							*turn, r, (c - 1) / 2);
					print_board(myStruct, stdout);
					*turn = 'X';
					finish = true;
					break;
				}
			}
		}				
	} else if (*turn == 'X') { // right to left, bottom to top
		for (int r = numOfRows - 2; r > 0; r--) {
			if (finish) {
				break;
			}

			for (int c = numOfCols - 4; c > 1; c -= 2) {
				if (data[r][c] == '.') {
					data[r][c] = *turn;
					printf("Player %c placed at %d %d\n", 
							*turn, r, (c - 1) / 2);
					print_board(myStruct, stdout);
					*turn = 'O';
					finish = true;
					break;
				}
			}
		}
	}
}

// for type 1: 7 functions below
// this function is used to support one of 4 below functions and to decide
// what to do when a position on an edge is found to lower the opponent score
void handle_lower_score(FILE *file, struct Myfile *myStruct, 
		int rowToInsert, int colToInsert) {
	is_valid_insert(myStruct, rowToInsert, colToInsert, true);

	printf("Player %c placed at %d %d\n", myStruct->turn, 
		rowToInsert, colToInsert);

	insert_board(file, myStruct, rowToInsert, colToInsert);

	print_board(myStruct, stdout);
}

// these 4 functions support handle_type1()
// if one of the edge is determined to lower the opponent score, 
// it return true and otherwise
bool calculate_insert_top(FILE *file, struct Myfile *myStruct) {
	int rows = myStruct->rows;
	int cols = myStruct->cols;
	char **data = myStruct->data;
	char opponentTurn = myStruct->turn == 'X' ? 'O' : 'X';
	bool canInsert = false;

	for (int c = 1; c < cols - 1; c++) {
		int opponentScore = 0;
		int opponentScoreLater = 0;	 
		int colToInsert = 2 * c + 1;

		if (is_valid_insert(myStruct, 0, c, false)) {
			bool isDotFound = false;

			for (int r = 1; r < rows - 1; r++) {
				if (isDotFound) {
					break;
				}

				if (data[r][colToInsert] == opponentTurn) {
					opponentScore += (data[r][colToInsert - 1] - '0');
					opponentScoreLater += 
							(data[r + 1][colToInsert - 1] - '0');
				} else if (data[r][colToInsert] == '.') {
					isDotFound = true;
					break;
				}
			}

			if (opponentScoreLater < opponentScore) {
				handle_lower_score(file, myStruct, 0, c);
				canInsert = true;
				break;
			}
		}
	}

	return canInsert;
}

bool calculate_insert_right(FILE *file, struct Myfile *myStruct) {
	int rows = myStruct->rows;
	int cols = myStruct->cols;
	char **data = myStruct->data;
	char opponentTurn = myStruct->turn == 'X' ? 'O' : 'X';
	bool canInsert = false;

	for (int r = 1; r < rows - 1; r++) {
		int opponentScore = 0;
		int opponentScoreLater = 0;

		if (is_valid_insert(myStruct, r, cols - 1, false)) {
			bool isDotFound = false;

			for (int c = 2 * cols - 3; c > 1; c -= 2) {
				if (isDotFound) {
					break;
				}

				if (data[r][c] == opponentTurn) {
					opponentScore += (data[r][c - 1] - '0');
					opponentScoreLater += data[r][c - 3] - '0';
				} else if (data[r][c] == '.') {
					isDotFound = true;
					break;
				}
			}

			if (opponentScoreLater < opponentScore) {
				handle_lower_score(file, myStruct, r, cols - 1);
				canInsert = true;
				break;
			}
		}
	}

	return canInsert;
}

bool calculate_insert_bottom(FILE *file, struct Myfile *myStruct) {
	int rows = myStruct->rows;
	int cols = myStruct->cols;
	char **data = myStruct->data;
	char opponentTurn = myStruct->turn == 'X' ? 'O' : 'X';
	bool canInsert = false;

	for (int c = cols - 2; c > 0; c--) {
		int opponentScore = 0;
		int opponentScoreLater = 0;	 
		int colToInsert = 2 * c + 1;

		if (is_valid_insert(myStruct, rows - 1, c, false)) {
			bool isDotFound = false;

			for (int r = rows - 2; r > 0; r--) { 
				if (isDotFound) {
					break;
				}

				if (data[r][colToInsert] == opponentTurn) {
					opponentScore += (data[r][colToInsert - 1] - '0');
					opponentScoreLater += 
							(data[r - 1][colToInsert - 1] - '0');
				} else if (data[r][colToInsert] == '.') {
					isDotFound = true;
					break;
				}	
			}

			// compare opponentScore vs opponentScoreLater
			if (opponentScoreLater < opponentScore) {
				handle_lower_score(file, myStruct, rows - 1, c);
				canInsert = true;
				break;
			} 
		}
	}

	return canInsert;
}

bool calculate_insert_left(FILE *file, struct Myfile *myStruct) {
	int rows = myStruct->rows;
	int cols = myStruct->cols;
	char **data = myStruct->data;
	char opponentTurn = myStruct->turn == 'X' ? 'O' : 'X';
	bool canInsert = false;

	for (int r = rows - 2; r > 0; r--) {
		int opponentScore = 0;
		int opponentScoreLater = 0;

		if (is_valid_insert(myStruct, r, 0, false)) {
			bool isDotFound = false;

			for (int c = 3; c < 2 * cols - 2; c += 2) { 
				if (data[r][c] == opponentTurn) {
					opponentScore += (data[r][c - 1] - '0');
					opponentScoreLater += data[r][c + 1] - '0';
				} else if (data[r][c] == '.') {
					isDotFound = true;
					break;
				}
			}

			if (opponentScoreLater < opponentScore) {
				handle_lower_score(file, myStruct, r, 0);
				canInsert = true;
				break;
			} 
		}
	}

	return canInsert;
}

// when there are no edges to throw a stone to lower
// the opponent score, this function is used to throw a stone
// at the highest score in the interior
void insert_interior(FILE *file, struct Myfile *myStruct) {
    int rows = myStruct->rows;
    int cols = myStruct->cols;
    char **data = myStruct->data;

	for (int score=9; score > 0; score--) {
		for (int r = 1; r < rows - 1; r++) {
			for (int c = 1; c < cols - 1; c++) {
				if ((data[r][c * 2]-'0') == score &&
						data[r][c * 2 + 1] == '.') {
					printf("Player %c placed at %d %d\n", 
							myStruct->turn, r, c);
					insert_board(file, myStruct, r, c);
					print_board(myStruct, stdout);

					return;
				}
			}
		}
	}
}

// combine those 5 upper functions
void handle_type1(FILE *file, struct Myfile *myStruct) {
	// top: left to right
    if (calculate_insert_top(file, myStruct)) {
        return;
	}

	// right: top to bottom
	if (calculate_insert_right(file, myStruct)) {
		return;
	}

	// bottom: right to left
	if (calculate_insert_bottom(file, myStruct)) {
		return;
	}

	// left: bottom to top
	if (calculate_insert_left(file, myStruct)) {
		return;
	}

	// Insert at interior
	insert_interior(file, myStruct);
}

// open a file to save the playing game and close it
void handle_save_file(struct Myfile *myStruct, char* input) {
	FILE* writtenFile;
	writtenFile = fopen((input + 1), "w");

	fprintf(writtenFile, "%d %d\n", myStruct->rows, myStruct->cols);
	fflush(writtenFile);
	fprintf(writtenFile, "%c\n", myStruct->turn);
	fflush(writtenFile);		

	print_board(myStruct, writtenFile);

	fclose(writtenFile);
}

// get user input to throw a stone at the given position
// repeat request if input is invalid or after saving game to a file
// stop playing when Ctrl+D is detected or throw an error when a file name
// of saving game is invalid
bool handle_type_H(FILE *file, struct Myfile *myStruct, char *input) {
	char *turn = &myStruct->turn;
	int R, C, bufPosition, returnValue; 
 	char line[50];
	bool isValidInput = true;

	printf("%c:(R C)> ", *turn);

	char *getInput = fgets(line, sizeof(line), stdin);
	if (getInput == NULL) {
		myStruct->exitCode = 5;	
		myStruct->validBoard = false;

		fprintf(stderr, "End of file\n");
		return true;
	}

	returnValue = sscanf(line, "%s %n", input, &bufPosition);

	if (returnValue == 1 && line[bufPosition] == '\0' && input[0] == 's') {
		if (input[1] && input[1] != '/') {
			handle_save_file(myStruct, input);
		} else if (input[1] == '/') {
			fprintf(stderr, "Save failed\n");
		}
	}

	returnValue = sscanf(line, "%d %d %n", &R, &C, &bufPosition);

	if (returnValue == 2 && line[bufPosition] == '\0' && R>=0 && C>=0) {
		bool valid = is_valid_insert(myStruct, R, C, true);

		if (valid) {
			insert_board(file, myStruct, R, C);
			print_board(myStruct, stdout);
		} else {
			isValidInput = false;
		}
	} else {
		isValidInput = false;
	}

	return isValidInput;
}

// while it is not end game, detect the player type (0,1,H) and
// repeatedly play
void play_game(FILE *file, struct Myfile *myStruct, char typeOfPlayerX, 
		char typeOfPlayerO) {
	char* turn = &myStruct->turn;

	while (!is_end_game(myStruct)) {
		char input[30];
		char currentType = (*turn == 'X') ? typeOfPlayerX : typeOfPlayerO;

		if (currentType == 'H') {
			bool isValidInput = handle_type_H(file, myStruct, input);
			if (myStruct->exitCode == 5) {
				break;
			}
			if (!isValidInput) {
				continue;
			}
		}

		if (currentType == '0') {
			handle_type0(file, myStruct);
		}

		if (currentType == '1') {
			handle_type1(file, myStruct);
		}
	}
}

int main(int argc, char **argv) {
	// check error from 1-3 when running an executable file
	int errorCode = handle_error_123(argc,argv);

	if (errorCode > 0) {
		return errorCode;
	}

	// create new instance of a Myfile struct to pass into functions
	struct Myfile myStruct;

	// create a file pointer and point to where get_file_to_read() return
	// using that file pointer as an argument
	FILE *file;
	file = get_file_to_read(argv);

	read_file(file, &myStruct);

	if (!myStruct.validBoard) {
		return myStruct.exitCode;
	}

	if (is_end_game(&myStruct)) {
		fprintf(stderr, "Full board in load\n");
		return 6;
	}

	print_board(&myStruct, stdout);

	// PlayerO and PlayerX are the instances of struct Player,
	// This strucy is used to store type of each player
	struct Player playerO, playerX;
	playerO.type = argv[1];
	playerX.type = argv[2];

	play_game(file, &myStruct, *playerX.type, *playerO.type);

	if (!myStruct.validBoard) {
		return myStruct.exitCode;
	}

	find_winner(&myStruct);

	fclose(file);

	free_data(myStruct.data, myStruct.rows);

	return 0;
}
