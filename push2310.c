#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

struct file_content {
	bool valid;
	bool is_end_game;
	char Winners;
	int row;
	int col;
	int numOfCols;
	char turn;
	char** data;
};

struct Player{
	char* type; // 0 1 H
};

void print_board(struct file_content* myStruct, FILE* file){
	char** data = myStruct->data;
	int rows = myStruct->row;
	int cols = myStruct->col;
	int numOfCols = myStruct->numOfCols;
	int turn = myStruct->turn;

	// fprintf(file, "%d %d\n", rows, cols);
	// fflush(file);
	// fprintf(file, "%c\n", turn);
	
	for(int r=0; r<rows; r++){
		for(int c=0; c<numOfCols; c++){
			if(data[r][c] == '\n'){
				fprintf(file, "\n");
			}
			else fprintf(file, "%c", data[r][c]); 
		}
	}
}

bool is_valid_insert(struct file_content* myStruct, int R, int C){
	int rows = myStruct->row; // row of board
	int cols = myStruct->col; // col of board 
	int row_to_insert = R;  // position in data
	int col_to_insert = 2*C+1; // position in data
	char** data = myStruct->data;

	//printf("Line 45: %d %d %c\n",row_to_insert,col_to_insert, data[row_to_insert][col_to_insert]);

	if(R>=rows || C>=cols) {
		//printf("first.\n");
		return false;
	}

	if(data[row_to_insert][col_to_insert] != '.'){
		//printf("Second.\n");
		return false;
	}

	if(R == 0){
		bool invalids = C==0 || 
						C==cols-1 || 
						data[1][col_to_insert] == '.';
		bool isEmptyShell = false;
		for(int i=1; i<rows; i++){
			if(data[i][col_to_insert] == '.'){
				for(int shiftIndex=i; shiftIndex>0; shiftIndex--){
					data[shiftIndex][col_to_insert] = data[shiftIndex-1][col_to_insert];
				}
				//printf("Dot at %d %d (before shift)\n", i, col_to_insert);
				isEmptyShell = true;
				break;
			}
		}
		if(invalids || !isEmptyShell) {
			//printf("Third.\n");
			return false;
		}
	} 

	if(R==rows-1){
		bool invalids = C==0 || 
						C==cols-1 ||
						data[rows-2][col_to_insert] == '.';
						//data[0][col_to_insert] != '.';
		bool isEmptyShell = false;
		for(int i=rows-2; i>=0; i--){
				if(data[i][col_to_insert] == '.'){
					for(int shiftIndex=i; shiftIndex<rows-1; shiftIndex++){
						data[shiftIndex][col_to_insert] = data[shiftIndex+1][col_to_insert];
					}
					//printf("Dot at %d %d (before shift)\n", i, col_to_insert);
					isEmptyShell = true;
					break;
			}
		}
		if(invalids || !isEmptyShell) {
			//printf("Fourth.\n");
			return false;
		}
	}

	if(C==0){
		bool invalids = data[row_to_insert][col_to_insert+2]=='.' ||
						data[row_to_insert][2*cols-1]!='.';
		bool isEmptyShell = false;
		for(int i=3; i<2*cols; i+=2){
				if(data[row_to_insert][i] == '.'){
					for(int shiftIndex=i; shiftIndex>=3; shiftIndex-=2){
						data[row_to_insert][shiftIndex] = data[row_to_insert][shiftIndex-2];
					}
					isEmptyShell = true;
					break;
			}
		}
		if(invalids || !isEmptyShell) {
			//printf("Fifth.\n");
			return false;
		}
	}

	if(C==cols-1){
		//printf("This one\n");
		bool invalids = data[row_to_insert][col_to_insert-2] == '.'||
						data[row_to_insert][1] != '.';
		bool isEmptyShell = false;
		for(int i=2*cols-3; i>=1; i-=2){
				if(data[row_to_insert][i] == '.'){
					for(int shiftIndex=i; shiftIndex<2*cols-2; shiftIndex+=2){
						data[row_to_insert][shiftIndex] = data[row_to_insert][shiftIndex+2];
					}
					isEmptyShell = true;
					break;
			}
		}
		if(invalids || !isEmptyShell) {
			//printf("Sixth.\n");
			return false;
		}
	}

	return true;
}

bool is_end_game(struct file_content* myStruct){
	char** data = myStruct->data;
	int rows = myStruct->row;
	int cols = myStruct->col;
	bool is_end = true;
	for(int i=1; i<rows-1; i++){
		for(int j=3; j<2*cols-2; j+=2){
			if(data[i][j] == '.'){
				is_end = false;
				break;
			}
		}
		if(is_end == false) break;
	}
	return is_end;
}

void findWinner(struct file_content* myStruct){
	char** data = myStruct->data;
	int rows = myStruct->row;
	int numOfCols = myStruct->numOfCols;
	int scoreX = 0;
	int scoreO = 0;
	for(int r=0; r<rows; r++){
		for(int c=0; c<numOfCols; c++){
			if(data[r][c] == 'X'){
				scoreX+= (data[r][c-1]-'0');
			} else if(data[r][c] == 'O'){
				scoreO+= (data[r][c-1]-'0');
			}
		}
	}
	//printf("Score: %d %d\n", scoreX, scoreO);
	if(scoreX > scoreO) {
		myStruct->Winners = 'X';
	} else if(scoreO> scoreX){
		myStruct->Winners = 'O';
	} else myStruct->Winners = '?';

	if(myStruct->Winners == '?'){
		printf("Winners: %c %c\n", 'O', 'X');
	} else {
		printf("Winners: %c\n", myStruct->Winners);
	}
	
}

void insert_board(FILE* file, struct file_content* myStruct, int R, int C){
	char** data = myStruct->data;
	int row_to_insert = R;
	int col_to_insert = 2*C+1;
	int rows = myStruct->row;
	int cols = myStruct->col;

	char* turn = &myStruct->turn;

	//Insert into position

	if(R==0){
		data[1][col_to_insert] = *turn;
	} else if(R==rows-1){
		data[rows-2][col_to_insert] = *turn;
	} else if(C==0){
		data[row_to_insert][col_to_insert+2] = *turn;
	} else if(C==cols-1){
		data[row_to_insert][col_to_insert-2] = *turn;
	} 
	else data[row_to_insert][col_to_insert] = *turn;
	
	*turn = *turn=='X' ? 'O' : 'X'; 
	
	//Modify data
	myStruct->turn = *turn;
}

void read_board(FILE* file, struct file_content* myStruct) {
	char** data = myStruct->data;
	int numOfRows = myStruct->row;
	int numOfCols = myStruct->col * 2 + 1;
	int next = 0;
	myStruct->valid=true;

	//printf("%d %d \n", numOfRows, numOfCols);

	while(1){
		next = fgetc(file);
		if(next=='\n'){
			next = fgetc(file);
			break;
		}
	}

	for(int r=0; r<numOfRows; r++){
		if(!myStruct->valid) break;
		for(int c=0; c<numOfCols; c++){
			//if (next == EOF) break;
			// check numbers of 4 edges must be 0
			if(r==0 || r==numOfRows-1){
				if(c > 1 && c < numOfCols-3 && c%2==0){
					//printf("This line '%d' '%d' '%c'\n", r,c, (char)next);
					if((char)next-'0' != 0){
						myStruct->valid = false;
						printf("Line 257\n");
						break;
					}
				}
			}
			else{
				if(c == 0 || c==numOfCols-3){
					if((char)next-'0' != 0){
						myStruct->valid = false;
						printf("Line 269\n");
						break;
					}
				}
			}

			//
			if(next == '\n'){
				data[r][c] = '\n';
				//printf("hi %c %d %d \n", data[r][c], r, c);
				next = fgetc(file);
				//break;
			}
			else{
				if( (char)next-'0' == 0 ||
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
				){
					data[r][c] = (char)next;
					//printf("%c %d %d \n", data[r][c], r, c);
					next = fgetc(file);
				}
				else{
					myStruct->valid = false;
					break;
				}
			}
		}
	}

	if(!myStruct->valid) return;

	int rowCheck = numOfRows - 1;
	int colCheck = numOfCols - 3;

	if(data[rowCheck][colCheck] != ' ' || data[rowCheck][colCheck+1] != ' '){
		myStruct->valid = false;
		//printf("Invalid from line 91\n");
		return;
	}
	
	// printf("Value at rc cc: '%c' '%c'\n", data[rowCheck][colCheck], data[rowCheck][colCheck+1]);
	// printf("rowcheck, colcheck valid: %d %d %d\n", rowCheck, colCheck, myStruct->valid);

	myStruct->data = data;
}

void free_data(struct file_content* myStruct, char** data){
	int numOfRows = myStruct->row;
	for(int i=0; i<numOfRows ; i++){
		free(data[i]);
	}
	free(data);
}

int main(int argc, char** argv){
	if(argc != 4){
		fprintf(stderr, "Usage: push2310 typeO typeX fname\n");
		fflush(stderr);
		return 1;	
	}
	
	// check player type 
	for(int i=1; i<3; i++){
		if(strcmp(argv[i],"0") && strcmp(argv[i],"1") &&  strcmp(argv[i],"H")){
			fprintf(stderr, "Invalid player Type\n");
			fflush(stderr);
			return 2;
		}
	}

	struct Player playerO, playerX;
	playerO.type = argv[1];
	playerX.type = argv[2];

	// Check give file name
	char* fileName = argv[3];
	FILE* file;
	file = fopen(fileName, "r");
	if(file == NULL){
		fprintf(stderr, "No file to load from\n");
		fflush(stderr);
		return 3;
	}

	struct file_content myStruct;

	// check given row and col
	int temp;
	int count = 0;
	while(fscanf(file,"%d",&temp)==1){
    	count++;
		if(count == 1){
			myStruct.row = temp;
		}
		else if(count == 2){
			myStruct.col = temp;
		}
		//printf("Temp: %d\n", temp);
  	}
	// printf("count numbers of int: %d\n", count);
	// printf("Rows: %d\n", myStruct.row);
	// printf("Cols: %d\n", myStruct.col);

	if(count != 2 || myStruct.row < 3 || myStruct.col < 3){
		fprintf(stderr, "Invalid file contents\n");
		fflush(stderr);
		return 4;
	}

	//Read the second line && get turn X:O
	int next = fgetc(file);
	char turn = (char)next;
	if(turn != 'X' && turn != 'O'){
		fprintf(stderr, "Invalid file contents\n");
		fflush(stderr);
		return 4;
	}
	myStruct.turn = turn;

	int numOfRows = myStruct.row;
	int numOfCols = myStruct.col*2 + 1;
	char** data = malloc(sizeof(char*) * numOfRows);
	for(int i=0; i<numOfRows ; i++){
		data[i] = malloc(sizeof(char) * numOfCols);
	}
	myStruct.data = data;
	myStruct.numOfCols = numOfCols;

	read_board(file, &myStruct);

	if(!myStruct.valid){
		fprintf(stderr, "Invalid file contents\n");
		fflush(stderr);
		free_data(&myStruct,data);
		return 4;
	}

	print_board(&myStruct, stdout);

	// Base on player type 
	int R,C, bufPos, returnValue;
    char line[20];

	while(!is_end_game(&myStruct)){

		//print_board(&myStruct, stdout);

		char* saveName;
		// What player turn?
		turn = myStruct.turn; // X or O // char

		char currentType; // 0 1 H ?
		if(turn == 'X'){
			currentType = *playerX.type;
		} else {
			currentType = *playerO.type;
		}

		if(currentType == 'H'){
			printf("%c:(R C)> ", turn);

				// if(scanf("%c%s", "s", saveName) == 2){
				// 	printf("388: %s\n", saveName);
				// 	// save File 
				// 	FILE* file_write;
				// 	file_write = fopen(saveName, "w");
				// 	print_board(&myStruct, file_write);
				// 	fclose(file_write);
				// }

			if(fgets(line, sizeof(line), stdin) == 0){
				fprintf(stderr, "End of file\n");
				return 5;
			}

				// if (fgets(buffer, 80, fin)==0) {
				// return 1;
				// }

			returnValue = sscanf(line, "%d %d %n", &R, &C, &bufPos);
				// printf("Return value: '%d'\n", bufPos);
				// printf("line[bufPos]: '%c'\n", line[bufPos]);

			if (returnValue==2 && line[bufPos] == '\0'){
				bool valid = is_valid_insert(&myStruct, R,C);
				if(valid){
					insert_board(file, &myStruct, R, C);
					print_board(&myStruct, stdout);
				} else continue;
			}
			else continue;
		}

		if(currentType == '0'){
			bool finish = false;
			//print_board(&myStruct, stdout);
			if(turn == 'O'){ //left to right 
				//printf("Line 463\n");
				for(int r=1; r<numOfRows-1; r++){
					if(finish) break;
					for(int c=3; c<numOfCols-3; c+=2){
						if(myStruct.data[r][c] == '.'){
							myStruct.data[r][c] = turn;
							printf("Player %c placed at %d %d\n", turn, r, (c-1)/2);
							print_board(&myStruct, stdout);
							myStruct.turn = 'X';
							finish = true;
							break;
						}
					}
				}				
			}
			else if(turn == 'X'){
				for(int r=numOfRows-2; r>=1; r--){
					if(finish) break;
					for(int c=numOfCols-4; c>1; c-=2){
						if(myStruct.data[r][c] == '.'){
							myStruct.data[r][c] = turn;
							printf("Player %c placed at %d %d\n", turn, r, (c-1)/2);
							print_board(&myStruct, stdout);
							myStruct.turn = 'O';
							finish = true;
							break;
						}
					}
				}
			}
		}


	}

	findWinner(&myStruct);

	fclose(file);

	// Free data
	free_data(&myStruct,data);

	return 0;
}
