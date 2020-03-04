#include<stdio.h>

int main(int argc, char** argv){
	if(argc != 4){
		fprintf(stderr, "Usage: push2310 typeO typeX fname\n");
		return 1;	
	}
	//enum validType = {0,1,H};
	
	// check player type 
	for(int i=1; i<3; i++){
		printf("%c\n", *argv[i]);
		if(*argv[i] != '0' && *argv[i] != '1' &&  *argv[i] != 'H'){
			printf("Invalid player Type\n");
			return 1;
		}
	}

	// read file 
	

	return 0;
}
