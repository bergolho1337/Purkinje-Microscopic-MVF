#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;

// =============================================================================================================
// Structure from a QNode of the queue
struct QNode
{
	QNode *next;				// Pointer to the next element
	QNode *prev;				// Pointer to the previous element
	int id;					    // Identifier of the node
    double d_ori[3];            // Growth direction vector
}typedef QNode;

QNode* newQNode (QNode *next, QNode *prev, int id, double d_ori[]);
// =============================================================================================================
// =============================================================================================================
// Structure of the Queue
struct Queue
{
	QNode *head;				// Pointer to the first of the queue
	QNode *last;				// Pointer to the last of the queue
	int in_the_queue;			// Number of nodes currently on the queue
}typedef Queue; 

Queue* newQueue ();
void Enqueue (Queue **q, int id, double d_ori[]);
bool isEmpty (Queue *q);
QNode* Dequeue (Queue **q);
void printQueue (Queue *q);
// =============================================================================================================
// =============================================================================================================

#endif