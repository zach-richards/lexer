// lexer.c

// Parses the input file into tokens split by whitespace. Can also classify and parse strings, chars and comments,
// throwing out comments.

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// List of operators
const char *operators[] = {
	"{:", "}:", ":=", "..", "<<", ">>", "<>", "<=", ">=", "**", "==", "!=", "=>", "<", ">", "(", ")", "+", 
	"-", "*", "/", "|", "&", ";", ",", ":", "=", "$", "@", "[", "]", "{", "}", "."
};

const int NUM_OF_OPS = 34;

// List of keywords
const char *keywords[] = { 
	"accessor", "character", "constant", "mutator", "natural", "positive", "subtype", "integer", 
	"format", "elsif", "others", "while", "exit", "float", "array", "accessor", "proc", "then", 
	"type", "func", "ptr", "sort", "plus", "case", "else", "ifc", "null", "out", "pkg", "or", 
	"in", "is", "if", "of", "when" 
}; 

const int NUM_OF_KEYWORDS = 35;

int indentation = 0;

// indent for functions and loops
void indent(int indentation) {
        for (int i = 0; i < indentation; i++) {
                printf("  ");
        }
}

// check for operators and if needed, print operator
int checkForOps(FILE *fp, int ch) {

    if (ch == '}') { indentation--; }

    for (int i = 0; i < NUM_OF_OPS; i++) {

        int opLen = 0;
        while (operators[i][opLen] != '\0') opLen++;

        if (opLen == 2) {

            int next = fgetc(fp);

            if (ch == operators[i][0] && next == operators[i][1]) {
                indent(indentation);
                printf("Operator: %s\n", operators[i]);
		if (ch == '{') { indentation++; }
                return 1;
            }

            ungetc(next, fp);
        }

        else if (opLen == 1 && ch == operators[i][0]) {
            indent(indentation);
            printf("Operator: %s\n", operators[i]);
	    if (ch == '{') { indentation++; }
            return 1;
        }
    }

    return 0;
}

int strcmp(const char *s1, const char *s2) {
    	int i = 0;

    	// Keep looping as long as characters are the same
    	while (s1[i] == s2[i]) {
        	if (s1[i] == '\0') {
            		return 0; // Both strings ended at the same time; they are equal
        	}
        	i++;
    	}

    	// Return the difference between the two differing characters
    	return (unsigned char)s1[i] - (unsigned char)s2[i];
}

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
	
	int isKeyword = 0; 
	int isOperator = 0;

	while ((ch = fgetc(fp)) != EOF) {
	    	if (isspace(ch)) continue;

	    	// handle Comments
	    	if (ch == '/') {
			int next = fgetc(fp);
			if (next == '*') {
		    		// Read until "*/" is found
		    		int prev = 0;
		    		while ((ch = fgetc(fp)) != EOF) {
					if (prev == '*' && ch == '/') break;
					prev = ch;
		    		}
		    		continue; 
			} else {
		    		ungetc(next, fp);
			}
	    	}

	    	// Handle Strings
	    	if (ch == '"') {
			indent(indentation);
			printf("String: ");
			while ((ch = fgetc(fp)) != EOF && ch != '"') {
		    		putchar(ch);
			}
			printf("\n");
			continue;
	    	}

	    	// Handle Keywords
	    	if (isalpha(ch)) {
			char temp[64];
			int i = 0;
			temp[i++] = (char)ch;
		
			while (i < 63) {
		    		int next = fgetc(fp);
		    		if (next == EOF || (!isalnum(next) && next != '_')) {
					ungetc(next, fp);
					break;
		    		}
		    		temp[i++] = (char)next;
			}
			temp[i] = '\0';

			int foundKeyword = 0;
			for (int j = 0; j < NUM_OF_KEYWORDS; j++) {
		    		if (strcmp(temp, keywords[j]) == 0) {
					indent(indentation);
					printf("Keyword: %s\n", temp);
					foundKeyword = 1;
					break;
		    		}
			}
			if (!foundKeyword) {
				indent(indentation);
				printf("Token: %s\n", temp);
			}

			continue; 
	    	}

		// Default to token
		int matched = 0;

		if (!isspace(ch)) {

			if (checkForOps(fp, ch)) {
				matched = 1;
			}

			char temp[256];
			int i = 0;

			while (ch != EOF && !isspace(ch) && matched == 0) {

				temp[i++] = (char)ch;

				int next = fgetc(fp);

				int isOpStart = 0;
				for (int j = 0; j < NUM_OF_OPS; j++) {
					if (operators[j][0] == next) {
						isOpStart = 1;
						break;
					}	
				}

				if (isOpStart) {
					ungetc(next, fp);
					break;
				}

				ch = next;
			}

			temp[i] = '\0';

			if (i > 0) {
				indent(indentation);
				printf("Token: %s\n", temp);
			}	
		}
	}	
	
	fclose(fp); 
	return 0; 
}
