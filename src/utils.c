/**
 * @file 		utils.c
 * @author		Berki Tamás - PDRE31
 * @date		2017.12.02.
 */

#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include "utils.h"

uint64_t strlen_long(char str[])
{
	uint64_t i;
	for (i = 0; str[i] != '\0'; ++i);
	return i;
}

void error(const char *code, const char *detail)
{
	fprintf(stderr, "[ERROR: %s] %s\n", code, detail);
	exit(EXIT_FAILURE);
}