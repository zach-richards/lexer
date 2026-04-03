// type_case.h

#ifndef TYPE_CASE_H
#define TYPE_CASE_H

#include <stdio.h>
#include <stdlib.h>

void comment_case(char ch, FILE *fp, int *isComment);

void string_case(char ch, FILE *fp, int isString);

void keyword_case(char ch, FILE *fp, int isKeyword);

void operator_case(char ch, FILE *fp, int isOperator);

#endif