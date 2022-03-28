#ifndef WW_H
#define WW_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#define DEBUG 1
#define BUFSIZE 10

int isDirectory(const char *path);
void wordWrapTextFile(char* argument2, int wrapWidth);

#endif
