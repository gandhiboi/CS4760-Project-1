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

//Prototype for breadth-first function
void traversal(char *dir, bool symlink, char *options);
void usage();

int main(int argc, char* argv[]) {

        int opt;
        bool symbolicLink = false;
        char option[10] = "";


	
        while ((opt = getopt(argc, argv, "hLdgipstul")) != -1) {
                switch (opt) {

                case 'h':
			usage();
                        exit(EXIT_SUCCESS);

                case 'L':
                        symbolicLink = true;
			strcat(option, "L");
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
                        perror("bt.c: error: please use -h option for more info.\n");
                        exit(EXIT_FAILURE);
                }


        }

	//Initializes dirname with current directory (.) or argument provided
        initDirectory(argc, argv);

	//Used to reference dirname from different source file (permissions.c)
	extern char *dirname;
	printf("%s",dirname);					//Prints first directory name

	//does the breadth-first traversal of initial directory
	traversal(dirname, symbolicLink, option);

        return EXIT_SUCCESS;
}

void traversal(char *dir, bool symlink, char *options) {

	//Initializations to use dirent structure, stat permissions, and directory streams
	DIR *dp = NULL, *dire = NULL;			//directory streams; used to open and change dir
	struct dirent *entry;				//dirent structure; used to obtain info about dir
	struct stat fileStat;				//file permissions; similar to dirent

	struct queue *q;				//
	
	//holds the name of complete path of directory/file
	char * name;
	
	//Used for dynamically allocating size of name
	long len_parent, len_child;
	extern char *dirname;

	//initializes the queue and queues the first dir
	q = initQueue();
	enqueue(q, dir);
	
	//Opens the directory that was specified, if none then current is used
	dire = opendir(dir);
	filePermissions(dir, symlink, options);			//filters through the options used and concatenates file/dir info to string
	display();						
	closedir(dire);


	//loops until the queue is empty
	while(!isEmpty(q)) {

		//dequeues top node in queue and opens so that it can be traversed
		struct node *next = dequeue(q);
		dp = opendir(next->dirname);

		len_parent = strlen(next->dirname);
		
		//loops until there entry is not pointing at null
		while((entry = readdir(dp)) != NULL) {
			if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".git") == 0) {			//ignores current and previous dir and git repository
				continue;
			}			

			len_child = strlen(entry->d_name);
			name = (char *)malloc(sizeof(len_parent + len_child + 2));					//dynamically allocates memory for name of path
			name = addPath(len_parent, len_child, next->dirname, entry->d_name);				//concatenates next dir/file to the whole path name
			if(stat(name, &fileStat) == -1) {
				perror("bt.c: error: stat failed in traversal");
				free(name);
			}
			else {
				//queues if it is a directory and displays info otherwise just display info
				if(S_ISDIR(fileStat.st_mode)){								
					enqueue(q, name);
					printf(name);
					printf(" ");
					filePermissions(name, symlink, options);
					display();
				}
				else {
					printf(name);
					printf(" ");
					filePermissions(name,symlink,options);
					display();
				}
			}
		}
			closedir(dp);
	}
	name = NULL;
}

//help menu
void usage() {
	printf("=============================USAGE:=============================\n\n");
	printf("bt [-h] [-I n] [-L -d -g -i -p -s -t -u | -l] [dirName].\n\n");
        printf("-h      :       Print a help message and exit.\n");
        printf("-L      :       Follow symbolic links, if any. Default will be to not follow symbolic links.\n");
        printf("-t      :       Print information on file type.\n");
        printf("-p      :       Print permission bits as rwxrwxrwx.\n");
        printf("-i      :       Print the number of links to file in inode table.\n");
        printf("-u      :       Print the UID associated with the file.\n");
        printf("-g      :       Print the GID associated with the file.\n");
        printf("-s      :       Print the size of file in bytes. Appropriate suffixes have been added.\n");
        printf("-d      :       Show the time of last modification.\n");
        printf("-l      :       Used to print information on the file as if the options tpiugs are all specified.\n");

}
