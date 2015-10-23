#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	// If argument list isn't complete print info:
	if(argc != 4) {
		printf("Usage: %s filename 0/1(coding type) 0/1(grayscale)\n", argv[0]);
	}
	else {
		// Try to open a file:
		FILE* file = fopen(argv[1], "r");

		if(!file) printf("Could not open file \"%s\".\n", argv[1]);
		else {
			printf("\"%s\" opened successfully.\n", argv[1]);
			fclose(file);
		}
	}
	return 0;
}
