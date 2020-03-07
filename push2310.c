#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

struct file_content {
	bool valid;
	int row;
	int col;
	int numOfCols;
	int scoreX;
	int scoreY;
	char turn;
	char** data;
};

bool is_valid_insert(char** data, struct file_content* myStruct, int R, int C){
	int rows = myStruct->row; // row of board
	int cols = myStruct->col; // col of board 
	int row_to_insert = R;  // position in data
	int col_to_insert = 2*C+1; // position in data

	if(R>=rows || C>=cols) return false;
	if(data[row_to_insert][row_to_insert] != '.') return false;

	if(R == 0){
		bool invalids = C==0 || 
						C==cols-1 || 
						data[1][col_to_insert] == '.' || 
						data[rows-1][col_to_insert] != '.';
		if(invalids) return false;
	} 
	else if(R==rows-1){
		bool invalids = C==0 || 
						C==cols-1 ||
						data[rows-2][col_to_insert] == '.' || 
						data[0][col_to_insert] != '.';
		if(invalids) return false;
	}
	else if(C==0){
		bool invalids = data[row_to_insert][col_to_insert+2]=='.' ||
						data[row_to_insert][2*cols-1]!='.';
		if(invalids) return false;
	}
	else if(C==cols-1){
		bool invalids = data[row_to_insert][col_to_insert-2] == '.'||
						data[row_to_insert][1] != '.';
		if(invalids) return 0;
	}

	return true;
}

int scoring(char** data, struct file_content* myStruct, char turn){

}

void insert_board(FILE* file, struct file_content* myStruct, int R, int C){
	char** data = myStruct->data;
	int row_to_insert = R;
	int col_to_insert = 2*C+1;
	char* turn = &myStruct->turn;
	
	//Insert into position
	data[row_to_insert][col_to_insert] = *turn;
	*turn = 'X' ? 'Y' : 'X'; 
	//myStruct->turn = turn;
	//Modify data
	// printf("From line 66: %c\n", myStruct->data[row_to_insert][col_to_insert]);
	printf("From line 70: %c\n", *turn);
	//scoreX, Y
}

void read_board(FILE* file, struct file_content* myStruct) {
	// printf("Row: %d\n", myStruct -> row);
	// printf("Col: %d\n", myStruct -> col);
	// printf("Turn: %c\n", myStruct -> turn);

	int row = myStruct->row;
	int col = myStruct->col;
	int next = 0;

	int numOfRows = row;
	int numOfCols = col*2 + 1;
	myStruct->numOfCols = numOfCols;
	myStruct->valid=true;

	//printf("%d %d \n", numOfRows, numOfCols);

	while(1){
		next = fgetc(file);
		if(next=='\n'){
			next = fgetc(file);
			break;
		}
	}

	// Define 2D array

	char** data = malloc(sizeof(char*) * numOfRows);
	for(int i=0; i<numOfRows ; i++){
		data[i] = malloc(sizeof(char) * numOfCols);
	}

	for(int r=0; r<numOfRows; r++){
		
		for(int c=0; c<numOfCols; c++){
			if (next == EOF) break;
			else if(next == '\n'){
				data[r][c] = '\n';
				//printf("hi %c %d %d \n", data[r][c], r, c);
				next = fgetc(file);
				break;
			}
			else{
				data[r][c] = (char)next;
				//printf("%c %d %d \n", data[r][c], r, c);
				next = fgetc(file);
			}
		}
	}

	int rowCheck = numOfRows - 1;
	int colCheck = numOfCols - 3;

	if(data[rowCheck][colCheck] != ' ' || data[rowCheck][colCheck+1] != ' '){
		myStruct->valid = false;
		//printf("Invalid from line 91\n");
		free(data);
		return;
	}
	
	// printf("Value at rc cc: '%c' '%c'\n", data[rowCheck][colCheck], data[rowCheck][colCheck+1]);
	// printf("rowcheck, colcheck valid: %d %d %d\n", rowCheck, colCheck, myStruct->valid);

	myStruct->data = data;
}

void print_board(struct file_content* myStruct){
	char** data = myStruct->data;
	int rows = myStruct->row;
	int cols = myStruct->col;
	int numOfCols = myStruct->numOfCols;
	int turn = myStruct->turn;

	printf("%d %d\n", rows, cols);
	printf("%c\n", turn);
	for(int r=0; r<rows; r++){
		for(int c=0; c<numOfCols; c++){
			if(data[r][c] == '\n'){
				printf("\n");
			}
			else printf("%c", data[r][c]);
		}
	}
}

void write_board_to_file(FILE* writeFile, struct file_content* myStruct){
	//fprintf(writeFile, print_board(myStruct));
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

	char* firstType = argv[1];
	char* secondType = argv[2];

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

	read_board(file, &myStruct);

	if(!myStruct.valid){
		fprintf(stderr, "Invalid file contents\n");
		fflush(stderr);
		return 4;
	}

	//printf("Turn: %c\n", myStruct.turn);
	// if(!myStruct.valid){
	// 	fprintf(stderr, "Invalid file contents\n");
	// 	return 1;
	// }

	print_board(&myStruct);

	// Base on player type 
	// H H ???

	if(*firstType == 'H' && *secondType == 'H'){
		int R, C;
		printf("%c:(R C)> ", turn);
		scanf("%d %d", &R, &C);
		//printf("R: %d C: %d\n", R, C);

		//Insert into board 
		insert_board(file, &myStruct, R, C);
		// printf("From line 250: %c\n", myStruct.data[R][2*C+1]);
		// printf("From line 251: %c\n", myStruct.turn);

		// Write to File 
		FILE* file_write;
		file_write = fopen("sSavedFile", "w");
		write_board_to_file(file_write, &myStruct);

		// Show board to the screen 


		// Trigger new turn
	}

	fclose(file);

	return 0;
}
