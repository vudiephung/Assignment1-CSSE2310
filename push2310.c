#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

struct file_content {
	bool valid: true;
	int row;
	int col;
	char turn;
	char* result;
};

struct file_content *read_line_test(FILE* file) {
	struct file_content* myFile;
	//char* result = malloc(sizeof(char) * 80);
	int position = 0;
	int next = 0;
	int digitCount = 0;
	// Read first line to get row and col

	while(1){
		next = fgetc(file);
		if(next == EOF || next == '\n'){
			myFile -> valid = false;
			return myFile;
		}
		else {
			myFile -> row = (int)((char)next);
			printf("%d\n", myFile->row);
		}
		// if(next == EOF || next=='\n'){
		// 	printf("%c\n", (char)next);
		// 	myFile -> valid = false;
		// 	return myFile;		
		// } 
		// else{
		// 	if(next == ' '){
		// 		printf("%c\n", (char)next);
		// 	}

		// 	return myFile;
		// }
	}

}

char* read_line(FILE* file) {
	char* result = malloc(sizeof(char) * 80);
	int position = 0;
	int next = 0;

	while(1){
		next = fgetc(file);
		if(next == EOF || next=='\n'){
			result[position] = '\0';
			return result;
		} else if((char)next != ' ') result[position++] = (char)next;
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
	FILE* file = fopen(fileName, "r");
	if(file == NULL){
		fprintf(stderr, "No file to load from\n");
			return 1;
	}

	// read file && check valid file
	FILE* outputFile;
	//char* input;

	outputFile = fopen(fileName, "r");

	struct file_content* input;
	input = read_line_test(outputFile);

	//fprintf(stdout, "Read %s\n", input);

	// int i=0;
	// while(input[i] != '\0'){
	// 	printf("%c\n", input[i]);
	// 	i++;
	// }

	//fflush(stdout);

	//free(input);
	//fflush(outputFile);
	fclose(outputFile);

	return 0;
}
