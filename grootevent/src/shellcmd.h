#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BACKQUOTE 96

#define SUBSTITUTE_OK 0
#define SUBSTITUTE_ERROR_NOT_FOUND 1
#define SUBSTITUTE_ERROR_UNCLOSED_BACKQUOTE 2

void strnadd (char **str, char *append, unsigned int count);
void replace (char **str, char *needle, char *replace);
int substitute (char **exec_string);
