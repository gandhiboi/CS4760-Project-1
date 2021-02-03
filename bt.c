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
#include "permissions.h"

void traversal(char *dir ,bool symlink, char *options);

int main(int argc, char* argv[]) {

        int indentSpace = 4;
        int opt;
        bool symbolicLink = false;
        char option[10] = "";
        char* topdir, * targetdir, current[2] = ".";

	setDir(argc, argv);

        while ((opt = getopt(argc, argv, "hLdgipstul")) != -1) {
                switch (opt) {

                case 'h':
                        printf("\nUSAGE:\n");
                        printf("        %s dt [-h] [-I n] [-L -d -g -i -p -s -t -u | -l] [dirName].\n\n", argv[0]);
                        printf("        -h      :       Print a help message and exit.\n");
                        printf("        -L      :       Follow symbolic links, if any. Default will be to not follow symbolic links.\n");
                        printf("        -t      :       Print information on file type.\n");
                        printf("        -p      :       Print permission bits as rwxrwxrwx.\n");
                        printf("        -i      :       Print the number of links to file in inode table.\n");
                        printf("        -u      :       Print the UID associated with the file.\n");
                        printf("        -g      :       Print the GID associated with the file.\n");
                        printf("        -s      :       Print the size of file in bytes. Appropriate suffixes have been added.\n");
                        printf("        -d      :       Show the time of last modification.\n");
                        printf("        -l      :       Used to print information on the file as if the options tpiugs are all specified.\n");

                        return EXIT_SUCCESS;

                case 'L':
                        symbolicLink = true;
                        break;

                case 't':
                        strcat(option, "t");
                        break;

                case 'p':
                        strcat(option, "p");
                        break;

                case 'i':
                        strcat(option, "i");
                        break;

                case 'u':
                        strcat(option, "u");
                        break;

                case 'g':
                        strcat(option, "g");
                        break;

                case 's':
                        strcat(option, "s");
                        break;

                case 'd':
                        strcat(option, "d");
                        break;

                case 'l':
                        strcat(option, "tpiugs");
                        break;

                default:
                        fprintf(stderr, "%s: Please use \"-h\" option for more info.\n", argv[0]);
                        return EXIT_FAILURE;
                }


        }

        //setDir(argc, argv);

	extern char *dirname;

	//printf(dirname);

	traversal(dirname, symbolicLink, option);

        //printf("directory scan of:  %s\n", topdir);

        return EXIT_SUCCESS;
}

void traversal(char *dir, bool symlink, char *options) {

	DIR *dp, *dire;
	struct dirent *entry;
	struct stat fileStat;
	struct queue *q;
	char * name;
	long len_parent, len_child;
	extern char *dirname;

	q = makeQ();
	enqueue(q, dir);
	
	dire = opendir(dir);
	fileInfoBuilder(dir, symlink, options);
	show();
	closedir(dire);

	while(!isEmpty(q)) {
		
		//printf("\n\nHELLO\n\n");

		struct node *next = dequeue(q);
		dp = opendir(next->dirname);

		len_parent = strlen(next->dirname);
		
		while((entry = readdir(dp)) != NULL) {
			/*
 			*  DOES NOT ENTER THIS IF STATEMENT 
 			*/
			//if((strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0) && (strcmp(entry->d_name, ".git") != 0)) 
			if((strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0) && (strcmp(entry->d_name, ".git") != 0)) {	
				printf("\n\nHELLO\n\n");
				len_child = strlen(entry->d_name);
				name = (char *)malloc(sizeof(len_parent + len_child + 2));
				name = concatPathLine(len_parent, len_child, next->dirname, entry->d_name);
				if(lstat(name, &fileStat) < 0) {
					printf("ERROR: %s: %s\n", name, strerror(errno));
					free(name);
				}
				else {
					if(S_ISDIR(fileStat.st_mode)){
						printf("hello");
						enqueue(q, name);
						fileInfoBuilder(dirname, symlink, options);
						show();
					}
					else {
						printf("\nhello\n");
						fileInfoBuilder(dirname,symlink,options);
						show();
						free(name);
					}
				}
			}
			closedir(dp);
		}

	}

}

