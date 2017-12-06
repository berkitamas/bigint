/**
 * @file 		ops.h
 * @author		Berki Tamás - PDRE31
 * @date		2017.12.02.
 */

#ifndef OPS_H
#define OPS_H

#include <stdbool.h>
#include "rpn.h"

/**
 * Az operátorok típusa
 */
typedef struct op_t {
	operator_type type; /**< Operátor típusa */
	int prc; /**< Operátor sorrendisége (melyiket végezzük el először) */
	char ch; /**< Operátort szimbolizáló karakter */
	/**
	 * Unáris és bináris operátorok esetében mások lesznek a bemeneti értékek mennyisége.
	 */
	union {
		operand_t(*op_unary)(operand_t); /**< Unáris operátor művelete */
		operand_t(*op_binary)(operand_t,operand_t); /**< Bináris operátor művelete */
	};
} op_t;

/**
 * @brief      Operátorok listájának lekérdezése
 *
 * Jelenleg minden operátor 1 vagy 2 operandusú lehet (kivéve a nyitó zárójel - különleges).
 * Bináris operátor esetén az operátor a két operandus között van.
 * Az unáris operátor mindig az operandus után van (kivéve + és -).
 * Nyílván matematikailag nem helyes (Lehet előtte - sinx, és körülötte is - |x|).
 * Három operandusból álló műveletek nem támogatottak (Határozott integrál, határérték, stb.).
 *
 * @return     Operátorok listája, NULL-al zárva
 */
op_t *get_ops();

/**
 * @brief      A két szám egyenlő-e?
 *
 * @param[in]  l     Szám
 * @param[in]  r     Szám
 *
 * @return     Egyenlő-e?
 */
bool equal(operand_t l, operand_t r);

/**
 * @brief      A két szám abszolútértéke egyenlő-e?
 *
 * @param[in]  l     Szám
 * @param[in]  r     Szám
 *
 * @return     Egyenlő-e?
 */
bool abs_equal(operand_t l, operand_t r);

/**
 * @brief      A bal oldali szám nagyobb-e?
 *
 * @param[in]  l     Szám
 * @param[in]  r     Szám
 *
 * @return     Nagyobb-e?
 */
bool greater(operand_t l, operand_t r);

/**
 * @brief      A bal oldali szám kisebb-e?
 *
 * @param[in]  l     Szám
 * @param[in]  r     Szám
 *
 * @return     Kisebbb-e?
 */
bool lesser(operand_t l, operand_t r);

/**
 * @brief      A bal oldali szám nagyobb vagy egyenlő-e?
 *
 * @param[in]  l     Szám
 * @param[in]  r     Szám
 *
 * @return     Nagyobb vagy egyenlő-e?
 */
bool greater_equal(operand_t l, operand_t r);

/**
 * @brief      A bal oldali szám kisebb vagy egyenlő-e?
 *
 * @param[in]  l     Szám
 * @param[in]  r     Szám
 *
 * @return     Kisebb vagy egyenlő-e?
 */
bool lesser_equal(operand_t l, operand_t r);

/**
 * @brief      Szám "készítése" literálokból
 * (nem felszabadítható memóriahelyen lévő stringből).
 *
 * @param[in]  sign  Előjel
 * @param      num   Szám (literál stringként)
 *
 * @return     A szám
 */
operand_t number_from_literal(bool sign, char *num);

/**
 * @brief      Szám "készítése"
 *
 * @param[in]  sign  Előjel
 * @param      num   A szám stringje (dinamikus memóriahelyen!)
 *
 * @return     A szám
 */
operand_t number(bool sign, char *num);

/**
 * @brief      A függvény nem csinál semmit a bemenettel.
 *
 * @param[in]  l     A szám
 *
 * @return     A szám
 */
operand_t nop(operand_t l);

/**
 * @brief      A szám negálása (-1-el szorzás)
 *
 * @param[in]  l     A szám
 *
 * @return     A negált szám
 */
operand_t negate(operand_t l);

/**
 * @brief      A szám növelése eggyel
 *
 * @param[in]  l     A szám
 *
 * @return     Az eggyel megnövelt szám
 */
operand_t increment(operand_t l);

/**
 * @brief      A szám faktoriálisa
 *
 * @param[in]  l     A szám
 *
 * @return     A szám faktoriálisa
 */
operand_t factorial(operand_t l);

/**
 * @brief      A szám hatványozása megadott számmal
 *
 * @param[in]  l     A hatványalap
 * @param[in]  r     A hatványkitevő
 *
 * @return     A hatványozott szám
 */
operand_t power(operand_t l, operand_t r);

/**
 * @brief      Két szám összeadása
 *
 * @param[in]  l     Egyik szám
 * @param[in]  r     Másik szám
 *
 * @return     Az összeg
 */
operand_t add(operand_t l, operand_t r);

/**
 * @brief      Egyik szám kivonása a másikból
 *
 * @param[in]  l     Kisebbítendő szám
 * @param[in]  r     Kivonandó szám
 *
 * @return     A különbség
 */
operand_t subtract(operand_t l, operand_t r);

/**
 * @brief      Két szám összeszorzása
 *
 * @param[in]  l     Egyik szorzótényező
 * @param[in]  r     Másik szorzótényező
 *
 * @return     A szorzat
 */
operand_t multiply(operand_t l, operand_t r);

#endif /* OPS_H */