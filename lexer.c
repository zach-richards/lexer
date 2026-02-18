// x01_file_reader.c

// Parses the input file into tokens split by whitespace.

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argf[]) {
	FILE *fp = fopen(argf[1], "r"); // read only
	int isString = 0;
	
	// test null file
	if (fp == NULL) {
		printf("Error: Could not open file\n");
		exit(-1);
	}

	// read each char
	int ch;
	char sArr[256];
	int ind = 0;
	while ((ch = fgetc(fp)) != EOF) {
		switch (ch) {
			case ' ':

			case '\n':

				if (isString == 1) {
					sArr[ind] = ch;
					ind++;
				}

				break;

			case '"': // boolean. First \" sets bool to true, last sets it to false
				  
				if (isString == 0) {
					isString = 1;
					sArr[0] = '"';
					
					ind = 1;
				} else {
					isString = 0;

					sArr[ind] = ch;
					sArr[ind + 1] = '\0';
					ind = 0;
					
					printf("String: ");
					for (int i = 0; sArr[i] != '\0'; i++) {
						 printf("%c", sArr[i]);
					}
					printf("\n");
				}

				break;

			default:

				if (isString == 1){
					sArr[ind] = ch;
					ind++;
				} else {
					printf("Token: ");
					while (ch != '\n' && !isspace(ch)) { // parse by whitespace and new line
						printf("%c", ch);
						ch = fgetc(fp);
					}
					printf("\n");
				}

				break;
		}
	}
	
	return 0;
}
