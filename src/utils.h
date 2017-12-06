/**
 * @file 		utils.h
 * @author		Berki Tamás - PDRE31
 * @date		2017.12.02.
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

/**
 * Az adatfolyam, ahonnan a karaktereket olvassuk
 */
#define STREAM_IN stdin

/**
 * Az adatfolyan, amelybe a karaktereket írjuk
 */
#define STREAM_OUT stdout

/**
 * @brief      A mallocot tartalmazó makró, ami implementálja a hibakezelést is.
 *
 * @param      X     A pointer, amibe a malloc által lefoglalt memóriának címét fogjuk tárolni
 * @param      Y     A lefoglalalndó memória mérete
 *
 */
#define MY_MALLOC(X,Y) if(!((X) = malloc((Y)))) \
			error("MALLOC", "Memory allocation error")

/**
 * @brief      A reallocot tartalmazó makró, ami implementálja a hibakezelést is.
 *
 * @param      X     A pointer, amiben a módosítandó memória címét tároljuk
 * @param      Y     A lefoglalt memória új mérete
 *
 */
#define MY_REALLOC(X,Y) if(!((X) = realloc((X), (Y)))) \
			error("MALLOC", "Memory allocation error")

/**
 * @brief      Meghatározza a szöveg hosszát
 *
 * Az strlen alapból size_t-vel tér vissza, aminek a hossza nem definiált, csak az, hogy unsigned legyen.
 * Ezzel a függvénnyel így meghatározott (ált. 64 bit) hosszú lesz.
 *
 * @param      str   A szöveg
 *
 * @return     A szöveg hossza
 */
uint64_t strlen_long(char str[]);

/**
 * @brief      A hibakezelést végrehajtó függvény
 *
 * A program le is fog állni ezen a ponton EXIT_FAILURE kóddal.
 *
 * @param[in]  code    A hibakód (ált. rövid, nagybetűs szó)
 * @param[in]  detail  A hiba részletesebb leírása
 */
void error(const char *code, const char *detail);
#endif /* UTILS_H */