/**
 * @file 		queue.c
 * @author		Berki Tamás - PDRE31
 * @date		2017.12.02.
 */

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

/*
Ez az absztrakt adatstruktúra nem feladatspecifikus.
Típuselrejtéssel dolgoztam végig, így máshol is fel lehet használni nulla módosítással
*/

/**
 * A várólista elemeit leíró adatstruktúra
 */
typedef struct _queue_e_t {
	void *data; ///< Az adat (akármi lehet)
	struct _queue_e_t *right; ///<A tőle jobbra levő elem pointere
} _queue_e_t;

/**
 * A várólistát leíró adatstruktúra
 */
typedef struct _queue_t {
	_queue_e_t *head; ///< A várólista első eleme
} _queue_t;

my_queue_t init_queue()
{
	_queue_t *q;
	if(!(q = malloc(sizeof(_queue_t)))) {
		fprintf(stderr, "Memory allocation error\n");
		exit(0);
	}
	q->head = NULL;
	return (void*)q;
}

void enqueue(my_queue_t q, void *data)
{
	_queue_t *qq = (_queue_t*) q;
	_queue_e_t *e;
	if(qq) {
		if(!(e = malloc(sizeof(_queue_e_t)))) {
			fprintf(stderr, "Memory allocation error\n");
			exit(0);
		}
		e->data = data;
		e->right = NULL;
		//Sajnos itt nincs olyan egyszerű dolgunk, mint a veremnél, mert
		//mindig a végére szúrunk be egy új elemet. (Láncolt listának a hátránya)
		if(qq->head) {
			_queue_e_t *temp = qq->head;
			while(temp->right) {
				temp = temp->right;
			}
			temp->right = e;
		} else {
			qq->head = e;
		}
	}
}

void * dequeue(my_queue_t q)
{
	_queue_t *qq = (_queue_t*)q;
	if (!qq || !qq->head) return NULL; //Kikerüljük a segfaultot
	void *data = qq->head->data;
	_queue_e_t *temp = qq->head->right;
	free(qq->head);
	qq->head = temp;
	return data;
}

void * head(my_queue_t q)
{
	_queue_t *qq = (_queue_t*)q;
	if (!qq || !qq->head) return NULL; //Kikerüljük a segfaultot
	return qq->head->data;
}

void free_queue(my_queue_t q)
{
	_queue_t *qq = (_queue_t*)q;
	if (qq && qq->head) {
		//Nem csak a várólistát, hanem annak elemeit, és annak elemét is fel kell szabadítani
		while(qq->head->right != NULL) {
			_queue_e_t *temp = qq->head->right;
			free(qq->head->data);
			free(qq->head);
			qq->head = temp;
		}
		free(qq->head);
	}
	free(qq);
}