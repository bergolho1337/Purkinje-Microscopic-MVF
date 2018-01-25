#include "../include/queue.h"

// Queue constructor
Queue* newQueue ()
{
	Queue *queue = (Queue*)malloc(sizeof(Queue));
	queue->head = NULL;
	queue->last = NULL;
	queue->in_the_queue = 0;
	return queue;
}

// QNode constructor
QNode* newQNode (QNode *next, QNode *prev, int id, double d_ori[])
{
	QNode *qNode = (QNode*)malloc(sizeof(QNode));
	qNode->next = next;
	qNode->prev = prev;
	qNode->id = id;
    memcpy(qNode->d_ori,d_ori,sizeof(double)*3);
	return qNode;
}

void Enqueue (Queue **q, int id, double d_ori[])
{
	// Empty queue
	if ((*q)->head == NULL)
	{
		QNode *qNode = newQNode(NULL,NULL,id,d_ori);
		(*q)->head = qNode;
		(*q)->last = qNode;
	}
	// Someone is already on the queue
	else
	{
		QNode *qNode, *aux;
		aux = (*q)->last;
		qNode = newQNode(NULL,aux,id,d_ori);
		aux->next = qNode;
		(*q)->last = qNode;
	}
	(*q)->in_the_queue++;
}

QNode* Dequeue (Queue **q)
{
	QNode *ptr = (*q)->head;
	if ((*q)->head == (*q)->last)
	{	
		(*q)->head = NULL;
		(*q)->last = NULL;
	}
	else
	{
		(*q)->head = (*q)->head->next;
		(*q)->head->prev = NULL;
	}
	(*q)->in_the_queue--;
	return ptr;
}

bool isEmpty (Queue *q)
{
	return (q->in_the_queue == 0) ? true : false;
}

// Print the queue elements
void printQueue (Queue *q)
{
	QNode *qNode = q->head;
	cout << "============== GROWING QUEUE =========================================" << endl;
	cout << "head --> last" << endl;
	cout << "In the queue = " << q->in_the_queue << endl;
	while (qNode != NULL)
	{
		cout << qNode->id << " (" << qNode->d_ori[0] << "," << qNode->d_ori[1] << "," << qNode->d_ori[2] << ") -- ";
		qNode = qNode->next;
	}
	cout << "\n======================================================================" << endl;
	cout << endl;
}