#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct node{
	char *dirname;
	struct node *nextNode;
}node;

typedef struct queue{
	int index;
	struct node *front;
	struct node *rear;
}queue;

char * addPath(long, long, char *, char *);

int isEmpty(struct queue *);

struct queue *initQueue();

void enqueue(struct queue *, char *);

struct node *dequeue(struct queue *);

#endif
