#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

struct file_content {
	bool valid;
	int row;
	int col;
	int numOfCols;
	char turn;
	char** data;
};

void read_line(FILE* file, struct file_content* myFile) {
	// printf("Row: %d\n", myFile -> row);
	// printf("Col: %d\n", myFile -> col);
	// printf("Turn: %c\n", myFile -> turn);

	int row = myFile->row;
	int col = myFile->col;
	int next = 0;

	int numOfRows = row;
	int numOfCols = col*2 + 1;
	myFile->numOfCols = numOfCols;
	myFile->valid=true;

	//printf("%d %d \n", numOfRows, numOfCols);

	while(1){
		next = fgetc(file);
		if(next=='\n'){
			next = fgetc(file);
			break;
		}
	}

	// char** arr = malloc(sizeof(char*) * 100);
	// for(int i=0; i<100 ; i++){
	// 	arr[i] = malloc(sizeof(char) * 100);
	// }

	// for(int r=0; r<100; r++){
	// 	for(int c=0; c<100; c++){
	// 		if (next == EOF) break;
	// 		if(next == '\n'){
	// 			arr[r][c] = '\n';
	// 			//printf("hi %c %d %d \n", arr[r][c], r, c);
	// 			next = fgetc(file);
	// 			break;
	// 		}
	// 		else{
	// 			arr[r][c] = (char)next;
	// 			//printf("%c %d %d \n", arr[r][c], r, c);
	// 			next = fgetc(file);
	// 		}
	// 	}
	// }

	// myFile->data = arr;
	// return;

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
		myFile->valid = false;
		//printf("Invalid from line 91\n");
		free(data);
		return;
	}
	
	// printf("Value at rc cc: '%c' '%c'\n", data[rowCheck][colCheck], data[rowCheck][colCheck+1]);
	// printf("rowcheck, colcheck valid: %d %d %d\n", rowCheck, colCheck, myFile->valid);

	myFile->data = data;
}

void printBoard(struct file_content* myFile){
	char** data = myFile->data;
	int rows = myFile->row;
	int cols = myFile->col;
	int numOfCols = myFile->numOfCols;
	int turn = myFile->turn;

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

int main(int argc, char** argv){
	if(argc != 4){
		fprintf(stderr, "Usage: push2310 typeO typeX fname\n");
		return 1;	
	}
	
	// check player type 
	for(int i=1; i<3; i++){
		if(*argv[i] != '0' && *argv[i] != '1' &&  *argv[i] != 'H'){
			fprintf(stderr, "Invalid player Type\n");
			return 1;
		}
	}

	// Check give file name
	char* fileName = argv[3];
	FILE* file;
	file = fopen(fileName, "r");
	if(file == NULL){
		fprintf(stderr, "No file to load from\n");
			return 1;
	}

	struct file_content myFile;

	// check given row and col
	int temp;
	int count = 0;
	while(fscanf(file,"%d",&temp)==1){
    	count++;
		if(count == 1){
			myFile.row = temp;
		}
		else if(count == 2){
			myFile.col = temp;
		}
		//printf("Temp: %d\n", temp);
  	}
	// printf("count numbers of int: %d\n", count);
	// printf("Rows: %d\n", myFile.row);
	// printf("Cols: %d\n", myFile.col);

	if(count != 2 || myFile.row < 3 || myFile.col < 3){
		fprintf(stderr, "Invalid file contents\n");
		return 1;
	}

	//Read the second line && get turn X:O
	int next = fgetc(file);
	char turn = (char)next;
	if(turn != 'X' && turn != 'O'){
		fprintf(stderr, "Invalid file contents\n");
		return 1;
	}
	myFile.turn = turn;

	read_line(file, &myFile);

	if(!myFile.valid){
		fprintf(stderr, "Invalid file contents\n");
		return 1;
	}

	//printf("Turn: %c\n", myFile.turn);
	// if(!myFile.valid){
	// 	fprintf(stderr, "Invalid file contents\n");
	// 	return 1;
	// }

	printBoard(&myFile);

	fclose(file);

	return 0;
}
