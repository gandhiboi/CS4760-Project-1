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

/*
typedef struct Queue {
        int capacity;
        int size;
        int front;
        int rear;
        char** elements;
} Queue;

Queue* createQueue(int maxElements) {
        Queue* Q;
        Q = (Queue*)malloc(sizeof(Queue));
        Q->elements = (char**)malloc(sizeof(char*) * maxElements);
        Q->size = 0;
        Q->capacity = maxElements;
        Q->front = 0;
        Q->rear = -1;
        return Q;
}

void dequeue(Queue* Q, char* element) {
        if (Q->size != 0) {
                Q->size--;
                Q->front++;
                if (Q->front == Q->capacity) {
                        Q->front = 0;
                }
        }
        return;
}

char* front(Queue* Q, char* element) {
        if (Q->size != 0) {
                return Q->elements[Q->front];
        }
        return NULL;
}

void enqueue(Queue* Q, char* element) {
        if (Q->size == Q->capacity) {
                printf("Queue is full\n");
        }
        else {
                Q->size++;
                Q->rear = Q->rear + 1;
                if (Q->rear == Q->capacity) {
                        Q->rear = 0;
                }
                Q->elements[Q->rear] = (char*)malloc((sizeof element + 1) * sizeof(char));
                strcpy(Q->elements[Q->rear], element);
        }
        return;
}

*/
void breadthFirst(char *dir,bool symlink, char *options);

int main(int argc, char* argv[]) {

        int indentSpace = 4;
        int opt;
        bool symbolicLink = false;
        char option[10] = "";
        char* topdir, * targetdir, current[2] = ".";


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
                        printf("                -l      :       Used to print information on the file as if the options tpiugs are all specified.\n");

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

        if (argv[optind] == NULL) {
                char origin[4096];
                getcwd(origin, sizeof(origin));
                topdir = origin;
        }
        else {
                topdir = argv[optind];
        }

        printf("directory scan of:  %s\n", topdir);

        breadthFirst(topdir, symbolicLink, option);

        return EXIT_SUCCESS;
}

void breadthFirst(char* dir, bool symlink, char* options) {

        DIR* dp = opendir(dir);
        struct dirent* entry;
        entry = readdir(dp);
        struct stat fileStat;
        lstat(entry->d_name, &fileStat);
        struct group* grp;
        struct passwd* pwd;
        struct tm lt;
        int i;
        char* fileType = "";

        //Queue* Q = createQueue(25);

        while((entry = readdir(dp)) != NULL) {
                //Queue* Q = createQueue(3);
                if(strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0 || strcmp (".git", entry->d_name) == 0){
                        continue;
                }
                lstat(entry->d_name, &fileStat);
               // if(S_ISDIR(fileStat.st_mode)) {
               //         enqueue(Q, entry->d_name);
                       printf("\nFile Name is %s", entry->d_name);
                //        front(Q, entry->d_name);
                 //       dequeue(Q, entry->d_name);
                   //     front(Q, entry->d_name);
               // }
                //dequeue(Q, entry->d_name);
		

        for (i = 0; i < strlen(options); i++) {
		//lstat(entry->d_name, &fileStat);
                switch (options[i]) {

                        case 'd':
                                printf("%s", ctime(&fileStat.st_atime));

                        case 's':
                                if (fileStat.st_size > 1000)
                                        printf(" %dK", fileStat.st_size / 1000);
                                else if (fileStat.st_size > 1000000)
                                        printf(" %dM", fileStat.st_size / 1000000);
                                else if (fileStat.st_size > 1e+9)
                                        printf(" %dG", fileStat.st_size / 1e+9);
                                else
                                        printf(" %dB", fileStat.st_size);

                               // printf("%s      , ", dir, fileStat.st_size);

                        case 'u':
                                pwd = getpwuid(fileStat.st_uid);
                                if ((pwd = getpwuid(fileStat.st_uid)) != NULL)
                                        printf(" {%s}", pwd->pw_name);
                                else
                                        printf(" {%d}", fileStat.st_uid);
                                break;

                        case 'g':
                                grp = getgrgid(fileStat.st_gid);
                                if ((grp = getgrgid(fileStat.st_gid)) != NULL)
                                        printf(" %s", grp->gr_name);
                                else
                                        printf(" %d", fileStat.st_gid);
                                break;

                        case 'i':
                                printf("[%d] ", fileStat.st_nlink);
                                break;

                        case 't':
                                if (S_ISDIR(fileStat.st_mode)) {
                                        fileType = "directory";
                                        printf("[%s]", fileType);
                                }
                                else if (S_ISLNK(fileStat.st_mode)) {
                                        fileType = "symbolicLink";
                                        printf("[%s]", fileType);
                                }
                                else {
                                        fileType = "file";
                                        printf("[%s]", fileType);
                                }
                                break;

                        case 'p':
                                printf("\nFile Permissions: \t");

                                if (S_ISDIR(fileStat.st_mode))
                                        printf("d");
                                else if (S_ISLNK(fileStat.st_mode))
                                        printf("l");
                                else
                                        printf("-"); 

				printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
                                printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
                                printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
                                printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
                                printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
                                printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
                                printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
                                printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
                                printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
                                printf("\n");
                                break;
                }
        }

}
}

