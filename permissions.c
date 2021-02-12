#include  "permissions.h"

#define TIMEVAR 26

char fileInfo[256] = "";
char *dirname;

//initializes the directory name, if specified use that directory, if not use cwd
void initDirectory(int argc, char *argv[]) {
        if (optind < argc) {
                dirname = argv[optind];
        }
        if (dirname == NULL) {
                dirname = ".";
        }
}

//responsible for creating the string containing all file/dir info
void concat(const char *str) {
	strcat(fileInfo, str);
}

void filePermissions(char *path, bool symlink, char* options) {

	int i;			//iterator
	struct group *grp;	
	struct passwd *pwd;
	struct stat fileStat;

	char hold[128];					//temp var to hold gid
	char hold2[128];				//var to hold num links
	char hold3[128];				//temp var to hold uid
	
	char mtime[TIMEVAR];				//str to hold date
	int link;
	char buffer[256];				//holds symbolic link path

	if(lstat(path, &fileStat) == -1) {
		perror("permissions.c: error: failed to get file status");
		exit(EXIT_FAILURE);
	}
	
	//loops through all the options passed from main
        for (i = 0; i < strlen(options); i++) {
                switch (options[i]) {

			//checks if there is a symbolic link, if error occurs terminate, if not concatenate to string
			case 'L':
				if(symlink) {
					if((fileStat.st_mode & S_IFMT) == S_IFLNK) {
						if(link = readlink(path, buffer, sizeof(buffer)) == -1) {
							perror("permissions.c: error: readlink()");
							exit(EXIT_FAILURE);
						}
						else {
							concat("Symbolic Link: ");
							concat(buffer);
							concat(" ");
						}
					}
				}
				break;
			
			//retrieves current time and date and concatenates to string
                        case 'd':
				strncpy(mtime, ctime(&fileStat.st_mtime), TIMEVAR - 1);
				mtime[TIMEVAR - 2] = 0;									
				concat(mtime);
				concat(" ");
				break;

			//calls function to retrieve byte size and display appropriate unit
                        case 's':
				concat(sizeFile(fileStat.st_size));
				concat("\t");
				break;

			//gets user id and stores it, if no user id store the user's id number
                        case 'u':
                                pwd = getpwuid(fileStat.st_uid);
                                if ((pwd = getpwuid(fileStat.st_uid)) != NULL) {
                                        concat(pwd->pw_name);
					concat(" ");
				}
                                else {
					sprintf(hold3, "%d", fileStat.st_uid);
                                        concat(hold3);
					concat(" ");
				}
                                break;

			//same as user except for group id/number
                        case 'g':
                                grp = getgrgid(fileStat.st_gid);
                                if ((grp = getgrgid(fileStat.st_gid)) != NULL) {
                                        concat(grp->gr_name);
					concat(" "); 
				}
                                else {
					sprintf(hold, "%d", fileStat.st_gid);
                                        concat(hold);
					concat(" ");
				}
                                break;

			//displays the number of links to the file
                        case 'i':
				sprintf(hold2, "%d", fileStat.st_nlink);
                                concat(hold2);
				concat(" ");
                                break;
			
			//gets the type of file and cats to string
                        case 't':
                                if(S_ISREG(fileStat.st_mode)) {
                                        concat("FILE");
                                }
                                else if(S_ISLNK(fileStat.st_mode)) {
                                        concat("SYMLINK");
                                }
                                else if(S_ISCHR(fileStat.st_mode)){
                                       concat("CHARACTER"); 
                                }
				else if(S_ISBLK(fileStat.st_mode)) {
					concat("BLOCK");
				}
				else if(S_ISFIFO(fileStat.st_mode)) {
					concat("FIFO/PIPE");
				}
				else if(S_ISREG(fileStat.st_mode)) {
					concat("FILE");
				}
				else if(S_ISSOCK(fileStat.st_mode)) {
					concat("SOCKET");
				}
				else if(S_ISDIR(fileStat.st_mode)) {
					concat("DIRE");
				}
				concat(" ");
                                break;

			//gets file permissions
                        case 'p':
                                if (S_ISDIR(fileStat.st_mode)) {
                                        concat("d");
				}
                                else if (S_ISLNK(fileStat.st_mode)) {					//l if it's a symbolic link
                                        concat("l");
				}
                                else {
                                        concat("-"); 
				}

                                (fileStat.st_mode & S_IRUSR) ? concat("r") : concat("-");
                                (fileStat.st_mode & S_IWUSR) ? concat("w") : concat("-");
                                (fileStat.st_mode & S_IXUSR) ? concat("x") : concat("-");
                                (fileStat.st_mode & S_IRGRP) ? concat("r") : concat("-");
                                (fileStat.st_mode & S_IWGRP) ? concat("w") : concat("-");
                                (fileStat.st_mode & S_IXGRP) ? concat("x") : concat("-");
                                (fileStat.st_mode & S_IROTH) ? concat("r") : concat("-");
                                (fileStat.st_mode & S_IWOTH) ? concat("w") : concat("-");
                                (fileStat.st_mode & S_IXOTH) ? concat("x") : concat("-");
				concat(" ");
                                break;
                }
        }
	
	//if the directory path specified is ../ then add three spots for the characters in the full path name 
	if(strstr(path, "../")) {
		path += 3;
		concat(path);
	}
	//similar to above, but add only two spots for the characters
	else if(strstr(path, "./")) {
		path += 2;
		concat(path);
	}
}

//calculates the file size and displays corresponding unit
char *sizeFile(const int fileSize) {
        char *size = (char *)malloc(500);

        if((fileSize / 1073741824) > 0){
                sprintf(size, "%dG", (fileSize / 1073741824));
                return size;
        }
        else if((fileSize / 1048576) > 0) {
                sprintf(size, "%dM", (fileSize / 1048576));
                return size;
        }
        else if((fileSize / 1024) > 0) {
                sprintf(size, "%dK", (fileSize / 1024));
                return size;
        }
        else {
                sprintf(size, "%d", fileSize);
                return size;
        }
        size = NULL;
}

//print the string containing all of the file/dir info and clear it for next file/dir
void display() {
	printf("%s\n", fileInfo);
	fileInfo[0] = 0;
}
