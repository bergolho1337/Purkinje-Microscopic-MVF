#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;

// =============================================================================================================
// Estrutura de um Node da fila
struct QNode
{
	QNode *next;				// Ponteiro para o proximo elemento da fila
	QNode *prev;				// Ponteiro para o elemento anterior da fila
	int id;					    // Indice do nodo em crescimento da arvore
    double d_ori[3];            // Vetor da direcao original de crescimento
}typedef QNode;

QNode* newQNode (QNode *next, QNode *prev, int id, double d_ori[]);
// =============================================================================================================
// =============================================================================================================
// Estrutura da fila
struct Queue
{
	QNode *head;				// Ponteiro para a cabeca da fila
	QNode *last;				// Ponteiro para o ultimo da fila
	int in_the_queue;			// Contador de nos em crescimento na fila
}typedef Queue; 

Queue* newQueue ();
void Enqueue (Queue **q, int id, double d_ori[]);
bool isEmpty (Queue *q);
QNode* Dequeue (Queue **q);
void printQueue (Queue *q);
// =============================================================================================================
// =============================================================================================================

#endif