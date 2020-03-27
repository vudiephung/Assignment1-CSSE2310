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

/* Function: print_board
 * ---------------------
 * Two purposes: Display the board to the screen or save game
 * (Depends on FILE* file)
 * ---------------------
 * Parameters: 
 * myStruct: Information of the board
 * file: pointer to the file that want to write
 * (stdout or pointer to the file that want to write)
 * ---------------------
 * Return: Nothing
*/
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

/* Function: free_data
 * ---------------------
 * Delete the allocated memory
 * ---------------------
 * Parameters: 
 * data: Two dimentional array that store the 
 * information of the board
 * rows: Number of rows of the board
 * ---------------------
 * Return: Nothing
*/
void free_data(char **data, int rows) {
    for (int r = 0; r < rows ; r++) {
        free(data[r]);
    }

    free(data);
}

/* Function: handle_error_123
 * ---------------------
 * Handle error code of main()
 * from 1 to 3 
 * ---------------------
 * Parameters:
 * argc: argument count
 * argv: argument value
 * ---------------------
 * Return: error code (1 or 2 or 3)
*/
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

/* Function: get_file_to_read
 * ---------------------
 * Read the argv[3] and return file pointer to that given file name                 
 * ---------------------
 * Parameters:
 * argv: Argument value (from main() function)
 * ---------------------
 * Return: pointer to a file with respect to given file name argv[3]
*/
FILE* get_file_to_read(char **argv) {
    char *fileName = argv[3];
    FILE* file;
    file = fopen(fileName, "r");

    return file;
}

/* Function: read_rows_cols
 * ---------------------
 * Read the number of rows and colums of the board from given file pointer 
 * and then save that information to given struct if valid or set the error
 * to the struct if not valid             
 * ---------------------
 * Parameters:
 * file: file pointer to the file that want to read from
 * myStruct: After read, then save to myStruct (which save all necessary
 * things of the board)
 * ---------------------
 * Return: Nothing
*/
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

/* Function: read_turn
 * ---------------------
 * Read turn of next player of the board from given file pointer 
 * and then save that information to given struct if valid turn or 
 * set the error to the struct if turn is not valid             
 * ---------------------
 * Parameters:
 * file: file pointer to the file that want to read from
 * myStruct: After read, then save to myStruct (which save all necessary
 * things of the board)
 * next = fgetc(file): after read rows and cols, go to next line 
 * to read turn
 * ---------------------
 * Return: Nothing
*/
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

/* Function: read_board
 * ---------------------
 * Read the board from given file pointer 
 * and then save that information to **data of given struct if valid board or 
 * set the error to the struct if the board is not valid             
 * ---------------------
 * Parameters:
 * file: file pointer to the file that want to read from
 * myStruct: After read, then save to myStruct (which save all necessary
 * things of the board)
 * next = fgetc(file): after read turn of next player, go to next line 
 * to read the entire board
 * ---------------------
 * Return: Nothing
*/ 
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

/* Function: read_file
 * ---------------------
 * This function is used to handle all of 3: read_rows_cols(), read_turn()
 * and read_board     
 * ---------------------
 * Parameters:
 * file: file pointer to the file that want to read from
 * myStruct: After read from file, save value to myStruct
 * ---------------------
 * Return: Nothing
*/ 
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

// PLAY GAME
/* Function: is_valid_insert_at_top
 * ---------------------
 * Check weather it is valid to throw the stone at the upper edge
 * ---------------------
 * Parameters:
 * data: the 2D array that stores information of the board
 * rows, cols: number of rows and colums of the board respectively
 * c: Position that player want to insert to
 * the board (like 'C' in 'X:(R C)>')
 * colToInsert = 2 * c + 1: Potision in **data. 
 * For example: When player want to throw
 * a stone at (0,2), it is actually at position (0,5) in **data
 * shift: Depends on the usage, if shift == true, push the interior or donot
 * push when shift == false
 * ---------------------
 * Return: The insertion is valid or not
*/ 
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

/* Function: is_valid_insert_at_bottom
 * ---------------------
 * Check weather it is valid to throw the stone at the lower edge
 * ---------------------
 * Parameters:
 * data: the 2D array that stores information of the board
 * rows, cols: number of rows and colums of the board respectively
 * c: Position that player want to insert to
 * the board (like 'C' in 'X:(R C)>')
 * colToInsert = 2 * c + 1: Potision in **data. 
 * For example: When player want to throw
 * a stone at (5,3), it is actually at position (5,7) in **data
 * shift: Depends on the usage, if shift == true, push the interior or donot
 * push when shift == false
 * ---------------------
 * Return: The insertion is valid or not
*/ 
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

/* Function: is_valid_insert_at_left
 * ---------------------
 * Check weather it is valid to throw the stone at the left edge
 * ---------------------
 * Parameters:
 * data: the 2D array that stores information of the board
 * rows, cols: number of rows and colums of the board respectively
 * r: Position that player want to insert to
 * the board (like 'R' in 'X:(R C)>')
 * rowToInsert = r: Position to insert into **data
 * colToInsert = 2 * c + 1: Potision in **data. 
 * For example: When player want to throw
 * a stone at (5,3), it is actually at position (5,7) in **data
 * shift: Depends on the usage, if shift == true, push the interior or donot
 * push when shift == false
 * ---------------------
 * Return: The insertion is valid or not
*/ 
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

/* Function: is_valid_insert_at_right
 * ---------------------
 * Check weather it is valid to throw the stone at the right edge
 * ---------------------
 * Parameters:
 * data: the 2D array that stores information of the board
 * rows, cols: number of rows and colums of the board respectively
 * r: Position that player want to insert to
 * the board (like 'R' in 'X:(R C)>')
 * rowToInsert = r: Position to insert into **data
 * colToInsert = 2 * c + 1: Potision in **data. 
 * For example: When player want to throw
 * a stone at (5,3), it is actually at position (5,7) in **data
 * shift: Depends on the usage, if shift == true, push the interior or donot
 * push when shift == false
 * ---------------------
 * Return: The insertion is valid or not
*/ 
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

/* Function: is_valid_insert
 * ---------------------
 * This is the combination of 4 edges valid checked function above
 * Moreover, after checking at edges, this function checks at interior 
 * whether that position is empty of not
 * ---------------------
 * Parameters:
 * myStruct: get the needed information from myStruct and pass these down as
 * parameters
 * r: Position that player want to insert to
 * the board (like 'R' in 'X:(R C)>')
 * c: Position that player want to insert to
 * the board (like 'C' in 'X:(R C)>')
 * shift: Depends on the usage, if shift == true, push the interior or do not
 * push when shift == false
 * ---------------------
 * Return: The insertion is valid or not
*/ 
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

/* Function: insert_board
 * ---------------------
 * Throw the stone to the given position
 * ---------------------
 * Parameters:
 * myStruct: get the needed information from myStruct and pass these down as
 * parameters
 * r: Position that player want to insert to
 * the board (like 'R' in 'X:(R C)>')
 * c: Position that player want to insert to
 * the board (like 'C' in 'X:(R C)>')
 * ---------------------
 * Return: Nothing
*/ 
void insert_board(struct Myfile *myStruct, int r, int c) {
    char **data = myStruct->data;
    int rowToInsert = r;
    int colToInsert = 2 * c + 1;
    int rows = myStruct->rows;
    int cols = myStruct->cols;

    char *turn = &myStruct->turn;

    if (r == 0) {
        data[1][colToInsert] = *turn;
    } else if (r == rows - 1) {
        data[rows - 2][colToInsert] = *turn;
    } else if (c == 0) {
        data[rowToInsert][colToInsert + 2] = *turn;
    } else if (c == cols - 1) {
        data[rowToInsert][colToInsert - 2] = *turn;
    } 
    else data[rowToInsert][colToInsert] = *turn;

    *turn = *turn == 'X' ? 'O' : 'X'; 

    //Modify data
    myStruct->turn = *turn;
}

/* Function: is_end_game
 * ---------------------
 * Check whether the game is end or not by looking into interior of the board
 * ---------------------
 * Parameters:
 * myStruct: get the needed information from myStruct
 * ---------------------
 * Return: true if the interior of the board is full and false if not full
*/ 
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

/* Function: find_winner
 * ---------------------
 * Find the Winner(s) by calculating the score of each player
 * ---------------------
 * Parameters:
 * myStruct: get the needed information from myStruct
 * ---------------------
 * Return: Nothing
*/ 
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

// Type O
/* Function: handle_type0
 * ---------------------
 * Run when type 0 is identified
 * ---------------------
 * Parameters:
 * myStruct: get the needed information from myStruct
 * ---------------------
 * Return: Nothing
*/ 
void handle_type0(struct Myfile *myStruct) {
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

// TYPE 1
/* Function: handle_lower_score
 * ---------------------
 * For type1: After one edge was found to decrease the opponent score,
 * push the interior and then throw the stone of given position and display
 * board to the screen
 * ---------------------
 * Parameters:
 * myStruct: get the needed information from myStruct
 * ---------------------
 * Return: Nothing
*/ 
void handle_lower_score(struct Myfile *myStruct,
        int rowToInsert, int colToInsert) {
    // push the interior
    is_valid_insert(myStruct, rowToInsert, colToInsert, true);

    printf("Player %c placed at %d %d\n", myStruct->turn, 
        rowToInsert, colToInsert);

    insert_board(myStruct, rowToInsert, colToInsert);

    print_board(myStruct, stdout);
}

/* Function: calculate_insert_top
 * ---------------------
 * Loop for left to right on the top edge, look into the interior at given
 * colum to calculate whether at that position can lower score of the
 * opponent or not
 * ---------------------
 * Parameters:
 * myStruct: get the needed information from myStruct
 * ---------------------
 * Return: True if can lower score of the opponent and false otherwise
*/
bool calculate_insert_top(struct Myfile *myStruct) {
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
                handle_lower_score(myStruct, 0, c);
                canInsert = true;
                break;
            }
        }
    }

    return canInsert;
}

/* Function: calculate_insert_right
 * ---------------------
 * Loop for top to bottom on the right edge, look into the interior at given
 * row to calculate whether at that position can lower score of the
 * opponent or not
 * ---------------------
 * Parameters:
 * myStruct: get the needed information from myStruct
 * ---------------------
 * Return: True if can lower score of the opponent and false otherwise
*/
bool calculate_insert_right(struct Myfile *myStruct) {
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
                handle_lower_score(myStruct, r, cols - 1);
                canInsert = true;
                break;
            }
        }
    }

    return canInsert;
}

/* Function: calculate_insert_bottom
 * ---------------------
 * Loop for right to left on the bottom edge, look into the interior at given
 * column to calculate whether at that position can lower score of the
 * opponent or not
 * ---------------------
 * Parameters:
 * myStruct: get the needed information from myStruct
 * ---------------------
 * Return: True if can lower score of the opponent and false otherwise
*/
bool calculate_insert_bottom(struct Myfile *myStruct) {
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
                handle_lower_score(myStruct, rows - 1, c);
                canInsert = true;
                break;
            }
        }
    }

    return canInsert;
}

/* Function: calculate_insert_left
 * ---------------------
 * Loop for bottom to top on the left edge, look into the interior at given
 * row to calculate whether at that position can lower score of the
 * opponent or not
 * ---------------------
 * Parameters:
 * myStruct: get the needed information from myStruct 
 * ---------------------
 * Return: True if can lower score of the opponent and false otherwise
*/
bool calculate_insert_left(struct Myfile *myStruct) {
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
                handle_lower_score(myStruct, r, 0);
                canInsert = true;
                break;
            } 
        }
    }

    return canInsert;
}

/* Function: insert_interior
 * ---------------------
 * When there are no edges to throw a stone to lower
 * the opponent score, throw a stone
 * at the highest score in the interior
 * ---------------------
 * Parameters:
 * myStruct: get the needed information from myStruct 
 * ---------------------
 * Return: Nothing
*/
void insert_interior(struct Myfile *myStruct) {
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
                    insert_board(myStruct, r, c);
                    print_board(myStruct, stdout);

                    return;
                }
            }
        }
    }
}

/* handle_type1
 * ---------------------
 * Check for all the edges and interior to throw a stone
 * ---------------------
 * Parameters:
 * myStruct: get the needed information from myStruct
 * ---------------------
 * Return: Nothing
*/
void handle_type1(struct Myfile *myStruct) {
    // top: left to right
    if (calculate_insert_top(myStruct)) {
        return;
    }

    // right: top to bottom
    if (calculate_insert_right(myStruct)) {
        return;
    }

    // bottom: right to left
    if (calculate_insert_bottom(myStruct)) {
        return;
    }

    // left: bottom to top
    if (calculate_insert_left(myStruct)) {
        return;
    }

    // Insert at interior
    insert_interior(myStruct);
}

// Type H
/* handle_save_file
 * ---------------------
 * Open a file to save the playing game and close it
 * ---------------------
 * Parameters:
 * myStruct: get the needed information from myStruct
 * input: 's' + saveName
 * ---------------------
 * Return: Nothing
*/
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


/* handle_type_H
 * ---------------------
 * Get user input to throw a stone at the given position
 * repeat request if input is invalid or after saving game to a file
 * stop playing when Ctrl+D is detected or throw an error when a file name
 * of saving game is invalid
 * ---------------------
 * Parameters:
 * myStruct: get the needed information from myStruct
 * ---------------------
 * Return: true if input (R C) from player is valid and false otherwise
*/
bool handle_type_H(struct Myfile *myStruct) {
    char *turn = &myStruct->turn;
    int r, c, bufPosition, returnValue; 
    char line[50];
	char input[30];
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

    returnValue = sscanf(line, "%d %d %n", &r, &c, &bufPosition);

    if (returnValue == 2 && line[bufPosition] == '\0' && r >= 0 && c >= 0) {
        bool valid = is_valid_insert(myStruct, r, c, true);

        if (valid) {
            insert_board(myStruct, r, c);
            print_board(myStruct, stdout);
        } else {
            isValidInput = false;
        }
    } else {
        isValidInput = false;
    }

    return isValidInput;
}

/* play_game
 * ---------------------
 * While the game is not end, detect the player type (0,1,H) and
 * repeatedly play
 * ---------------------
 * Parameters:
 * myStruct: get the needed information from myStruct
 * typeOfPlayerX: Whether 0,1,H (get from argv[2])
 * typeOfPlayerO: Whether 0,1,H (get from argv[1])
 * ---------------------
 * Return: Nothing
*/
void play_game(struct Myfile *myStruct, char typeOfPlayerX, 
        char typeOfPlayerO) {
    char* turn = &myStruct->turn;

    while (!is_end_game(myStruct)) {
        char currentType = (*turn == 'X') ? typeOfPlayerX : typeOfPlayerO;

        if (currentType == 'H') {
            bool isValidInput = handle_type_H(myStruct);
            if (myStruct->exitCode == 5) {
                break;
            }

            if (!isValidInput) {
                continue;
            }
        }

        if (currentType == '0') {
            handle_type0(myStruct);
        }

        if (currentType == '1') {
            handle_type1(myStruct);
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

    play_game(&myStruct, *playerX.type, *playerO.type);

    if (!myStruct.validBoard) {
        return myStruct.exitCode;
    }

    find_winner(&myStruct);

    fclose(file);

    free_data(myStruct.data, myStruct.rows);

    return 0;
}
