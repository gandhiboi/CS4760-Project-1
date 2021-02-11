#ifndef PERMISSIONS_H
#define PERMISSIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <getopt.h>
#include <time.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>

#include "queue.h"

void concat(const char *);

char *sizeFile(const int);

void filePermissions(char *, bool, char *);

void initDirectory();

void display();

#endif
