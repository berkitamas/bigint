/**
 * @file 		stack.c
 * @author		Berki Tamás - PDRE31
 * @date		2017.12.02.
 */

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"


/*
Ez az absztrakt adatstruktúra nem feladatspecifikus.
Típuselrejtéssel dolgoztam végig, így máshol is fel lehet használni nulla módosítással
*/

/**
 * A verem elemét leíró adatstruktúra
 */
typedef struct _stack_e_t {
	void *data; ///< A veremben tárolt adat pointere
	struct _stack_e_t *left; ///< Az elemtől jobra lévő elem pointere
} _stack_e_t;

/**
 * A vermet leíró adatstruktúra
 */
typedef struct _stack_t {
	_stack_e_t *tos; ///< Top Of the Stack, a verem teteje
} _stack_t;

my_stack_t init_stack()
{
	_stack_t *s;
	if(!(s = malloc(sizeof(_stack_t)))) {
		fprintf(stderr, "Memory allocation error\n");
		exit(0);
	}
	s->tos = NULL;
	return (void*)s;
}

void push(my_stack_t s, void *data)
{
	_stack_t *ss = (_stack_t*)s;
	_stack_e_t *e;
	if(ss) {
		if(!(e = malloc(sizeof(_stack_e_t)))) {
			fprintf(stderr, "Memory allocation error\n");
			exit(0);
		}
		e->data = data;
		e->left = ss->tos;
		ss->tos = e;
	}
}

void * pop(my_stack_t s)
{
	_stack_t *ss = (_stack_t*)s;
	if (!ss || !ss->tos) return NULL; //Kikerüljük a segfaultot
	void *data = ss->tos->data;
	_stack_e_t *temp = ss->tos->left;
	free(ss->tos);
	ss->tos = temp;
	return data;
}

void * peek(my_stack_t s)
{
	_stack_t *ss = (_stack_t*)s;
	if (!ss || !ss->tos) return NULL; //Kikerüljük a segfaultot
	return ss->tos->data;
}

void free_stack(my_stack_t s)
{
	_stack_t *ss = (_stack_t*)s;
	if (ss && ss->tos) {
		while(ss->tos->left != NULL) {
			//A vermet mindenestül felszabaítjuk
			//Hártány: Nem lehet literált tárolni benne (de hisz nem is akarunk,
			//mert annak az értéke felszabadul a blokk végén)
			_stack_e_t *temp = ss->tos->left;
			free(ss->tos->data);
			free(ss->tos);
			ss->tos = temp;
		}
		free(ss->tos);
	}
	free(ss);
}