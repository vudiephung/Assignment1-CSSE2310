#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

struct file_content {
	bool valid;
	int row;
	int col;
	char* turn;
	char* result;
};

void read_line(FILE* file, struct file_content* myFile) {
	char* result = malloc(sizeof(char) * 80);
	int position = 0;
	int next = 0;

	//printf("Row: %d\n", myFile -> row);
	//printf("Col: %d\n", myFile -> col);

	while(1){
		next = fgetc(file);
		if(next == EOF){
			result[position] = '\0';
			myFile -> result = result;
			break;
		} 
		// if(next == '\n') {
		// 	next = fgetc(file);
		// 	// read 2nd line
		// 	char turn = (char)next;
		// 	printf("Turn: %c\n",turn);
		// }
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
		else if (count == 2){
			myFile.col = temp;
		}
		//printf("Temp: %d\n", temp);
  	}
	//printf("count numbers of int: %d\n", count);
	// printf("Rows: %d\n", myFile.row);
	// printf("Cols: %d\n", myFile.col);

	if(count != 2 || myFile.row < 3 || myFile.col < 3){
		fprintf(stderr, "Invalid file contents\n");
		return 1;
	}

	//Read the second line && check valid file

	read_line(file, &myFile);
	if(!myFile.valid){
		fprintf(stderr, "Invalid file contents\n");
		return 1;
	}

	fclose(file);

	return 0;
}
