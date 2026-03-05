// lexer.c

// Parses the input file into tokens split by whitespace. Can also classify and parse strings, chars and comments,
// throwing out comments.

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char* argv[]) {
    	if (argc < 2) {
        	printf("Usage: %s <filename>\n", argv[0]);
        	return -1;
    	}

    	FILE *fp = fopen(argv[1], "r");
    
	if (fp == NULL) {
        	printf("Error: Could not open file\n");
        	return -1;
    	}

    	int ch;

    	int isString = 0;
    	char sArr[256];
    	int sInd = 0;

    	int isChar = 0;
    	char cArr[4];
    	int cInd = 0;

    	int isComment = 0;
    	char comArr[256];
    	int comInd = 0;

    	while ((ch = fgetc(fp)) != EOF) {

        	if (ch == '"') {
            		if (!isString) {
                		isString = 1;
                		sInd = 0;
                		sArr[sInd] = ch;
				sInd++;
            		} else {
                		sArr[sInd] = ch;
				sInd++;
                		sArr[sInd] = '\0';
                		printf("String: %s\n", sArr);
                		isString = 0;
                		sInd = 0;
            		}
            		continue;
        	}

        	if (ch == '\'') {
            		if (!isChar && !isString) {
                		isChar = 1;
                		cInd = 0;
                		cArr[cInd] = ch;
				cInd++;
           	 	} else if (isChar) {
                		cArr[cInd] = ch;
				cInd++;
                		cArr[cInd] = '\0';
               	 		printf("Char: %s\n", cArr);
                		isChar = 0;
            		} else if (isString) {
                		sArr[sInd] = ch;
				sInd++;
            		}
            		continue;
        	}

		if (ch == '/' && !isChar) {
		    	int next = fgetc(fp);

			if (next == '*') {
		    		isComment = 1;
		    		comInd = 0;
		    		comArr[comInd] = '/';
				comInd++;
		    		comArr[comInd] = '*';
				comInd++;

		    		int prev = 0;

		    		while ((ch = fgetc(fp)) != EOF) {
					comArr[comInd] = ch;
					comInd++;

					if (prev == '*' && ch == '/') {
			    			break;
					}
					prev = ch;
		    		}

		    		comArr[comInd] = '\0';
		    		isComment = 0;
		    		continue;
		    	} else {
				ungetc(next, fp);
			}
		}

		if (isComment) {
			continue;
		}

        	if (isString) {
            		sArr[sInd++] = ch;
            		continue;
        	}

        	if (isChar) {
            		cArr[cInd++] = ch;
            		continue;
        	}

        	if (!isspace(ch)) {
            		printf("Token: ");
            	do {
                	printf("%c", ch);
                	ch = fgetc(fp);
            	} while (ch != EOF && !isspace(ch) && ch != '"' && ch != '\'');
            	printf("\n");
            	if (ch == EOF) break;
            		ungetc(ch, fp);
        	}
    	}

    	fclose(fp);
    	return 0;
}
