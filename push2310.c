#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

struct file_content {
	bool valid;
	int row;
	int col;
	int numOfCols;
	// int scoreX;
	// int scoreY;
	char turn;
	char** data;
};

void print_board(struct file_content* myStruct, FILE* file){
	char** data = myStruct->data;
	int rows = myStruct->row;
	int cols = myStruct->col;
	int numOfCols = myStruct->numOfCols;
	int turn = myStruct->turn;

	fprintf(file, "%d %d\n", rows, cols);
	fflush(file);
	fprintf(file, "%c\n", turn);
	
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
		printf("first.\n");
		return false;
	} 

	if(R == 0){
		bool invalids = C==0 || 
						C==cols-1 || 
						data[1][col_to_insert] == '.' || 
						data[rows-1][col_to_insert] != '.';
		if(invalids) {
			printf("Third.\n");
			return false;
		}
	} 
	else if(R==rows-1){
		bool invalids = C==0 || 
						C==cols-1 ||
						data[rows-2][col_to_insert] == '.' || 
						data[0][col_to_insert] != '.';
		if(invalids) {
			printf("Fourth.\n");
			return false;
		}
	}
	else if(C==0){
		bool invalids = data[row_to_insert][col_to_insert+2]=='.' ||
						data[row_to_insert][2*cols-1]!='.';
		if(invalids) {
			printf("Fith.\n");
			return false;
		}
	}
	else if(C==cols-1){
		bool invalids = data[row_to_insert][col_to_insert-2] == '.'||
						data[row_to_insert][1] != '.';
		if(invalids) {
			printf("Sixth.\n");
			return 0;
		}
	}

	if(data[row_to_insert][col_to_insert] != '.'){
		printf("Second.\n");
		return false;
	}

	return true;
}

int scoring(char** data, struct file_content* myStruct, char turn){

}

void insert_board(FILE* file, struct file_content* myStruct, int R, int C){
	char** data = myStruct->data;
	int row_to_insert = R;
	int col_to_insert = 2*C+1;
	int rows = myStruct->row;
	int cols = myStruct->col;

	char* turn = &myStruct->turn;
	
	//Insert into position
	data[row_to_insert][col_to_insert] = *turn;

	if(R==0){
		for(int i=rows-1; i>0; i--){
			data[i][col_to_insert] = data[i-1][col_to_insert];
		}
	} else if(R==rows-1){
		for(int i=0; i<rows; i++){
			data[i][col_to_insert] = data[i+1][col_to_insert];
		}
	} else if(C==0){
		for(int i=2*cols-1; i>0; i-=2){
			data[row_to_insert][i] = data[row_to_insert][i-2];
		}
	} else if(C==cols-1){
		for(int i=0; i<2*cols; i+=2){
			data[row_to_insert][i] = data[row_to_insert][i+2];
		}
	} 

	data[row_to_insert][col_to_insert] = '.';
	// else {
	// 	data[row_to_insert][col_to_insert] = *turn;
	// }
	
	*turn = *turn=='X' ? 'O' : 'X'; 
	
	//Modify data
	myStruct->turn = *turn;

	//printf("From line 76: %c\n", myStruct->data[row_to_insert][col_to_insert]);

	//printf("From line 116: %c\n", myStruct->turn);

	//scoreX, Y
}

void read_board(FILE* file, struct file_content* myStruct) {
	// printf("Row: %d\n", myStruct -> row);
	// printf("Col: %d\n", myStruct -> col);
	// printf("Turn: %c\n", myStruct -> turn);
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

	//printf("Turn: %c\n", myStruct.turn);
	// if(!myStruct.valid){
	// 	fprintf(stderr, "Invalid file contents\n");
	// 	return 1;
	// }

	print_board(&myStruct, stdout);

	// Base on player type 
	// H H ???

	if(*firstType == 'H' && *secondType == 'H'){
		int R, C;
		printf("%c:(R C)> ", turn);
		scanf("%d %d", &R, &C);
		//printf("R: %d C: %d\n", R, C);

		//Insert into board 
		if(is_valid_insert(&myStruct, R,C)){
			insert_board(file, &myStruct, R, C);
		} else {
			//printf("Invalid input\n");
		}
		
		// printf("From line 250: %c\n", myStruct.data[R][2*C+1]);
		// printf("From line 251: %c\n", myStruct.turn);

		// Write to File 
		FILE* file_write;
		file_write = fopen(fileName, "w");

		print_board(&myStruct, file_write);

		fclose(file_write);
		//write_board_to_file(file_write, &myStruct);

		// Show board to the screen 
		//print_board(&myStruct);

		// Trigger new turn
	}

	fclose(file);

	// Free data
	free_data(&myStruct,data);

	return 0;
}
