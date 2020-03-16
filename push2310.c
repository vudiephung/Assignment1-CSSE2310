#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

struct Myfile {
	bool validBoard;
	char Winners;
	int rows;
	int cols;
	int numOfCols; // 2 * cols + 1 : Number of cols to insert into array
	char turn;
	char **data;
};

struct Player { // X O
	char *type; // 0 1 H
};

void print_board(struct Myfile* myStruct, FILE* file) {
	char **data = myStruct->data;
	int rows = myStruct->rows;
	int numOfCols = myStruct->numOfCols;
	
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < numOfCols; c++) {
			if (data[r][c] == '\n') {
				fprintf(file, "\n");
			}
			else fprintf(file, "%c", data[r][c]); 
		}
	}
}

bool is_valid_insert_at_top(int col){
	if (r == 0) {
		bool invalids = c == 0 || c == cols - 1 || data[1][colToInsert] == '.';
		bool isEmptyShell = false;
		for (int r = 1; r < rows; r++) {
			if (data[r][colToInsert] == '.') {
				if (shift) {
					for (int shiftIndex = r; shiftIndex > 0; shiftIndex--) {
						data[shiftIndex][colToInsert] = data[shiftIndex - 1][colToInsert];
					}
				}
				isEmptyShell = true;
				break;
			}
		}
		if (invalids || !isEmptyShell) {
			return false;
		}
	} 
}

bool is_valid_insert(struct Myfile* myStruct, int r, int c, bool shift) {
	int rows = myStruct->rows; // rows of board
	int cols = myStruct->cols; // cols of board 
	int rowToInsert = r; // position in data
	int colToInsert = 2 * c + 1; // position in data
	char **data = myStruct->data;

	if (r >= rows || c >= cols) {
		return false;
	}

	if (data[rowToInsert][colToInsert] != '.') {
		return false;
	}

	if (r == 0) {
		bool invalids = c == 0 || c == cols - 1 || data[1][colToInsert] == '.';
		bool isEmptyShell = false;
		for (int r = 1; r < rows; r++) {
			if (data[r][colToInsert] == '.') {
				if (shift) {
					for (int shiftIndex = r; shiftIndex > 0; shiftIndex--) {
						data[shiftIndex][colToInsert] = data[shiftIndex - 1][colToInsert];
					}
				}
				isEmptyShell = true;
				break;
			}
		}
		if (invalids || !isEmptyShell) {
			return false;
		}
		// if (is_valid_insert_at_top(myStruct)){
		// 	return true;
		// } else {
		// 	return false;
		// }
	} 

	if (r == rows - 1) {
		bool invalids = c == 0 || c == cols - 1 || data[rows - 2][colToInsert] == '.';
		bool isEmptyShell = false;
		for (int r = rows - 2; r >= 0; r--) {
				if (data[r][colToInsert] == '.') {
					if (shift) {
						for (int shiftIndex = r; shiftIndex < rows - 1; shiftIndex++) {
							data[shiftIndex][colToInsert] = data[shiftIndex + 1][colToInsert];
						}
					}
					isEmptyShell = true;
					break;
			}
		}
		if (invalids || !isEmptyShell) {
			return false;
		}
	}

	if (c == 0) {
		bool invalids = data[rowToInsert][colToInsert + 2] == '.' ||
						data[rowToInsert][2 * cols - 1] != '.';
		bool isEmptyShell = false;
		for (int r = 3; r < 2 *cols; r += 2) {
				if (data[rowToInsert][r] == '.') {
					if (shift) {
						for (int shiftIndex = r; shiftIndex >= 3; shiftIndex -= 2) {
							data[rowToInsert][shiftIndex] = data[rowToInsert][shiftIndex - 2];
						}
					}
					isEmptyShell = true;
					break;
			}
		}
		if (invalids || !isEmptyShell) {
			return false;
		}
	}

	if (c == cols - 1) {
		bool invalids = data[rowToInsert][colToInsert - 2] == '.' ||
						data[rowToInsert][1] != '.';
		bool isEmptyShell = false;
		for (int r = 2 * cols - 3; r >= 1; r-=2) {
				if (data[rowToInsert][r] == '.') {
					if (shift) {
						for (int shiftIndex = r; shiftIndex < 2 * cols - 2; shiftIndex += 2) {
							data[rowToInsert][shiftIndex] = data[rowToInsert][shiftIndex + 2];
						}
					}
					isEmptyShell = true;
					break;
			}
		}
		if (invalids || !isEmptyShell) {
			return false;
		}
	}

	return true;
}

bool is_end_game(struct Myfile* myStruct) {
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

void find_winner(struct Myfile* myStruct) {
	char **data = myStruct->data;
	int rows = myStruct->rows;
	int numOfCols = myStruct->numOfCols;
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

void insert_board(FILE* file, struct Myfile* myStruct, int R, int C) {
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

void read_board(FILE* file, struct Myfile* myStruct) {
	char **data = myStruct->data;
	int numOfrows = myStruct->rows;
	int numOfCols = myStruct->numOfCols;
	int next = 0;
	myStruct->validBoard = true;

	while (1) {
		next = fgetc(file);
		if (next == '\n') {
			next = fgetc(file);
			break;
		}
	}

	for (int r = 0; r < numOfrows; r++) {
		if (!myStruct->validBoard) {
			break;
		}

		for (int c = 0; c < numOfCols; c++) {
			// check numbers of 4 edges must be 0
			if (r == 0 || r == numOfrows - 1) {
				if (c > 1 && c < numOfCols - 3 && c % 2 == 0) {
					if ((char)next-'0' != 0) {
						myStruct->validBoard = false;
						break;
					}
				}
			} else {
				if (c == 0 || c == numOfCols - 3) {
					if ((char)next-'0' != 0) {
						myStruct->validBoard = false;
						break;
					}
				}
			}

			if (next == '\n') {
				data[r][c] = '\n';
				next = fgetc(file);
			} else {
				if ((char)next-'0' == 0 ||
					(char)next-'0' == 1 ||
					(char)next-'0' == 2 ||
					(char)next-'0' == 3 ||
					(char)next-'0' == 4 ||
					(char)next-'0' == 5 ||
					(char)next-'0' == 6 ||
					(char)next-'0' == 7 ||
					(char)next-'0' == 8 ||
					(char)next-'0' == 9 ||
					(char)next == 'X' 	||
					(char)next == 'O' 	||
					(char)next == '.' 	||
					(char)next == ' ' 	
				) {
					data[r][c] = (char)next;
					next = fgetc(file);
				} else {
					myStruct->validBoard = false;
					break;
				}
			}
		}
	}

	if (!myStruct->validBoard) return;

	int rowsCheck = numOfrows - 1;
	int colsCheck = numOfCols - 3;

	if (data[rowsCheck][colsCheck] != ' ' || data[rowsCheck][colsCheck+ 1] != ' ') {
		myStruct->validBoard = false;
		return;
	}
	
	myStruct->data = data;
}

void free_data(struct Myfile* myStruct, char **data) {
	int numOfrows = myStruct->rows;

	for (int r = 0; r < numOfrows ; r++) {
		free(data[r]);
	}

	free(data);
}

void handle_type_1(FILE* file, struct Myfile* myStruct) {
	char **data = myStruct->data;
	int rows = myStruct->rows;
	int cols = myStruct->cols;
	char myTurn = myStruct->turn;
	char opponentTurn = myStruct->turn == 'X'?'O':'X';
	int rowOfDot, colOfDot;

	// top: left to right
	for (int c = 1; c < cols - 1; c++) {
		int opponentScore = 0;
		int opponentScoreLater = 0;	 
		int colToInsert = 2 * c + 1;

		if (is_valid_insert(myStruct, 0, c, false)) {
			bool isDotFound = false;

			for (int r = 1; r < rows - 1; r++) { // at that cols, go down
				if (data[r][colToInsert] == opponentTurn) {
					opponentScore += (data[r][colToInsert - 1] - '0');
				}
			}

			for (int r = 1; r < rows - 1; r++) { 
				if (isDotFound) {
					break;
				}

				if (data[r][colToInsert] == '.') {
					rowOfDot = r;
					colOfDot = colToInsert;

					for (int shiftIndex = rowOfDot; shiftIndex > 0; shiftIndex--) {
						if (data[shiftIndex][colToInsert] == opponentTurn) {
							opponentScoreLater += (data[shiftIndex + 1][colToInsert - 1] - '0');
						}
					}

					isDotFound = true;
					break;
				}
			}
			
			if (isDotFound) {
				// plus opponentScore after dot
				for (int r = rowOfDot; r < rows - 1; r++) {
					if (data[r][colToInsert] == opponentTurn) {
						opponentScoreLater += (data[r][colToInsert - 1] - '0');
					}
				}
			} else {
				for (int r = 1; r < rows - 1; r++) {
					if (data[r][colToInsert] == opponentTurn) {
						opponentScoreLater += (data[r+ 1][colToInsert - 1] - '0');
					}
				}
			}

			// compare opponentScore vs opponentScoreLater
			if (opponentScoreLater < opponentScore) {
				is_valid_insert(myStruct, 0, c, true);
				insert_board(file, myStruct, 0, c);
				printf("Player %c placed at %d %d\n", myTurn, 0, c);
				print_board(myStruct, stdout);
				return;
			}
		}
	}

	// right: top to bottom
	for (int r = 1; r < rows - 1; r++) {
		int opponentScore = 0;
		int opponentScoreLater = 0;
		if (is_valid_insert(myStruct, r, cols - 1, false)) {
			bool isDotFound = false;
			for (int c = 2 * cols - 3; c > 1; c -= 2) { // at that cols, go left
				if (data[r][c] == opponentTurn) {
					opponentScore += (data[r][c - 1] - '0');
				}
			}
			for (int c = 2 * cols - 3; c > 1; c -= 2) { // find a dot 
				if (isDotFound) {
					break;
				}
				if (data[r][c] == '.') {
					colOfDot = c;
					// Loop from right of the dot and cal score of op
					for (int shiftIndex = colOfDot; shiftIndex < 2 * cols - 1; shiftIndex += 2) {
						if (data[r][shiftIndex] == opponentTurn) {
							opponentScoreLater += data[r][shiftIndex - 3] - '0';
						}
					}
					isDotFound = true;
					break;
				}
			}
			if (isDotFound) {
				// plus opponentScore left of dot
				for (int c = 1; c < colOfDot; c += 2) {
					if (data[r][c] == opponentTurn) {
						opponentScoreLater += (data[r][c - 1] - '0');
					}
				}
			}
			else {
				for (int c = 3; c < 2 * cols - 2; c += 2) {
					if (data[r][c] == opponentTurn) {
						opponentScoreLater += (data[r][c - 3] - '0');
					}
				}
			}
			// compare opponentScore vs opponentScoreLater
			if (opponentScoreLater < opponentScore) {
				is_valid_insert(myStruct, r, cols - 1, true);
				insert_board(file, myStruct, r, cols - 1);
				printf("Player %c placed at %d %d\n", myTurn, r, cols - 1);
				print_board(myStruct, stdout);
				return;
			}
		}
	}

	// bottom: right to left
	for (int c = cols - 2; c > 0; c--) {
		int opponentScore = 0;
		int opponentScoreLater = 0;	 
		int colToInsert = 2 * c + 1;

		if (is_valid_insert(myStruct, rows - 1, c, false)) {
			bool isDotFound = false;

			for (int r = rows - 2; r > 0; r--) {
				if (data[r][colToInsert] == opponentTurn) {
					opponentScore += (data[r][colToInsert - 1] - '0');
				}
			}

			for (int r = rows - 2; r > 0; r--) { 
				if (isDotFound) {
					break;
				}

				if (data[r][colToInsert] == '.') {
					rowOfDot = r;

					for (int shiftIndex = rowOfDot; shiftIndex < rows - 1; shiftIndex++) {
						if (data[shiftIndex][colToInsert] == opponentTurn) {
							opponentScoreLater += (data[shiftIndex - 1][colToInsert - 1] - '0');
						}
					}

					isDotFound = true;
					break;
				}
			}
			
			if (isDotFound) {
				for (int r = rowOfDot; r > 0; r--) {
					if (data[r][colToInsert] == opponentTurn) {
						opponentScoreLater += (data[r][colToInsert - 1] - '0');
					}
				}
			} else {
				for (int r = 1; r < rows - 1; r++) {
					if (data[r][colToInsert] == opponentTurn) {
						opponentScoreLater += (data[r - 1][colToInsert - 1] - '0');
					}
				}
			}

			// compare opponentScore vs opponentScoreLater
			if (opponentScoreLater < opponentScore) {
				is_valid_insert(myStruct, rows - 1, c, true);
				insert_board(file, myStruct, rows - 1, c);
				printf("Player %c placed at %d %d\n", myTurn, rows - 1, c);
				print_board(myStruct, stdout);
				return;
			}
		}
	}

	// left: bottom to top
	for (int r = rows - 1; r > 0; r--) {
		int opponentScore = 0;
		int opponentScoreLater = 0;

		if (is_valid_insert(myStruct, r, 0, false)) {
			bool isDotFound = false;

			for (int c = 3; c < 2 * cols - 2; c += 2) { // at left cols -> go right
				if (data[r][c] == opponentTurn) {
					opponentScore += (data[r][c - 1] - '0');
				}
			}

			for (int c = 3; c < 2 * cols - 2; c += 2) { 
				if (isDotFound) {
					break;
				}

				if (data[r][c] == '.') {
					colOfDot = c;

					// Loop from left of the dot and cal score of op
					for (int shiftIndex = 3; shiftIndex<colOfDot; shiftIndex += 2) {
						if (data[r][shiftIndex] == opponentTurn) {
							opponentScoreLater += data[r][shiftIndex + 1] - '0';
						}
					}

					isDotFound = true;
					break;
				}
			}

			if (isDotFound) {

				// Loop right of dot to cal opponentScoreLater
				for (int c = colOfDot; c < 2 * cols - 2; c += 2) {
					if (data[r][c] == opponentTurn) {
						opponentScoreLater += (data[r][c - 1] - '0');
					}
				}
			} else {
				for (int c = 3; c < 2 * cols - 2; c += 2) {
					if (data[r][c] == opponentTurn) {
						opponentScoreLater += (data[r][c + 1] - '0');
					}
				}
			}

			// compare opponentScore vs opponentScoreLater
			if (opponentScoreLater < opponentScore) {
				is_valid_insert(myStruct, r, 0, true);
				insert_board(file, myStruct, r, 0);
				printf("Player %c placed at %d %d\n", myTurn, r, 0);
				print_board(myStruct, stdout);

				return;
			}
		}
	}

	// Insert at interior
	for (int score=9; score > 0; score--) {
		for (int r = 1; r < rows - 1; r++) {
			for (int c = 1; c < cols - 1; c++) {
				if ((data[r][c * 2]-'0') == score && data[r][c * 2 + 1] == '.') {
					insert_board(file, myStruct, r, c);
					printf("Player %c placed at %d %d\n", myTurn, r, c);
					print_board(myStruct, stdout);

					return;
				}
			}
		}
	}
}

int main(int argc, char **argv) {
	if (argc != 4) {
		fprintf(stderr, "Usage: push2310 typeO typeX fname\n");
		fflush(stderr);

		return 1;	
	}
	
	// check player type 
	for (int r = 1; r < 3; r++) {
		if (strcmp(argv[r], "0") && strcmp(argv[r], "1") && strcmp(argv[r], "H")) {
			fprintf(stderr, "Invalid player Type\n");
			fflush(stderr);

			return 2;
		}
	}

	struct Player playerO, playerX;
	playerO.type = argv[1];
	playerX.type = argv[2];

	// Check give file name
	char *fileName = argv[3];
	FILE* file;
	file = fopen(fileName, "r");

	if (file == NULL) {
		fprintf(stderr, "No file to load from\n");
		fflush(stderr);

		return 3;
	}

	struct Myfile myStruct;
	int temp;
	int count = 0;

	while (fscanf(file, "%d", &temp) == 1) {
 		count++;

		if (count == 1) {
			myStruct.rows = temp;
		} else if (count == 2) {
			myStruct.cols = temp;
		}
 	}

	if (count != 2 || myStruct.rows < 3 || myStruct.cols < 3) {
		fprintf(stderr, "Invalid file contents\n");
		fflush(stderr);

		return 4;
	}

	//Read the second line && get turn X:O
	int next = fgetc(file);
	char turn = (char)next;

	if (turn != 'X' && turn != 'O') {
		fprintf(stderr, "Invalid file contents\n");
		fflush(stderr);
		return 4;
	}

	myStruct.turn = turn;

	int numOfrows = myStruct.rows;
	int numOfCols = myStruct.cols * 2 + 1;
	char **data = malloc(sizeof(char *) * numOfrows);

	for (int r = 0; r < numOfrows ; r++) {
		data[r] = malloc(sizeof(char) * numOfCols);
	}

	myStruct.data = data;
	myStruct.numOfCols = numOfCols;

	read_board(file, &myStruct);

	if (!myStruct.validBoard) {
		fprintf(stderr, "Invalid file contents\n");
		fflush(stderr);
		free_data(&myStruct, data);

		return 4;
	}

	if (is_end_game(&myStruct)) {
		fprintf(stderr, "Full board in load\n");

		return 6;
	}

	print_board(&myStruct, stdout);

	// Base on player type 
	int R, C, bufPosition, returnValue;
 	char line[50];

	while (!is_end_game(&myStruct)) {
		char input[30];
		char *track = input;
		bool isDotIncluded = false;
		char currentType = (turn == 'X') ? *playerX.type : *playerO.type; // 0 1 H

		turn = myStruct.turn; // X or O

		if (currentType == 'H') {
			printf("%c:(R C)> ", turn);

			char *getInput = fgets(line, sizeof(line), stdin);
				if (getInput == NULL) {
				fprintf(stderr, "End of file\n");
				
				return 5;
			}

			returnValue = sscanf(line, "%s %n", input, &bufPosition);
			
			while (*track != '\0') {
				if (*track == '.') {
						isDotIncluded = true;
						break;
				}

				track = track+ 1;			
			}

			if (returnValue == 1 && line[bufPosition] == '\0' && input[0] == 's' && !isDotIncluded) {
				if (input[1] && input[1] != '/') {
					FILE* writtenFile;
					writtenFile = fopen((input+ 1), "w");

					fprintf(writtenFile, "%d %d\n", myStruct.rows, myStruct.cols);
					fflush(writtenFile);
					fprintf(writtenFile, "%c\n", turn);
					fflush(writtenFile);

					print_board(&myStruct, writtenFile);

					fclose(writtenFile);

					return 0;
				} else if (input[1] == '/') {
					fprintf(stderr, "Save failed\n");
				}
			}	

			returnValue = sscanf(line, "%d %d %n", &R, &C, &bufPosition);

			if (returnValue == 2 && line[bufPosition] == '\0' ) {
				bool valid = is_valid_insert(&myStruct, R, C, true);

				if (valid) {
					insert_board(file, &myStruct, R, C);
					print_board(&myStruct, stdout);
				} else continue;
			} else {
				 continue;
			}
		}

		if (currentType == '0') {
			bool finish = false;

			if (turn == 'O') { //left to right 
				for (int r = 1; r < numOfrows - 1; r++) {
					if (finish) {
						break;
					}

					for (int c = 3; c < numOfCols - 3; c += 2) {
						if (myStruct.data[r][c] == '.') {
							myStruct.data[r][c] = turn;
							printf("Player %c placed at %d %d\n", turn, r, (c - 1) / 2);
							print_board(&myStruct, stdout);
							myStruct.turn = 'X';
							finish = true;
							break;
						}
					}
				}				
			} else if (turn == 'X') {
				for (int r = numOfrows - 2; r > 0; r--) {
					if (finish) {
						break;
					}

					for (int c = numOfCols - 4; c > 1; c -= 2) {
						if (myStruct.data[r][c] == '.') {
							myStruct.data[r][c] = turn;
							printf("Player %c placed at %d %d\n", turn, r, (c - 1) / 2);
							print_board(&myStruct, stdout);
							myStruct.turn = 'O';
							finish = true;
							break;
						}
					}
				}
			}
		}

		if (currentType == '1') {
			handle_type_1(file, &myStruct);
		}
	}

	find_winner(&myStruct);

	fclose(file);

	free_data(&myStruct, data);

	return 0;
}
