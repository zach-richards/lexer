// type_case.c

#include "type_case.h"

void comment_case(char ch, FILE *fp, int *isComment) {
	int next = fgetc(fp);
	if (next == '*') {

		// Read until "*/" is found
		int prev = 0;
		while ((ch = fgetc(fp)) != EOF) {

			if (prev == '*' && ch == '/') break;
			prev = ch;
		    }

		    *isComment = 1;
		    return;

		} else {

		    ungetc(next, fp);
			*isComment = 0;
			return;
}