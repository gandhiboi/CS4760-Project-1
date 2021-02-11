#include "queue.h"
#include "permissions.h"

//concats complete path of directory and/or file
char * addPath(long parent, long child, char *start, char *next) {
        char *temp = (char *)malloc(sizeof(parent + child + 2));
        strcpy(temp, start);
        strcat(temp, "/");
        strcat(temp, next);
        return temp;
}


int isEmpty(struct queue *q) {
	return !(q->index);
}

//initializes queue
struct queue *initQueue() {
	struct queue *q = (struct queue *)malloc(sizeof(struct queue));
	q->index = 0;
	q->front = NULL;
	q->rear = NULL;
	return q;
}

//adds directory to queue
void enqueue(struct queue *q, char *path) {
	struct node *newNode = (struct node *)malloc(sizeof(struct node));
	newNode->dirname = path;
	newNode->nextNode = NULL;
	
	if(!isEmpty(q)) {
		q->rear->nextNode = newNode;
		q->rear = newNode;
	}
	else {
		q->front = q->rear = newNode;
	}
	q->index++;
}

//removes directory from queue
struct node *dequeue(struct queue *q) {
	struct node *temp;
	temp = q->front;
	q->front = q->front->nextNode;
	q->index--;
	return temp;
}

