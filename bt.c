/*
 * =====================================
 * Kenan Krijestorac
 * CS 4760 - Project 1
 * 11 February 2021
 *
 * Start: 1/29/21
 * End:
 * =====================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void usage();

int main(int argc, char* argv[]) {

	int opt = 0;
	bool symbolicLink = false;
	char option[10] = "";
	
	while((opt = getopt(argc, argv, "hLdgipstul")) != -1) {
		
		switch(opt) {

			case 'h':
				usage();
				return EXIT_SUCCESS;

			case 'L':
				symbolicLink = true;
				strncat(option, "L", 2);
				break;

			case 't':
				strncat(option, "t", 2);
				break;
			case 'p':
				strncat(option, "p", 2);
				break;
			case 'i':
				strncat(option, "i", 2);
				break;
			case 'u':
				strncat(option, "u", 2);
				break;
			case 'g':
				strncat(option, "g", 2);
				break;
			case 's':
				strncat(option, "s", 2);
				break;
			case 'd':
				strncat(option, "d", 2);
				break;
			case 'l':
				strcat(option, "tpiugs");
				break;
			default:
				fprintf(stderr, "%s: Please use \"-h\" option more info.\n", argv[0]);
				return EXIT_FAILURE;
		}
	printf(option);
	}

	return EXIT_SUCCESS;

}

void usage() {
	printf("\nUSAGE:\n");
	printf("	bt [-h] [-I n] [-L -d -g -i -p -s -t -u | -l] [dirname]\n\n");
	printf("	-h	:	Print a help message and exit\n");
	printf("	-L	:	Follow symbolic links, if any. Default will be to not follow symbols\n");
	printf("	-t	:	Print information on file type\n");
	printf("	-p	:	Print permission bits as rwxrwxrwx\n");
	printf("	-i	:	Print the number of links to the file in inode table\n");
	printf("	-u	:	Print the UID associated with the file\n");
	printf("	-g	:	Print the GID associated with the file\n");
	printf("	-s	:	Print the size of file in bytes\n");
	printf("	-d	:	Show the time of last modification\n");
	printf("	-l	:	Prints information on the file as if the options tpiugs are all specified\n");
}
